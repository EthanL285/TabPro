#include "tablature.h"

Tablature::Tablature(QWidget *parent)
    : QWidget{parent}
{
    tabLayout = new QHBoxLayout(this);
    tabLayout->setAlignment(Qt::AlignVCenter);
    tabLayout->setSpacing(0);

    // Create Tablature
    QLabel *strings = new QLabel();
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

    // TEST: Add rest
    addRest();
    /*
    addRest();
    addRest();
    addRest();
    addRest();
    addRest();
    addRest();
    addRest(); */
}

// Selects the previous column
void Tablature::goLeft()
{
    // Retrieve index of selected column
    auto it = std::find(columns.begin(), columns.end(), selectedColumn);
    int index = std::distance(columns.begin(), it);

    // Select previous column
    if (index != 0)
    {
        columns[index - 1]->setChecked(true); // selectColumn() handles the rest
    }
}

// Selects the next column
void Tablature::goRight()
{
    // Retrieve index of selected column
    auto it = std::find(columns.begin(), columns.end(), selectedColumn);
    int index = std::distance(columns.begin(), it);

    // Select next column
    if (index != columns.size() - 1)
    {
        columns[index + 1]->setChecked(true);
    }
}

// Plays the tab
void Tablature::play()
{
    qDebug() << "Play Successful";
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

// Adds rest line to tab
void Tablature::addRest()
{
    QPushButton *rest = new QPushButton("\u2015\n\u2015\n\u2015\n\u2015\n\u2015\n\u2015");
    rest->setFixedSize(35, 180);
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
    tabLayout->addWidget(rest);
    columns.push_back(rest);

    connect(rest, &QPushButton::toggled, this, &Tablature::selectColumn); // Automatically passes parameter 'checked' to slot

    // Set rest to selected column
    rest->setChecked(true); // Activates the signal above
}

// Adds bar line to tab
void Tablature::addBarLine()
{
}

// Selects the tab column
void Tablature::selectColumn(bool checked)
{
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


/*
    "E|------------------------|\n"
    "B|------------------------|\n"
    "G|------------------------|\n"
    "D|------------------------|\n"
    "A|------------------------|\n"
    "E|------------------------|n"
*/
