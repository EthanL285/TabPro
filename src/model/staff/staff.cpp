#include "staff.h"
#include "rest.h"
#include "restfactory.h"
#include "barline.h"

#include <QFrame>
#include <QPainter>
#include <QGraphicsOpacityEffect>

Staff::Staff(MenuBar *menu, QWidget *parent)
    : menu{menu}, QWidget{parent}
{
    setFixedWidth(length);
    mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setContentsMargins(16,0,0,0);
    mainLayout->setSpacing(0);

    // Clef
    QLabel *trebleClef = new QLabel("\uE050");
    trebleClef->setFont(QFont("Bravura Text", 57));
    trebleClef->setFixedSize(57, Staff::DEFAULT_HEIGHT);
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
    timeSignature = new SignatureWidget("\uE09E\uE084\uE09F\uE084", 57, QPoint(-50, -65), QPoint(-50, -35));
    timeSignature->setFixedSize(50, 67);
    mainLayout->addWidget(timeSignature, Qt::AlignVCenter);
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

// Toggles chord mode
void Staff::toggleChordMode()
{
    chordMode = !chordMode;
}

// Initialises a map of strings that corresponds to a staff line
const QMap<int, QPair<int, int>> Staff::STRING_MAP =
{
    {0, qMakePair(-11, 4)},  // E string
    {1, qMakePair(-8, 9)},   // A string
    {2, qMakePair(-5, 2)},   // D string
    {3, qMakePair(-2, 7)},   // G string
    {4, qMakePair(0, 11)},   // B string (staff center line)
    {5, qMakePair(3, 4)}     // E string
};

// Initialises a map of notes that corresponds to a number
const QMap<int, QString> Staff::NOTE_MAP =
{
    {0, "C"},
    {1, "C#"},
    {2, "D"},
    {3, "D#"},
    {4, "E"},
    {5, "F"},
    {6, "F#"},
    {7, "G"},
    {8, "G#"},
    {9, "A"},
    {10, "A#"},
    {11, "B"}
};

/// //////////////////////////////////////////////////////////////////
//                          NOTE OPERATIONS                         //
//////////////////////////////////////////////////////////////////////

// Adds a note at the given index
void Staff::addNote(int index, RhythmSymbol *symbol, TablatureButton *column)
{
    connect(symbol, &RhythmSymbol::widthChanged, this, &Staff::onNoteWidthChange, Qt::UniqueConnection);
    connect(column, &TablatureButton::widthChanged, symbol, &RhythmSymbol::onWidthChange, Qt::UniqueConnection);

    // Insertion at last index
    if (index == notes.size())
    {
        notes.append(symbol);
        mainLayout->addWidget(symbol, Qt::AlignVCenter);
        return;
    }
    // Insertion not at last index
    notes.insert(index, symbol);
    addNoteToLayout(index, symbol);
}

// Replaces the note at the given index
// Returns true if successful, false otherwise
bool Staff::replaceNote(int index, RhythmSymbol *symbol, TablatureButton *column)
{
    // Different beat values
    if (notes[index]->getBeatValue() != symbol->getBeatValue())
    {
        // Get measure info
        auto [measure, indexInMeasure] = getMeasureInfo(index);
        measure[indexInMeasure] = symbol;

        // Modify exceeded measure
        if (measureExceeded(measure) && !handleMeasureExceeded(measure, indexInMeasure, index)) return false;
    }
    removeNote(index);
    addNote(index, symbol, column);
    return true;
}

// Removes the note at the given index
void Staff::removeNote(int index)
{
    mainLayout->removeWidget(notes[index]);
    delete notes.takeAt(index);
}

// Highlights the note at the given index
void Staff::selectNote(int index)
{
    if (selectedNote) selectedNote->toggleSelect();
    if (index == -1)
    {
        selectedNote = nullptr;
        return;
    }
    selectedNote = notes[index];
    selectedNote->toggleSelect();
}

// Adds the column to the layout at the given index, excluding barlines
void Staff::addNoteToLayout(int index, RhythmSymbol *symbol)
{
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

// Converts a vector of fret numbers to their corresponding staff line positions
const QVector<int> Staff::fretToLines(const QVector<int> &fretNumbers)
{
    QVector<int> staffLines(6, INVALID_LINE);
    for (int string = 0; string < 6; string++)
    {
        if (fretNumbers[string] == -1) continue;

        // Determine which staff line to place the note on
        QPair<int, int> stringInfo = STRING_MAP[string];
        int staffLine = stringInfo.first;
        int stringKey = stringInfo.second + 1;

        for (int i = 0; i < fretNumbers[string]; i++)
        {
            // Current note is not a sharp
            int key = (stringKey + i) % 12;
            if (!NOTE_MAP[key].contains("#")) staffLine++;
        }
        staffLines[string] = staffLine;
    }
    return staffLines;
}

/// //////////////////////////////////////////////////////////////////
//                         MEASURE FUNCTIONS                        //
//////////////////////////////////////////////////////////////////////

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

// Checks if the total duration of the measure exceeds the time signature
bool Staff::measureExceeded(QVector<RhythmSymbol*> measure)
{
    double beats = 0;
    for (int i = 0; i < measure.size(); i++)
    {
        beats += measure[i]->getBeatValue();
    }
    return beats > beatsPerMeasure;
}

// Modifies the given exceeded measure to be within limits
// Returns true if successful, false otherwise
bool Staff::handleMeasureExceeded(QVector<RhythmSymbol*> measure, int indexInMeasure, int index)
{
    qDebug() << "Exceeds Measure!";

    // Cancel replacement if the note is last in measure
    if (indexInMeasure == measure.size() - 1)
    {
        delete measure[indexInMeasure];
        return false;
    }
    // Remove subsequent notes until measure is within limits
    while (measureExceeded(measure) && indexInMeasure + 1 < measure.size())
    {
        emit removeColumn(index + 1);
        measure.remove(indexInMeasure + 1);
    }
    // Insert a rest to fill measure
    double beats = getBeats(measure);
    if (beats < beatsPerMeasure)
    {
        Rest *rest = RestFactory::createRest(beatsPerMeasure - beats);
        emit addColumn(index + 1, EMPTY_COLUMN, rest);
    }
    return true;
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

/// //////////////////////////////////////////////////////////////////
//                      UPDATE EVENT FUNCTIONS                      //
//////////////////////////////////////////////////////////////////////

// Draws the staff lines
void Staff::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::white, 2));

    int centerY = height() / 2;
    for (int i = 0; i < LINE_COUNT; i++)
    {
        int y = centerY - ((LINE_COUNT - 1) * LINE_SPACING) / 2 + i * LINE_SPACING;
        painter.drawLine(0, y, width(), y);
    }
}

// Updates the staff length
void Staff::updateLength(int delta, int factor)
{
    length += delta * factor;
    setFixedWidth(length);
}

/// //////////////////////////////////////////////////////////////////
//                              SLOTS                               //
//////////////////////////////////////////////////////////////////////

// Slot for time signature changed
void Staff::onTimeSignatureChanged(int beatsPerMeasure, int beatUnit)
{
    this->beatsPerMeasure = beatsPerMeasure;
    this->beatUnit = beatUnit;
    timeSignature->changeSignature(beatsPerMeasure, beatUnit);
}

// Slot for note width change
void Staff::onNoteWidthChange(int newWidth, int prevWidth)
{
    updateLength(newWidth - prevWidth);
}

/// //////////////////////////////////////////////////////////////////
//                        TESTING FUNCTIONS                         //
//////////////////////////////////////////////////////////////////////

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
