#include "tablature.h"
#include "scoreupdater.h"
#include "note.h"
#include "restfactory.h"
#include "notefactory.h"
#include "tablatureindent.h"
#include "tablaturescrollarea.h"

#include <QScrollArea>
#include <QScrollBar>

const QString Tablature::EMPTY_COLUMN = "\u2015\n\u2015\n\u2015\n\u2015\n\u2015\n\u2015";

Tablature::Tablature(MenuBar *menu, Sound *sound, Staff *staff, QWidget *parent)
    : menu{menu}, sound{sound}, staff{staff}, QWidget{parent}
{
    tabLayout = new QHBoxLayout(this);
    tabLayout->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    tabLayout->setSpacing(0);

    // Tablature display box
    QFrame *frame = new QFrame();
    frame->setStyleSheet("background-color: rgb(23,23,23)");
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Size policy is handled in guitar.cpp
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Sunken);
    frame->setLineWidth(3);
    frame->setMinimumHeight(310);
    tabLayout->addWidget(frame);

    // Row Layout
    rowLayout = new QVBoxLayout(frame);
    rowLayout->setSpacing(50);
    rowLayout->setAlignment(Qt::AlignHCenter);

    // Column layout
    columnLayout = new QHBoxLayout();
    columnLayout->setAlignment(Qt::AlignHCenter);
    columnLayout->setSpacing(0);
    columnLayout->setContentsMargins(0, 0, 0, 18); // Bottom margin is space between tab and scroll bar

    // Create a margin widget for the scroll area inside the frame
    QWidget *tabMargin = new QWidget();
    tabMargin->setContentsMargins(25, 0, 25, 0);
    rowLayout->addWidget(tabMargin);

    QVBoxLayout *tabMarginLayout = new QVBoxLayout(tabMargin);
    tabMarginLayout->setAlignment(Qt::AlignCenter);

    // Set scroll area
    scrollArea = new TablatureScrollArea(this);
    scrollArea->setWidget(new QWidget());
    scrollArea->widget()->setLayout(columnLayout);
    tabMarginLayout->addWidget(scrollArea, Qt::AlignCenter);

    // Create tablature
    QLabel *strings = new QLabel();
    strings->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    strings->setStyleSheet("color: white; font: 20pt Consolas; letter-spacing: 10px;");
    strings->setText("E\nB\nG\nD\nA\nE");

    columnLayout->addWidget(strings);
    columnLayout->addWidget(new TablatureIndent(120));

    // Add leading tab button
    TablatureButton *lead = createColumn();
    columnLayout->addWidget(lead);
    tab.push_back(lead);
    selectedColumn = lead;
    lead->toggleSelect();

    // Initialise timer
    tempo = new QTimer(this);
    QObject::connect(tempo, &QTimer::timeout, this, [this]() { playColumn(playIndex++); });

    /*
     * LAYOUT (-> means the widget has the specified layout)
     * 1. TabLayout                                             - Contains ALL of the tab
     * 2. Frame -> rowLayout                                    - The display box of the tab
     * 3. tabLeftRightMargin -> tabMarginLayout                 - Serves as left and right margins for the scroll area in the frame
     * 4. scrollArea set to a widget -> columnLayout            - Scroll area is set to a new widget that has the layout we want to scroll (columnLayout)
     */
}

//////////////////////////////////////////////////////////////////////
//                          TAB FUNCTIONS                           //
//////////////////////////////////////////////////////////////////////

// Visually updates the tab
void Tablature::updateTab()
{
    ScoreUpdater::updateBarLines(staff->getNotes(), this, staff, staff->getBeatsPerMeasure());
}

// Resets the tab
void Tablature::resetTab()
{
    selectColumn(tab.last());
    for (int i = staff->getNotes().size() - 1; i >= 0; i--)
    {
        remove();
    }
    updateTab();
}

//////////////////////////////////////////////////////////////////////
//                       PLAYBACK FUNCTIONS                         //
//////////////////////////////////////////////////////////////////////

// Pauses the tab
void Tablature::pauseTab()
{
    if (!tempo->isActive()) return;

    emit tabPaused();
    tempo->stop();
    staff->selectNote(-1);
}

// Selects the previous column
void Tablature::goLeft()
{
    pauseTab();

    // Select previous column
    int index = getSelectedColumnIndex();
    if (index != 0)
    {
        selectColumn(index - 1);
        adjustScrollBarPosition(tab[index - 1], "right");
    }
}

// Selects the next column
void Tablature::goRight()
{
    pauseTab();

    // Select next column
    int index = getSelectedColumnIndex();
    if (index != tab.size() - 1)
    {
        selectColumn(index + 1);
        adjustScrollBarPosition(tab[index + 1], "left");
    }
}

// Plays the entire tab
void Tablature::playTab()
{
    // Play tab from the selected column OR beginning if the last column is selected
    int index = (selectedColumn == tab.last()) ? 0 : getSelectedColumnIndex();
    playIndex = index + 1;

    // Set tempo
    tempo->setInterval(60000 / BPM);
    tempo->start();

    playColumn(index);
}

// Plays the notes of the column at the given index
void Tablature::playColumn(int index)
{
    // Reached end of tab
    if (index >= tab.size() - 1)
    {
        pauseTab();
        selectColumn(tab.last());
        return;
    }
    // Play note
    QVector<int> fretNumbers = textToFrets(tab[index]->text());
    for (int i = 0; i < Tablature::NUM_STRINGS; i++)
    {
        if (fretNumbers[i] == EMPTY_FRET) continue;
        sound->playNote(QString::asprintf("%d %d", 5 - i, fretNumbers[i]));
    }
    // Select column
    selectColumn(index);
    staff->selectNote(index);
    adjustScrollBarPosition(tab[index], "left");
}

// Changes the BPM of the tab via buttons
void Tablature::changeTempoButton(QLineEdit *field, QPushButton *increase, QPushButton *decrease)
{
    int tempo = field->text().toInt();
    int changeAmount = (qobject_cast<QPushButton*>(sender())->text() == "+") ? 10 : -10;
    tempo += changeAmount;

    // Disable or enable buttons based on tempo limits
    increase->setEnabled(tempo <= 250);
    decrease->setEnabled(tempo >= 30);

    field->setText(QString::number(tempo));
    BPM = tempo;
}

// Changes the BPM of the tab via manual editing
void Tablature::changeTempoEdit(QLineEdit *field, QPushButton *increase, QPushButton *decrease)
{
    int tempo = field->text().toInt();

    // Ensure tempo is within the valid range
    if (tempo > 260) tempo = 260;
    if (tempo < 20) tempo = 20;

    // Update the button states
    increase->setEnabled(tempo <= 250);
    decrease->setEnabled(tempo >= 30);

    field->setText(QString::number(tempo));
    field->clearFocus();
    BPM = tempo;
}

//////////////////////////////////////////////////////////////////////
//                         COLUMN FUNCTIONS                         //
//////////////////////////////////////////////////////////////////////

/// NOTE: For add, replace and remove functions, staff functions must be called first
/// since updateText() updates the width of notes

// Adds a fret number to tab
void Tablature::addFretNumber()
{
    pauseTab();

    // Retrieve string and fret number
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QStringList parts = button->objectName().split(" ");
    int row = parts[0].toInt();
    int col = parts[1].toInt();

    // Create button text
    QString text = (isChordMode) ? selectedColumn->text() : EMPTY_COLUMN;
    QStringList lines = text.split('\n');

    if (row >= 0 && row < lines.size())
    {
        lines[row] = QString::number(col);
        text = lines.join('\n');
    }
    // Create note
    QVector<int> fretNumbers = textToFrets(text);
    Note *note = NoteFactory::createNote(menu->getNote(), Staff::fretToLines(fretNumbers));

    // Add or replace the fret number
    int index = getSelectedColumnIndex();
    if (selectedColumn == tab.last())
    {
        addColumn(index, text, note);
        return;
    }
    replaceColumn(index, text, note);
}

// Creates a new column
TablatureButton *Tablature::createColumn()
{
    TablatureButton *rest = new TablatureButton();
    connect(rest, &QPushButton::toggled, this, &Tablature::onColumnClick);
    return rest;
}

// Adds a column at the given index
void Tablature::addColumn(int index, const QString &text, RhythmSymbol *symbol)
{
    TablatureButton *column = createColumn();
    staff->addNote(index, symbol, column);
    staff->updateLength(Tablature::DEFAULT_BUTTON_WIDTH);

    tab.insert(index, column);
    addColumnToLayout(index, column);
    column->updateText(text);

    if (isChordMode) selectColumn(index);
    updateTab();
}

// Removes the column at the given index
void Tablature::removeColumn(int index)
{
    QPushButton *temp = tab[index];
    columnLayout->removeWidget(temp);
    tab.remove(index);

    staff->removeNote(index);
    staff->updateLength(-temp->width());

    delete temp;
    updateTab();
}

// Replaces the column at the given index
void Tablature::replaceColumn(int index, const QString &text, RhythmSymbol *symbol)
{
    if (!staff->replaceNote(index, symbol, selectedColumn)) return;
    tab[index]->updateText(text);
    updateTab();
}

// Selects the column at the given index
void Tablature::selectColumn(int index)
{
    selectColumn(tab[index]);
}

// Selects the given column
void Tablature::selectColumn(TablatureButton *column)
{
    selectedColumn->toggleSelect();
    column->toggleSelect();
    selectedColumn = column;
}

// Slot for tablature button click
void Tablature::onColumnClick(bool checked)
{
    pauseTab();
    selectColumn(qobject_cast<TablatureButton*>(sender()));
}

// Adds the column to the layout at the given index, excluding barlines
void Tablature::addColumnToLayout(int index, TablatureButton *column)
{
    int count = 0;
    for (int i = LAYOUT_OFFSET; i < columnLayout->count() + 1; i++)
    {
        if (count == index)
        {
            columnLayout->insertWidget(i, column);
            break;
        }
        QWidget *widget = columnLayout->itemAt(i)->widget();
        if (dynamic_cast<QPushButton*>(widget)) count++;
    }
}

// Retrieve index of the selected column
int Tablature::getSelectedColumnIndex()
{
    auto it = std::find(tab.begin(), tab.end(), selectedColumn);
    int index = std::distance(tab.begin(), it);

    return index;
}

// Converts tab button text to a vector of fret numbers
QVector<int> Tablature::textToFrets(const QString &text)
{
    QVector<int> fretNumbers;
    QStringList lines = text.split("\n", Qt::SkipEmptyParts);

    for (const QString &line : lines)
    {
        int fret = (line == "\u2015") ? -1 : line.toInt();
        fretNumbers.prepend(fret);
    }
    return fretNumbers;
}

//////////////////////////////////////////////////////////////////////
//                        CHORD FUNCTIONS                           //
//////////////////////////////////////////////////////////////////////

// Adds a chord to the tab
void Tablature::addChord(QVector<int> chord)
{
    pauseTab();

    // Create button text
    QString text;
    for (int i = chord.size() - 1; i >= 0; i--)
    {
        QString num = (chord[i] == -1) ? "\u2015" : QString::number(chord[i]);
        text.append(num + "\n");
    }
    text.chop(1);

    // Create note
    int index = getSelectedColumnIndex();
    Note *note = NoteFactory::createNote(menu->getNote(), Staff::fretToLines(chord));

    // Add or replace the chord
    if (selectedColumn == tab.last())
    {
        addColumn(index, text, note);
        return;
    }
    replaceColumn(index, text, note);
}

// Toggles chord mode
void Tablature::toggleChordMode()
{
    isChordMode = !isChordMode;
    staff->toggleChordMode();
}

//////////////////////////////////////////////////////////////////////
//                        TECHNIQUE FUNCTIONS                       //
//////////////////////////////////////////////////////////////////////

// Inserts rest after selected column
void Tablature::insertRestAfter()
{
    int index = (selectedColumn == tab.last()) ? tab.size() - 1 : getSelectedColumnIndex() + 1;
    addColumn(index, EMPTY_COLUMN, RestFactory::createRest(menu->getNote()));
}

void Tablature::insertSlideUp()
{
    qDebug() << "Inserted Slide Up";
}
void Tablature::insertSlideDown()
{
    qDebug() << "Inserted Slide Down";
}
void Tablature::insertHammerOn()
{
    qDebug() << "Inserted Hammer On";
}
void Tablature::insertPullOff()
{
    qDebug() << "Inserted Pull Off";
}
void Tablature::insertBend()
{
    qDebug() << "Inserted Bend";
}
void Tablature::insertRelease()
{
    qDebug() << "Inserted Release";
}
void Tablature::insertVibrato()
{
    qDebug() << "Inserted Vibrato";
}
void Tablature::insertMutedHit()
{
    qDebug() << "Inserted Muted Hit";
}
void Tablature::insertBarLine()
{
    qDebug() << "Inserted Bar Line";
}
void Tablature::undo()
{
    qDebug() << "Reverted Last Change";
}

// Removes the notes of a column or the column itself if empty
void Tablature::remove()
{
    pauseTab();
    int index = getSelectedColumnIndex();

    // Empty tab
    if (tab.size() <= 1) return;

    // Selected column is the last column
    if (selectedColumn == tab.last())
    {
        removeColumn(index - 1);
    }
    // Empty column
    else if (selectedColumn->text() == EMPTY_COLUMN)
    {
        selectColumn(index + 1);
        removeColumn(index);
    }
    // Non-empty column
    else
    {
        Note *note = dynamic_cast<Note*>(staff->getNotes()[index]);
        Rest *rest = RestFactory::createRest(note->getType());
        replaceColumn(getSelectedColumnIndex(), EMPTY_COLUMN, rest);
    }
}

//////////////////////////////////////////////////////////////////////
//                      SCROLL AREA FUNCTIONS                       //
//////////////////////////////////////////////////////////////////////

// Adjusts the scrollbar position such that the button is aligned to either edge of the viewport
void Tablature::adjustScrollBarPosition(QPushButton *button, QString alignment)
{
    QWidget *viewport = scrollArea->viewport();
    QRect viewportRect = viewport->rect();

    // Convert button's rect to the viewport's coordinate system
    QRect buttonRect = button->geometry();
    QPoint buttonPos = button->mapTo(viewport, QPoint(0, 0));
    buttonRect.moveTo(buttonPos);

    if (viewportRect.contains(buttonRect)) return;

    // Set the scrollbar value
    QScrollBar *hScrollBar = scrollArea->horizontalScrollBar();

    if (button == tab.first())
    {
        hScrollBar->setValue(0);
        return;
    }
    if (alignment == "left")
    {
        int scrollbarValue = buttonPos.x();
        hScrollBar->setValue(hScrollBar->value() + scrollbarValue);
        return;
    }
    int buttonRightEdge = buttonPos.x() + buttonRect.width();
    int viewPortRightEdge = viewportRect.width();
    int scrollbarValue = buttonRightEdge - viewPortRightEdge;
    hScrollBar->setValue(hScrollBar->value() + scrollbarValue);
}

//////////////////////////////////////////////////////////////////////
//                         TESTING FUNCTIONS                        //
//////////////////////////////////////////////////////////////////////

// Returns the layout of the tab
QHBoxLayout *Tablature::getLayout()
{
    return columnLayout;
}

// Returns the layout item at the given index
QWidget *Tablature::getLayoutItem(int index)
{
    return columnLayout->itemAt(index + LAYOUT_OFFSET)->widget();
}
