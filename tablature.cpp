#include "tablature.h"

#include <QTimer>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

Tablature::Tablature(Sound *sound, QWidget *parent)
    : sound{sound}, QWidget{parent}
{
    tabLayout = new QHBoxLayout(this);
    tabLayout->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    tabLayout->setSpacing(0);

    // Create Tablature
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
    tabLayout->addWidget(strings);

    // Row Layout
    rowLayout = new QVBoxLayout();
    tabLayout->addLayout(rowLayout);

    // Column layout
    columnLayout = new QHBoxLayout();
    rowLayout->addLayout(columnLayout);

    addRest();
}

// Retrieve index of the selected column
int Tablature::getSelectedColumnIndex()
{
    auto it = std::find(columns.begin(), columns.end(), selectedColumn);
    int index = std::distance(columns.begin(), it);

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
        columns[index - 1]->setChecked(true); // selectColumn() handles the rest
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
    if (index != columns.size() - 1)
    {
        columns[index + 1]->setChecked(true);
    }
}

// Plays the entire tab
void Tablature::playTab(double ms, QPushButton *play)
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
    if (index == columns.size() - 1)
    {
        index = 0;
    }

    // Initialise tempo timer
    tempo = new QTimer();
    tempo->setInterval(600);
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
    if (index < columns.size())
    {
        // Retrieve the notes in the column and store in hashmap
        getColumnInfo();
        columns[index]->setChecked(true);
        playSwitch = false; // selectColumn() identifies user intervention whilst play is active by seeing if playSwitch is false

        // Access hashmap and play note
        for (auto it = fretPositions->constBegin(); it != fretPositions->constEnd(); it++)
        {
            if (it.value() != "-")
            {
                sound->playNote(QString("%1 %2").arg(it.key()).arg(it.value()));
            }
        }
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

// Stores the notes of a column in a hash map
void Tablature::getColumnInfo()
{
    // Initialise hashmap
    if (fretPositions != nullptr)
    {
        delete fretPositions;
    }
    fretPositions = new QHash<int, QString>;
    fretPositions->insert(0, "-");
    fretPositions->insert(1, "-");
    fretPositions->insert(2, "-");
    fretPositions->insert(3, "-");
    fretPositions->insert(4, "-");
    fretPositions->insert(5, "-");

    // Parse column to retrieve fret numbers and strings
    QString notes = columns[index]->text();
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
            (*fretPositions)[stringNumber] = fretNumber;
        }
    }
}

// Adds fret number to tab
void Tablature::addFretNumber()
{
    // Retrieve pointer to the QPushButton that emitted the signal
    QPushButton *button = qobject_cast<QPushButton*>(sender());

    // Retrieve string and fret number
    QStringList parts = button->objectName().split(" ");
    int row = parts[0].toInt();
    int col = parts[1].toInt();

    // Add fret number to tab
    QString fretNumber;
    for (int i = 0; i < 6; i++)
    {
        if (i == row)
        {
            fretNumber+= QString("%1").arg(col);
        }
        else
        {
            fretNumber += "\u2015";
        }
        if (i < 5)
        {
            fretNumber += "\n";
        }
    }
    selectedColumn->setText(fretNumber);

    // Create new rest if inserted into last column
    if (selectedColumn == columns.last())
    {
        addRest();
    }
}

// Creates the rest QPushButton
QPushButton *Tablature::createRest()
{
    QPushButton *rest = new QPushButton("\u2015\n\u2015\n\u2015\n\u2015\n\u2015\n\u2015");
    rest->setFixedSize(35, 205);
    rest->setCheckable(true);
    rest->setCursor(Qt::PointingHandCursor);
    rest->setStyleSheet
    (
        "QPushButton { "
        "   border-radius: 1px;"
        "   background-color: rgb(33,33,33);"
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
    /*
    // Maximum width of tab exceeded
    int tabWidth = columns.size() * 35;
    int parentWidth = parentWidget()->width(); // Width of guitar widget in mainWidget.cpp (window area excluding side bar)
    int windowWidth = window()->width();

    int maxWidth = 0.75 * windowWidth;

    qDebug() << "Tab Width: " << tabWidth;
    qDebug() << "Parent Width: " << parentWidth;
    qDebug() << "Window Width: " << windowWidth;

    if (tabWidth > maxWidth)
    {
        qDebug() << "Exceeded";
    }
    */

    QPushButton *rest = createRest();
    columnLayout->addWidget(rest);
    columns.push_back(rest);

    // Set rest to selected column
    rest->setChecked(true);
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
            "   background-color: rgb(50,50,50);;"
            "   font: 20pt Consolas"
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
                "   background-color: rgb(33,33,33);"
                "   font: 20pt Consolas"
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

// Inserts rest after selected column
void Tablature::insertRest()
{
    if (selectedColumn == columns.last())
    {
        addRest();
    }
    else
    {
        int index = getSelectedColumnIndex();
        QPushButton *rest = createRest();
        columns.insert(index + 1, rest);
        columnLayout->insertWidget(index + 1, rest);
    }
}

void Tablature::insertSlideUp()
{

}
void Tablature::insertSlideDown()
{

}
void Tablature::insertHammerOn()
{

}
void Tablature::insertPullOff()
{

}
void Tablature::insertBend()
{

}
void Tablature::insertRelease()
{

}
void Tablature::insertVibrato()
{

}
void Tablature::insertMutedHit()
{

}
void Tablature::insertBarLine()
{

}
// Removes the notes of a column or the column itself if empty
void Tablature::remove()
{
    // Remove empty column
    if (selectedColumn->text() == "\u2015\n\u2015\n\u2015\n\u2015\n\u2015\n\u2015")
    {
        QPushButton *temp = selectedColumn;
        int index = getSelectedColumnIndex();

        // Column is not the last
        if (index != columns.size() - 1)
        {
            columns[index + 1]->setChecked(true);

            columns.remove(index);
            columnLayout->removeWidget(temp);
            delete temp;
        }
    }
    // Remove notes in column
    else
    {
        selectedColumn->setText("\u2015\n\u2015\n\u2015\n\u2015\n\u2015\n\u2015");
    }
}

/*
    "E|------------------------|\n"
    "B|------------------------|\n"
    "G|------------------------|\n"
    "D|------------------------|\n"
    "A|------------------------|\n"
    "E|------------------------|n"
*/
