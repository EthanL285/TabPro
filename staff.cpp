#include "staff.h"
#include "notes.h"

#include <QFrame>
#include <QLabel>
#include <QPainter>

#define INVALID_LINE -999
#define DEFAULT_HEIGHT 185
#define LINE_COUNT 5
#define LINE_SPACING 15

Staff::Staff(QWidget *parent)
    : QWidget{parent}
{
    setFixedHeight(DEFAULT_HEIGHT);
    mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    QPixmap trebleClef(":/Notes/Icons/Notes/Treble Clef.png");
    QSize size(145,145);
    trebleClef = trebleClef.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QLabel *trebleClefLabel = new QLabel();
    trebleClefLabel->setPixmap(trebleClef);
    trebleClefLabel->setAlignment(Qt::AlignLeft);
    trebleClefLabel->setFixedSize(86,150);
    trebleClefLabel->setStyleSheet("background: transparent;");
    mainLayout->addWidget(trebleClefLabel, Qt::AlignVCenter);

    // Test code below
    stringMap = createStringMap();
    noteMap = createNoteMap();

    notePixmaps["crotchet"] = QPixmap(":/Notes/Icons/Notes/crotchet.png");
    // addNote("crotchet");
    // addNote("crotchet");

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

// Retrieves the pixmap of the given note
QPixmap Staff::getNotePixmap(QString note)
{
    return notePixmaps[note];
}

// Adds the selected note to the staff
void Staff::addNote(QString note, QVector<int> fretNumbers, int index, bool isChord)
{
    QVector<int> staffLines(6);
    staffLines.fill(INVALID_LINE);

    // Get staff lines of the current note if chord mode is toggled
    if (!isChord && chordMode && index < notes.size())
    {
        Crotchet *crotchet = qobject_cast<Crotchet*>(notes[index]);
        if (crotchet) staffLines = crotchet->getStaffLines();
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
    QWidget *noteHead = new Crotchet(staffLines);

    // Insertion is not at last index
    int maxLine = *std::max_element(staffLines.begin(), staffLines.end());

    if (index != notes.size())
    {
        removeNote(index);
        notes.insert(index, noteHead);
        lines.insert(index, maxLine);
        mainLayout->insertWidget(index + 1, noteHead);
    }
    else
    {
        notes.append(noteHead);
        lines.append(maxLine);
        mainLayout->addWidget(noteHead, Qt::AlignVCenter);
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

// Replaces the note at the given index with a blank
void Staff::addBlank(int index)
{
    if (notes[index] != nullptr) removeNote(index);

    QWidget *blank = new Blank();
    notes.insert(index, blank);
    lines.insert(index, -1);
    mainLayout->insertWidget(index + 1, blank);
}

// Toggles chord mode
void Staff::toggleChordMode()
{
    chordMode = !chordMode;
}
