#include "staff.h"
#include "blank.h"
#include "rest.h"
#include "notefactory.h"
#include "restfactory.h"
#include "barline.h"

#include <QFrame>
#include <QPainter>
#include <QGraphicsOpacityEffect>

#define DEFAULT_HEIGHT 185
#define LINE_COUNT 5
#define LINE_SPACING 15

Staff::Staff(MenuBar *menu, QWidget *parent)
    : menu{menu}, QWidget{parent}
{
    setFixedHeight(DEFAULT_HEIGHT);
    mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setContentsMargins(16,0,0,0);
    mainLayout->setSpacing(0);

    // Clef
    QLabel *trebleClef = new QLabel("\uE050");
    trebleClef->setFont(QFont("Bravura Text", 57));
    trebleClef->setFixedSize(57, 150);
    trebleClef->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(trebleClef, Qt::AlignVCenter);

    // Key Signature
    QLabel *keySignature = new QLabel("\uE261");
    keySignature->setFont(QFont("Bravura Text", 57));
    keySignature->setFixedWidth(25);
    keySignature->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(keySignature, Qt::AlignVCenter);

    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(keySignature);
    opacity->setOpacity(0.5);
    keySignature->setGraphicsEffect(opacity);

    // Time Signature
    timeSignature = new QLabel("\uE09E\uE084\uE09F\uE084");
    timeSignature->setFont(QFont("Bravura Text", 56));
    timeSignature->setFixedWidth(50);
    timeSignature->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(timeSignature, Qt::AlignVCenter);

    stringMap = createStringMap();
    noteMap = createNoteMap();
}

// Getter for beats per measure
int Staff::getBeatsPerMeasure()
{
    return beatsPerMeasure;
}

// Getter for beat unit
int Staff::getBeatUnit()
{
    return beatUnit;
}

// Getter for notes
const QVector<RhythmSymbol*> &Staff::getNotes()
{
    return notes;
}

// Returns the layout item at the given index
QWidget *Staff::getLayoutItem(int index)
{
    return mainLayout->itemAt(index + LAYOUT_OFFSET)->widget();
}

// Returns the layout of the staff
QHBoxLayout *Staff::getLayout()
{
    return mainLayout;
}

// Creates the staff
void Staff::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::white);
    pen.setWidth(2);
    painter.setPen(pen);

    int centerY = height() / 2;

    // Draw the staff lines
    for (int i = 0; i < LINE_COUNT; i++)
    {
        int y = centerY - ((LINE_COUNT - 1) * LINE_SPACING) / 2 + i * LINE_SPACING;
        painter.drawLine(0, y, width(), y);
    }
}

// Updates the length of the note line to be the same as the tab length
void Staff::updateLength(bool increase, int multiplier)
{
    length += (increase ? 35 : -35) * multiplier;
    setFixedWidth(length);
}

// Initialises a map of strings that corresponds to a line on the staff
QMap<int, QPair<int, int>> Staff::createStringMap()
{
    // QPair<staff line, noteMap key>
    QMap<int, QPair<int, int>> stringMap;

    stringMap[0] = qMakePair(-11, 4); // E string
    stringMap[1] = qMakePair(-8, 9);  // A string
    stringMap[2] = qMakePair(-5, 2);  // D string
    stringMap[3] = qMakePair(-2, 7);  // G string
    stringMap[4] = qMakePair(0, 11);  // B string (staff center line)
    stringMap[5] = qMakePair(3, 4);   // E string

    return stringMap;
}

// Initialises a map of notes that corresponds to a number
QMap<int, QString> Staff::createNoteMap()
{
    QMap<int, QString> noteMap;
    noteMap[0] = "C";
    noteMap[1] = "C#";
    noteMap[2] = "D";
    noteMap[3] = "D#";
    noteMap[4] = "E";
    noteMap[5] = "F";
    noteMap[6] = "F#";
    noteMap[7] = "G";
    noteMap[8] = "G#";
    noteMap[9] = "A";
    noteMap[10] = "A#";
    noteMap[11] = "B";

    return noteMap;
}

// Adds the selected note to the staff
bool Staff::addNote(QVector<int> fretNumbers, int index, bool isChord)
{
    QVector<int> staffLines(6);
    staffLines.fill(INVALID_LINE);

    // Get staff lines of the current note if chord mode is toggled
    if (!isChord && chordMode && index < notes.size() && !dynamic_cast<Blank*>(notes[index]))
    {
        Note *note = dynamic_cast<Note*>(notes[index]);
        if (note) staffLines = note->getStaffLines();
    }

    // Update staff line vector
    for (int string = 0; string < 6; string++)
    {
        if (fretNumbers[string] == -1) continue;

        // Determine which staff line to place the note on
        QPair<int, int> stringInfo = stringMap[string];
        int staffLine = stringInfo.first;
        int stringKey = stringInfo.second + 1;

        for (int i = 0; i < fretNumbers[string]; i++)
        {
            // Increment staff line by 1 if current note is not a sharp
            int key = (stringKey + i) % 12;
            if (!noteMap[key].contains("#")) staffLine++;
        }
        staffLines[string] = staffLine;
    }
    // Add note to the staff
    Note *note = NoteFactory::createNote(menu->getSelectedNote(), staffLines);
    int maxLine = *std::max_element(staffLines.begin(), staffLines.end());

    // Insertion at last index
    if (index == notes.size())
    {
        notes.append(note);
        lines.append(maxLine);
        mainLayout->addWidget(note, Qt::AlignVCenter);
    }
    // Insertion not at last index
    else if (!replaceNote(index, maxLine, note))
    {
        return false;
    }
    // Update staff height
    if (maxLine > highestLine)
    {
        updateHeight(DEFAULT_HEIGHT + (maxLine - UPDATE_LINE) * LINE_SPACING, maxLine);
    }
    return true;
}

// Updates the staff height
void Staff::updateHeight(int height, int line)
{
    setFixedHeight(height);
    highestLine = line;
}

// Toggles chord mode
void Staff::toggleChordMode()
{
    chordMode = !chordMode;
}

// Inserts a rest at the given index
void Staff::insertRest(int index, double beat, bool emitSignal)
{
    Rest *rest = RestFactory::createRest(beat);
    notes.insert(index, rest);
    lines.insert(index, -1);
    addNoteToLayout(index, rest);

    // Emit signal to tab
    if (emitSignal) emit restInserted(index);
}

// Inserts the given note at the specified index
void Staff::insertNote(int index, int line, RhythmSymbol *symbol)
{
    notes.insert(index, symbol);
    lines.insert(index, line);
    addNoteToLayout(index, symbol);
}

// Removes the note at the given index
void Staff::removeNote(int index, bool emitSignal)
{
    // Remove note
    mainLayout->removeWidget(notes[index]);
    delete notes.takeAt(index);

    // Update height
    int staffLine = lines[index];
    int max = *std::max_element(lines.begin(), lines.end());
    int count = std::count(lines.begin(), lines.end(), staffLine);
    lines.remove(index);

    if (lines.empty())
    {
        updateHeight(DEFAULT_HEIGHT, UPDATE_LINE);
    }
    else if (staffLine > UPDATE_LINE && staffLine == max && count == 1)
    {
        int secondMax = *std::max_element(lines.begin(), lines.end());
        int newHeight = (secondMax > UPDATE_LINE) ? DEFAULT_HEIGHT + (secondMax - UPDATE_LINE) * LINE_SPACING : DEFAULT_HEIGHT;
        int line = (secondMax > UPDATE_LINE) ? secondMax : UPDATE_LINE;
        updateHeight(newHeight, line);
    }
    // Emit signal to tablature
    if (emitSignal) emit noteRemoved(index);
}

// Adds the note to the layout at the given index
void Staff::addNoteToLayout(int index, RhythmSymbol *symbol)
{
    // Exclude bar lines from index
    int count = 0;
    for (int i = LAYOUT_OFFSET; i < mainLayout->count() + 1; i++)
    {
        if (count == index)
        {
            mainLayout->insertWidget(i, symbol);
            break;
        }
        QWidget *widget = mainLayout->itemAt(i)->widget();
        if (dynamic_cast<RhythmSymbol*>(widget)) count++;
    }
}

// Replaces the note at the given index with a symbol
bool Staff::replaceNote(int index, int line, RhythmSymbol *symbol)
{
    // Different beat values
    if (notes[index]->getBeatValue() != symbol->getBeatValue())
    {
        // Get measure info
        QPair<QVector<RhythmSymbol*>, int> info = getMeasureInfo(index);
        QVector<RhythmSymbol*> measure = info.first;
        int idx = info.second;
        measure[idx] = symbol;

        // Measure exceeded
        if (exceedsMeasure(measure))
        {
            qDebug() << "Exceeds Measure!";

            // Don't replace if note is last in measure
            if (idx == measure.size() - 1)
            {
                qDebug() << "Unable to Replace Note";
                delete symbol;
                return false;
            }

            // Remove subsequent notes until measure is not exceeded
            int count = 1;
            for (int i = idx + 1; i < measure.size(); i++)
            {
                if (exceedsMeasure(measure))
                {
                    removeNote(index + count, true);
                    measure.remove(i);
                    count++;
                    i--;
                }
            }
            double beats = getBeats(measure);
            // Append rest to fill measure
            if (beats < beatsPerMeasure)
            {
                insertRest(index + 1, beatsPerMeasure - beats, true);
            }
            // Edge Case: Deleting subsequent notes still exceeds measure - Not allowed to replace
            // Testing: [Q,Q, C, Q, Q, C]
        }
    }
    // Replace note
    removeNote(index, false);
    insertNote(index, line, symbol);

    return true;
}

// Returns the total beats in the given measure
double Staff::getBeats(QVector<RhythmSymbol*> measure)
{
    double beats = 0;
    for (RhythmSymbol *symbol : measure)
    {
        beats += symbol->getBeatValue();
    }
    return beats;
}

// Returns all notes within the measure that contains the given index
// and the note's index within that measure
QPair<QVector<RhythmSymbol*>, int> Staff::getMeasureInfo(int index)
{
    int measureIdx = 0;
    QVector<RhythmSymbol*> measure;

    // Count number of barlines before measure
    int count = 0;
    for (int i = 0; i <= index + count; i++)
    {
        QWidget *widget = mainLayout->itemAt(i + LAYOUT_OFFSET)->widget();
        if (dynamic_cast<BarLine*>(widget)) count++;
    }
    int offset = LAYOUT_OFFSET + count;

    // Collect the notes after the current note until the next barline
    for (int i = index + offset; i < mainLayout->count(); i++)
    {
        QWidget *widget = mainLayout->itemAt(i)->widget();
        if (dynamic_cast<BarLine*>(widget)) break;
        measure.append(dynamic_cast<RhythmSymbol*>(widget));
    }
    // Collect notes before the current note until the previous barline
    for (int i = index + offset - 1; i >= LAYOUT_OFFSET; i--)
    {
        QWidget *widget = mainLayout->itemAt(i)->widget();
        if (dynamic_cast<BarLine*>(widget)) break;
        measure.prepend(dynamic_cast<RhythmSymbol*>(widget));
        measureIdx++;
    }
    return QPair<QVector<RhythmSymbol*>, int>(measure, measureIdx);
}

// Checks if the total duration of the measure exceeds the time signature
bool Staff::exceedsMeasure(QVector<RhythmSymbol*> measure)
{
    double beats = 0;
    for (int i = 0; i < measure.size(); i++)
    {
        beats += measure[i]->getBeatValue();
    }
    return beats > beatsPerMeasure;
}

///////////////////////// SLOTS /////////////////////////

// Slot for note removal
void Staff::onNoteRemoved(int index)
{
    removeNote(index, false);
}

// Slot for time signature changed
void Staff::onTimeSignatureChanged(int beatsPerMeasure, int beatUnit)
{
    this->beatsPerMeasure = beatsPerMeasure;
    this->beatUnit = beatUnit;

    // Convert to unicode
    QChar beatsPerMeasureChar = QChar(0xE080 + beatsPerMeasure);
    QChar beatUnitChar = QChar(0xE080 + beatUnit);
    QString text = QString("\uE09E%1\uE09F%2").arg(beatsPerMeasureChar).arg(beatUnitChar);
    timeSignature->setText(text);
}
