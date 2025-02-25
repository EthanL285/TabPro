#include "tabcolumn.h"
#include "tablature.h"

#include <QPainter>
#include <QVBoxLayout>
#include <QButtonGroup>

TabColumn::TabColumn(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(Tablature::DEFAULT_COLUMN_WIDTH, 205);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Fret buttons
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    for (int i = 0; i < 6; i++)
    {
        TabFret *fret = new TabFret(i);
        mainLayout->addWidget(fret);
        buttonGroup->addButton(fret);
        buttons.append(fret);

        connect(fret, &TabFret::keyPressed, this, &TabColumn::keyPressed);
        connect(fret, &TabFret::mouseMoved, this, &TabColumn::onHover);
        connect(fret, &TabFret::mouseLeft, this, &TabColumn::onLeave);
        connect(fret, &QPushButton::toggled, this, &TabColumn::onClick);
        connect(this, &TabColumn::stateChanged, fret, &TabFret::onColumnStateChange);
    }
}

// Getter for buttons
const QVector<TabFret*> &TabColumn::getButtons()
{
    return buttons;
}

// Getter for fret numbers
QVector<int> TabColumn::getFrets()
{
    QVector<int> frets;
    for (TabFret *button : buttons)
    {
        frets.append(button->getFret());
    }
    return frets;
}

// Sets the frets of each button
void TabColumn::setFrets(const QVector<int> &frets)
{
    for (int i = 0; i < Tablature::NUM_STRINGS; i++)
    {
        buttons[i]->setFret(frets[i]);
    }
}

// Returns the string of the selected button
int TabColumn::getString()
{
    for (TabFret *button : buttons)
    {
        if (button->isChecked()) return button->getString();
    }
    return -1;
}

// Selects the column
void TabColumn::select(int string)
{
    state = State::Selected;

    for (TabFret *button : buttons)
    {
        if (string != button->getString()) continue;
        button->click();
        button->setFocus();
    }
    update();
    emit stateChanged(state);
}

// Deselects the column
void TabColumn::deselect()
{
    state = State::Normal;
    update();
}

// Checks if the column is empty
bool TabColumn::isEmpty()
{
    QVector<int> frets = getFrets();
    return std::all_of(frets.begin(), frets.end(), [](int value) { return value == -1; });
}

// Checks if the column is selected
bool TabColumn::isSelected()
{
    return std::any_of(buttons.begin(), buttons.end(), [](QPushButton* button) { return button->isChecked(); });
}

// Paint event
void TabColumn::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);;
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(columnStateColours[state]);
    painter.drawRect(0, 0, width(), height());
}

// Mouse hover slot
void TabColumn::onHover()
{
    state = State::Hovered;
    update();
    emit stateChanged(state);
}

// Mouse leave slot
void TabColumn::onLeave()
{
    state = (isSelected()) ? State::Selected : State::Normal;
    update();
    emit stateChanged(state);
}

// Mouse click slot
void TabColumn::onClick()
{
    state = (isSelected()) ? State::Hovered : State::Normal;
    update();
    emit stateChanged(state);
}
