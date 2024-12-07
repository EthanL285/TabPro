#include "tablature.h"
#include "barline.h"
#include "note.h"

#include <QTimer>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QScrollArea>
#include <QScrollBar>

Tablature::Tablature(Sound *sound, Staff *staff, QWidget *parent)
    : sound{sound}, staff{staff}, QWidget{parent}
{
    tabLayout = new QHBoxLayout(this);
    tabLayout->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    tabLayout->setSpacing(0);

    // Create tablature display box
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
    scrollArea = createScrollArea();
    scrollArea->setWidget(new QWidget());
    scrollArea->widget()->setLayout(columnLayout);
    tabMarginLayout->addWidget(scrollArea, Qt::AlignCenter);

    // Create tablature
    QLabel *strings = createNewTabLine();
    columnLayout->addWidget(strings);

    addRest();

    /*
     * LAYOUT (-> means the widget has the specified layout)
     * 1. TabLayout                                             - Contains ALL of the tab
     * 2. Frame -> rowLayout                                    - The display box of the tab
     * 3. tabLeftRightMargin -> tabMarginLayout                 - Serves as left and right margins for the scroll area in the frame
     * 4. scrollArea set to a widget -> columnLayout            - Scroll area is set to a new widget that has the layout we want to scroll (columnLayout)
     */
}

// Creates new tab line with strings
QLabel *Tablature::createNewTabLine()
{
    QLabel *strings = new QLabel();
    strings->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    strings->setStyleSheet("color: white; font: 20pt Consolas; letter-spacing: 10px;");
    strings->setText
    (
        "E|\u2015\n"
        "B|\u2015\n"
        "G|\u2015\n"
        "D|\u2015\n"
        "A|\u2015\n"
        "E|\u2015"
    );
    return strings;
}

// Retrieve index of the selected column
int Tablature::getSelectedColumnIndex()
{
    auto it = std::find(tab.begin(), tab.end(), selectedColumn);
    int index = std::distance(tab.begin(), it);

    return index;
}

// Stops the tempo timer
void Tablature::stopTempoTimer()
{
    playSwitch = true;
    tempo->stop();
    tempo->deleteLater();
    tempo = nullptr;
}

// Selects the previous column
void Tablature::goLeft()
{
    // Stop tempo timer
    if (tempo != nullptr)
    {
        stopTempoTimer();
        playButton->setChecked(false);
        return;
    }

    int index = getSelectedColumnIndex();

    // Select previous column
    if (index != 0)
    {
        tab[index - 1]->setChecked(true); // selectColumn() handles the rest
        adjustScrollBarPosition(tab[index - 1], "right");
    }
}

// Selects the next column
void Tablature::goRight()
{
    // Stop tempo timer
    if (tempo != nullptr)
    {
        stopTempoTimer();
        playButton->setChecked(false);
        return;
    }

    int index = getSelectedColumnIndex();

    // Select next column
    if (index != tab.size() - 1)
    {
        tab[index + 1]->setChecked(true);
        adjustScrollBarPosition(tab[index + 1], "left");
    }
}

// Plays the entire tab
void Tablature::playTab(QPushButton *play)
{
    // Stop timer if user pauses the tab
    if (tempo != nullptr)
    {
        stopTempoTimer();
        return;
    }

    // Assign play button
    if (playButton == nullptr)
    {
        playButton = play;
    }

    // Play tab from the selected column onwards OR beginning if the last column is selected
    index = getSelectedColumnIndex();
    if (index == tab.size() - 1)
    {
        index = 0;
    }

    // Initialise tempo timer
    int ms = 60000 / BPM;
    tempo = new QTimer(this);
    tempo->setInterval(ms);
    tempo->start();
    QObject::connect(tempo, &QTimer::timeout, this, &Tablature::playColumn);

    // Play the first column
    playSwitch = true;
    playColumn();
}

// Plays the notes of a column
void Tablature::playColumn()
{
    playSwitch = true;

    // Iterate through tab
    if (index < tab.size())
    {
        // Retrieve the notes in the column and store in hashmap
        getColumnInfo();
        tab[index]->setChecked(true);
        playSwitch = false; // selectColumn() identifies user intervention whilst play is active by seeing if playSwitch is false

        // Access hashmap and play note
        for (auto it = fretPositions.constBegin(); it != fretPositions.constEnd(); it++)
        {
            if (it.value() != "-")
            {
                sound->playNote(QString("%1 %2").arg(it.key()).arg(it.value()));
            }
        }
        // Adjust scroll bar position
        adjustScrollBarPosition(tab[index], "left");

        index++;
    }
    // End of tab
    else
    {
        // Stop timer and revert button state to play
        stopTempoTimer();
        playButton->setChecked(false);
    }
}

// Adjusts the scrollbar position such that the button is aligned to either edge of the viewport
void Tablature::adjustScrollBarPosition(QPushButton *button, QString alignment)
{
    QWidget *viewport = scrollArea->viewport();
    QRect viewportRect = viewport->rect();

    // Convert button's rect to the viewport's coordinate system
    QRect buttonRect = button->geometry();
    QPoint buttonPos = button->mapTo(viewport, QPoint(0, 0));
    buttonRect.moveTo(buttonPos);

    if (!viewportRect.contains(buttonRect))
    {
        // Set the scrollbar value
        QScrollBar *hScrollBar = scrollArea->horizontalScrollBar();

        if (button == tab.first())
        {
            hScrollBar->setValue(0);
        }
        else if (alignment == "left")
        {
            int scrollbarValue = buttonPos.x();
            hScrollBar->setValue(hScrollBar->value() + scrollbarValue);
        }
        else
        {
            int buttonRightEdge = buttonPos.x() + buttonRect.width();
            int viewPortRightEdge = viewportRect.width();
            int scrollbarValue = buttonRightEdge - viewPortRightEdge;
            hScrollBar->setValue(hScrollBar->value() + scrollbarValue);
        }
    }
}

// Stores the notes of a column in a hash map
void Tablature::getColumnInfo()
{
    // Initialise hashmap
    fretPositions.insert(0, "-");
    fretPositions.insert(1, "-");
    fretPositions.insert(2, "-");
    fretPositions.insert(3, "-");
    fretPositions.insert(4, "-");
    fretPositions.insert(5, "-");

    // Parse column to retrieve fret numbers and strings
    QString notes = tab[index]->text();
    QStringList lines = notes.split("\n");
    static QRegularExpression numRegex("\\d+"); // Regular expression pattern to match digits

    for (int stringNumber = 0; stringNumber < lines.size(); stringNumber++)
    {
        QRegularExpressionMatchIterator it = numRegex.globalMatch(lines[stringNumber]); // Iterator for searching through string

        // Store fret number and string in hashmap
        while (it.hasNext())
        {
            QRegularExpressionMatch match = it.next();
            QString fretNumber = match.captured();
            (fretPositions)[stringNumber] = fretNumber;
        }
    }
}

// Adds a fret number to tab
void Tablature::addFretNumber()
{
    // Retrieve string and fret number
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QStringList parts = button->objectName().split(" ");
    int row = parts[0].toInt();
    int col = parts[1].toInt();

    // NOTE: NEED TO EDIT BUTTON NAME TO BE IN THE ORDER OF EADGBE
    QVector<int> fretNumbers(6);
    fretNumbers.fill(-1);
    fretNumbers[5 - row] = col;

    // Add fret number to tab
    int idx = 0;
    QString tabColumn = (chordMode) ? selectedColumn->text() : EMPTY_COLUMN;

    for (int i = 0; i < tabColumn.size(); idx++)
    {
        int count = (i < tabColumn.size() - 1 && tabColumn[i + 1] != '\n') ? 2 : 1;
        if (idx == row)
        {
            tabColumn.replace(i, count, QString::number(col));
            break;
        }
        i += count + 1;
    }
    selectedColumn->setText(tabColumn);
    staff->addNote(fretNumbers, getSelectedColumnIndex());
    updateBarLines();
    if (selectedColumn == tab.last())  addRest();
}

// Updates the tablature barlines to match the staff barlines
void Tablature::updateBarLines()
{
    staff->updateBarLines();
    QVector<int> currBarlinePos;
    QVector<int> newBarlinePos;

    // Get positions of new barlines
    double beats = 0;
    for (int i = 0; i < staff->notes.size(); i++)
    {
        Note *note = dynamic_cast<Note*>(staff->notes[i]);
        beats += note->getBeatValue();
        if (beats >= 4)
        {
            newBarlinePos.append(i + 1);
            beats = 0;
        }
    }
    // Get positions of current barlines
    int idx = 0;
    for (int i = 1; i < columnLayout->count(); i++)
    {
        QWidget *widget = columnLayout->itemAt(i)->widget();
        if (dynamic_cast<BarLine*>(widget))
        {
            currBarlinePos.append(idx);
            continue;
        }
        idx++;
    }
    // Compare current and new positions
    for (int i = 0; i < newBarlinePos.size(); i++)
    {
        int newPos = newBarlinePos[i];
        if (currBarlinePos.contains(newPos)) continue;
        addBarLine(newPos);
    }
}

// Inserts a barline at the given index
void Tablature::addBarLine(int index)
{
    BarLine *barline = new BarLine();
    columnLayout->addWidget(barline);
}

// Adds a chord to the tab
void Tablature::addChord(QVector<int> chord)
{
    QString tabColumn;
    for (int i = chord.size() - 1; i >= 0; i--)
    {
        QString num = (chord[i] == -1) ? "\u2015" : QString::number(chord[i]);
        tabColumn.append(num + "\n");
    }
    tabColumn.chop(1);

    selectedColumn->setText(tabColumn);
    staff->addNote(chord, getSelectedColumnIndex(), true);
    if (selectedColumn == tab.last()) addRest();
}

// Toggles chord mode
void Tablature::toggleChordMode()
{
    chordMode = !chordMode;
    staff->toggleChordMode();
}

// Creates the rest QPushButton
QPushButton *Tablature::createRest()
{
    QPushButton *rest = new QPushButton(EMPTY_COLUMN);

    rest->setFixedSize(35, 205);
    rest->setCheckable(true);
    rest->setCursor(Qt::PointingHandCursor);
    rest->setFocusPolicy(Qt::NoFocus);
    rest->setStyleSheet
    (
        "QPushButton { "
        "   border-radius: 1px;"
        "   background-color: rgb(23,23,23);"
        "   font: 20pt Consolas;"
        "}"
        "QPushButton:hover { "
        "   background-color: rgb(75,75,75);"
        "}"
    );
    connect(rest, &QPushButton::toggled, this, &Tablature::selectColumn); // Automatically passes parameter 'checked' to slot
    return rest;
}

// Adds rest line to tab
void Tablature::addRest()
{
    QPushButton *rest = createRest();
    columnLayout->addWidget(rest);
    tab.push_back(rest);

    // Set rest to selected column
    if (!chordMode) rest->setChecked(true);

    // Update note line length
    staff->updateLineLength(true);
}

// Selects the tab column
void Tablature::selectColumn(bool checked)
{
    // User selects a column whilst play is active
    if (!playSwitch)
    {
        stopTempoTimer(); // Sets playSwitch back to true
        playButton->setChecked(false);
    }

    // Retrieve pointer to the QPushButton that emitted the signal
    QPushButton *column = qobject_cast<QPushButton*>(sender());

    // Column is selected (De-selecting a column does nothing)
    if (checked)
    {
        column->setStyleSheet
        (
            "QPushButton { "
            "   border-radius: 1px;"
            "   background-color: rgb(50,50,50);"
            "   font: 20pt Consolas;"
            "}"
            "QPushButton:hover { "
            "   background-color: rgb(75,75,75);"
            "}"
        );
        // Selected Column is NULL during initialisation
        if (selectedColumn != nullptr && selectedColumn != column)
        {
            selectedColumn->setStyleSheet
            (
                "QPushButton { "
                "   border-radius: 1px;"
                "   background-color: rgb(23,23,23);" // was 33,33,33
                "   font: 20pt Consolas;"
                "}"
                "QPushButton:hover { "
                "   background-color: rgb(75,75,75);"
                "}"
            );
            selectedColumn->setChecked(false);
        }
        selectedColumn = column;
    }
}

// ==================================== TECHNIQUES ====================================

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

// Inserts rest after selected column
void Tablature::insertRest()
{
    if (selectedColumn == tab.last())
    {
        addRest();
    }
    else
    {
        int index = getSelectedColumnIndex();
        QPushButton *rest = createRest();
        tab.insert(index + 1, rest);
        columnLayout->insertWidget(index + 2, rest);
    }
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
    QPushButton *temp = selectedColumn;
    int index = getSelectedColumnIndex();

    // Selected column is the last column in the tab
    if (selectedColumn == tab.last())
    {
        // Empty column
        if (tab.size() > 1 && tab[index - 1]->text() == EMPTY_COLUMN)
        {
            tab[index - 1]->setChecked(true);
            tab.remove(index);
            columnLayout->removeWidget(temp);
            delete temp;

            staff->updateLineLength(false);
            staff->removeNote(index - 1);
        }
        // Non-Empty column
        else if (tab.size() > 1)
        {
            tab[index - 1]->setText(EMPTY_COLUMN);
            tab[index - 1]->setChecked(true);
            tab.remove(index);
            delete temp;

            staff->updateLineLength(false);
            staff->removeNote(index - 1);
        }
    }
    // Empty column
    else if (selectedColumn->text() == EMPTY_COLUMN)
    {
        tab[index + 1]->setChecked(true);
        tab.remove(index);
        columnLayout->removeWidget(temp);
        delete temp;

        staff->updateLineLength(false);
        staff->removeNote(index);
    }
    // Non-empty column
    else
    {
        selectedColumn->setText(EMPTY_COLUMN);
        staff->addRest(index);
    }
}

// Creates the scroll area for the tab
QScrollArea *Tablature::createScrollArea()
{
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setStyleSheet
    (
        "QScrollBar:horizontal {" // Horizontal scrollbar
        "    border: 1px solid #222222;"
        "    background: #333333;"
        "    height: 15px;"
        "    margin: 0 22px 0 22px;"
        "}"
        "QScrollBar::handle:horizontal {" // Scrollbar handle
        "    background: rgb(80,80,80);"
        "    min-width: 20px;"
        "    border: 1px solid rgb(70,70,70);"
        "    border-right: 1px solid rgb(40,40,40);"
        "    border-left: 1px solid rgb(40,40,40);"
        "    border-radius: 1px;"
        "}"
        "QScrollBar::handle:horizontal:hover {"
        "    background: rgb(100,100,100);"
        "}"
        "QScrollBar::handle:horizontal:pressed {"
        "    background: rgb(85,85,85);"
        "}"
        "QScrollBar::add-line:horizontal {" // Buttons at the end of the scrollbar
        "    border-right: 1px solid rgb(15,15,15);"
        "    border-bottom: 1px solid rgb(15,15,15);"
        "    background: rgb(25,25,25);"
        "    width: 20px;"
        "    subcontrol-position: right;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::sub-line:horizontal {"
        "    border-left: 1px solid rgb(15,15,15);"
        "    border-bottom: 1px solid rgb(15,15,15);"
        "    background: rgb(25,25,25);"
        "    width: 20px;"
        "    subcontrol-position: left;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::add-line:horizontal:hover, QScrollBar::sub-line:horizontal:hover {"
        "    background: rgb(80,80,80);"
        "}"
        "QScrollBar::add-line:horizontal:pressed, QScrollBar::sub-line:horizontal:pressed {"
        "    background: rgb(40,40,40);"
        "}"
        "QScrollBar::left-arrow:horizontal {" // Arrows of the end buttons
        "    width: 8px;"
        "    height: 8px;"
        "    image: url(:/scroll/scroll/scroll left.png);"
        "}"
        "QScrollBar::right-arrow:horizontal {"
        "    width: 8px;"
        "    height: 8px;"
        "    image: url(:/scroll/scroll/scroll right.png);"
        "}"
        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {" // Regions to the left and right of the handle
        "    background: none;"
        "}"
    );

    // Connect scroll bar signals to slots
    QScrollBar *hScrollBar = scrollArea->horizontalScrollBar();
    hScrollBar->setSingleStep(35);

    connect(hScrollBar, &QScrollBar::rangeChanged, [hScrollBar](int min, int max)
    {
        if (max > 0) hScrollBar->setValue(max);
    });
    connect(hScrollBar, &QScrollBar::sliderPressed, this, [this]()
    {
        if (tempo != nullptr)
        {
            stopTempoTimer();
            playButton->setChecked(false);
        }
    });

    return scrollArea;
}


/*
    "E|------------------------|\n"
    "B|------------------------|\n"
    "G|------------------------|\n"
    "D|------------------------|\n"
    "A|------------------------|\n"
    "E|------------------------|n"
*/
