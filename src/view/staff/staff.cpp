#include "staff.h"
#include "crotchet.h"
#include "quaver.h"
#include "blank.h"
#include "rest.h"
#include "quarterrest.h"
#include "eighthrest.h"
#include "barline.h"

#include <QFrame>
#include <QLabel>
#include <QPainter>

#define DEFAULT_HEIGHT 185
#define LINE_COUNT 5
#define LINE_SPACING 15
#define TIME_SIGNATURE 4

Staff::Staff(MenuBar *menu, QWidget *parent)
    : menu{menu}, QWidget{parent}
{
    setFixedHeight(DEFAULT_HEIGHT);
    mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    QPixmap trebleClef(":/notes/notes/Treble Clef.png");
    QSize size(145,145);
    trebleClef = trebleClef.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QLabel *trebleClefLabel = new QLabel();
    trebleClefLabel->setPixmap(trebleClef);
    trebleClefLabel->setAlignment(Qt::AlignLeft);
    trebleClefLabel->setFixedSize(86,150);
    trebleClefLabel->setStyleSheet("background: transparent;");
    mainLayout->addWidget(trebleClefLabel, Qt::AlignVCenter);

    stringMap = createStringMap();
    noteMap = createNoteMap();
}

// Define static member
QVector<StaffSymbol*> Staff::notes;

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
void Staff::updateLineLength(bool add)
{
    length += (add ? 35 : -35);
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
void Staff::addNote(QVector<int> fretNumbers, int index, bool isChord)
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
    Note *note;
    switch (menu->getSelectedNote())
    {
        case NoteType::Semibreve:
            note = new Crotchet(staffLines);
            break;
        case NoteType::Minim:
            note = new Crotchet(staffLines);
            break;
        case NoteType::Crotchet:
            note = new Crotchet(staffLines);
            break;
        case NoteType::Quaver:
            note = new Quaver(staffLines);
            break;
        case NoteType::Semiquaver:
            note = new Quaver(staffLines);
            break;
        default:
            break;
    }

    // Insertion is not at last index
    int maxLine = *std::max_element(staffLines.begin(), staffLines.end());

    if (index != notes.size())
    {
        removeNote(index);
        notes.insert(index, note);
        lines.insert(index, maxLine);
        mainLayout->insertWidget(index + 1, note);
    }
    else
    {
        notes.append(note);
        lines.append(maxLine);
        mainLayout->addWidget(note, Qt::AlignVCenter);
    }
    // Update staff height
    if (maxLine > highestLine)
    {
        updateHeight(DEFAULT_HEIGHT + (maxLine - UPDATE_LINE) * LINE_SPACING, maxLine);
    }
}

// Updates the staff height
void Staff::updateHeight(int height, int line)
{
    setFixedHeight(height);
    highestLine = line;
}

// Removes the note at the given index
void Staff::removeNote(int index)
{
    // Remove note
    QWidget *temp = notes[index];
    notes.remove(index);
    mainLayout->removeWidget(temp);
    delete temp;

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
}

// Toggles chord mode
void Staff::toggleChordMode()
{
    chordMode = !chordMode;
}

// Replaces the note at the given index with a rest
void Staff::addRest(int index)
{
    Note *note = dynamic_cast<Note*>(notes[index]);
    NoteType type = note->getType();
    removeNote(index);

    Rest *rest;
    switch (type)
    {
        case NoteType::Semibreve:
            rest = new QuarterRest();
            break;
        case NoteType::Minim:
            rest = new QuarterRest();
            break;
        case NoteType::Crotchet:
            rest = new QuarterRest();
            break;
        case NoteType::Quaver:
            rest = new EighthRest();
            break;
        case NoteType::Semiquaver:
            rest = new EighthRest();
            break;
        default:
            break;
    }
    notes.insert(index, rest);
    lines.insert(index, -1);
    mainLayout->insertWidget(index + 1, rest);
}

// Inserts a bar line at the given index
void Staff::addBarLine(int index)
{
    int idx = 0;
    for (int i = 0; i < mainLayout->count(); i++)
    {
        // Insert bar line
        if (idx == index)
        {
            mainLayout->insertWidget(i + 1, new BarLine());
            updateLineLength(true);
            return;
        }
        // Increment if widget is not a barline
        QWidget *widget = mainLayout->itemAt(i)->widget();
        if (!dynamic_cast<BarLine*>(widget)) idx++;
    }
}

// Updates the position of all bar lines
void Staff::updateBarLines()
{
    QVector<int> currBarlinePos;
    QVector<int> newBarlinePos;

    // Get positions of new barlines
    double beats = 0;
    for (int i = 0; i < notes.size(); i++)
    {
        Note *note = dynamic_cast<Note*>(notes[i]);
        beats += note->getBeatValue();
        if (beats >= TIME_SIGNATURE)
        {
            newBarlinePos.append(i + 1);
            beats = 0;
        }
    }
    // Get positions of current barlines
    int idx = 0;
    for (int i = 1; i < mainLayout->count(); i++)
    {
        QWidget *widget = mainLayout->itemAt(i)->widget();
        if (dynamic_cast<BarLine*>(widget))
        {
            currBarlinePos.append(idx);
            continue;
        }
        idx++;
    }
    // Compare current and new positions
    for (int newPos : newBarlinePos)
    {
        if (!currBarlinePos.contains(newPos)) addBarLine(newPos);
    }
}
