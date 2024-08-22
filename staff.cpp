#include "staff.h"
#include "notes.h"

#include <QFrame>
#include <QLabel>
#include <QPainter>

Staff::Staff(QWidget *parent)
    : QWidget{parent}
{
    setFixedHeight(185);
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

    const int lineCount = 5;
    const int lineSpacing = 15;
    int centerY = height() / 2;

    // Draw the staff lines
    for (int i = 0; i < lineCount; i++)
    {
        int y = centerY - ((lineCount - 1) * lineSpacing) / 2 + i * lineSpacing;
        painter.drawLine(0, y, width(), y);
    }
}

// Initialises a map of strings that corresponds to a line on the staff
QMap<int, QPair<int, int>> Staff::createStringMap()
{
    // QPair<staff line, noteMap key>
    QMap<int, QPair<int, int>> stringMap;

    stringMap[5] = qMakePair(-11, 4); // E string
    stringMap[4] = qMakePair(-8, 9);  // A string
    stringMap[3] = qMakePair(-5, 2);  // D string
    stringMap[2] = qMakePair(-2, 7);  // G string
    stringMap[1] = qMakePair(0, 11);  // B string (staff center line)
    stringMap[0] = qMakePair(3, 4);   // E string

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
void Staff::addNote(QString note, int string, int fretNumber)
{
    // Determine which staff line to place the note on
    QPair<int, int> stringInfo = stringMap[string];
    int staffLine = stringInfo.first;
    int stringKey = stringInfo.second + 1;

    for (int i = 0; i < fretNumber; i++)
    {
        // Increment staff line by 1 if current note is not a sharp
        int key = (stringKey + i) % 12;
        if (!noteMap[key].contains("#")) staffLine++;
    }

    // Add note to the staff
    QWidget *noteHead = new Crotchet(staffLine);
    notes.append(noteHead);
    mainLayout->addWidget(noteHead, Qt::AlignVCenter);
}

// Updates the length of the note line to be the same as the tab length
void Staff::updateLineLength(bool add)
{
    length += (add ? 35 : -35);
    setFixedWidth(length);
}

// Removes the note at the given index
void Staff::removeNote(int index)
{
    QWidget *temp = notes[index];
    notes.remove(index);
    mainLayout->removeWidget(temp);
    delete temp;
}

// Replaces the note at the given index with a blank
void Staff::addBlank(int index)
{
    if (notes[index] != nullptr) removeNote(index);

    QWidget *blank = new Blank();
    notes.insert(index, blank);
    mainLayout->insertWidget(index + 1, blank);
}

// test
