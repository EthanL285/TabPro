#include "spinbox.h"

#include <QHBoxLayout>
#include <QIntValidator>

static const int UP = 1;
static const int DOWN = -1;

SpinBox::SpinBox(int value, QPair<int, int> range, bool beatUnitBehaviour, QWidget *parent)
    : min{range.first}, max{range.second}, beatUnitBehaviour{beatUnitBehaviour}, QWidget{parent}
{
    // Container for outlining spinbox
    container = new QWidget();
    container->setObjectName("container");

    QHBoxLayout *containerLayout = new QHBoxLayout(this);
    containerLayout->addWidget(container);
    containerLayout->setContentsMargins(0,0,0,0);

    QHBoxLayout *mainLayout = new QHBoxLayout(container);
    QVBoxLayout *arrowLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(1,1,1,1);
    mainLayout->setSpacing(0);

    // Text field
    field = new QLineEdit();
    field->setFixedSize(40, 26);
    field->setStyleSheet
    (
        "QLineEdit {"
        "   border-radius: none;"
        "   padding-left: 5px;"
        "   font-weight: bold;"
        "   font-size: 15px;"
        "   background: rgb(60,60,60);"
        "   border-right: 2px solid rgb(30,30,30);"
        "}"
    );
    QIntValidator *validator = new QIntValidator(range.first, range.second);
    field->setValidator(validator);
    field->setText(QString::number(value));
    connect(field, &QLineEdit::textChanged, this, &SpinBox::onTextChanged);

    // Arrow buttons
    upArrow = createArrowButton(UP);
    downArrow = createArrowButton(DOWN);
    arrowLayout->addWidget(upArrow);
    arrowLayout->addWidget(downArrow);
    connect(upArrow, &QPushButton::clicked, this, [this]() { changeValue(UP); });
    connect(downArrow, &QPushButton::clicked, this, [this]() { changeValue(DOWN); });

    mainLayout->addWidget(field);
    mainLayout->addLayout(arrowLayout);
}

// Creates the arrow button of the spin box
QPushButton *SpinBox::createArrowButton(int direction)
{
    QString text = (direction == UP) ? "\u25B2" : "\u25BC";
    QPushButton *button = new QPushButton(text);
    button->setCursor(Qt::PointingHandCursor);
    button->setFixedSize(20, 13);
    button->setStyleSheet
    (
        "QPushButton {"
        "   background: rgb(40,40,40);"
        "   font-size: 8px;"
        "   border: none;"
        "   border-top: none;"
        "   border-bottom: none;"
        "   border-right: none;"
        "}"
        "QPushButton:hover {"
        "   background: rgb(55,55,55);"
        "}"
        "QPushButton:pressed {"
        "   background: rgb(30,30,30);"
        "}"
        "border-right: 1px solid red;"
    );
    return button;
}

// Sets the value of the spin box
void SpinBox::setValue(int value)
{
    if (!isInRange(value)) return;
    field->setText(QString::number(value));
}

// Increments or decrements the spin box value
void SpinBox::changeValue(int delta)
{
    int value = field->text().toInt() + delta;
    setValue(value);
}

// Emits the text changed signal
void SpinBox::onTextChanged(QString text)
{
    int value = text.toInt();
    bool isValid = !isInvalidValue(value);
    container->setStyleSheet(isValid ? "" : "QWidget#container { border: 1px solid red; }");

    if (isInRange(value) || value == 0) emit textChanged(value);
}

// Checks if the value is within range
bool SpinBox::isInRange(int value)
{
    return value >= min && value <= max;
}

// Checks if the value is invalid
bool SpinBox::isInvalidValue(int value)
{
    return !isInRange(value) || (beatUnitBehaviour && (value & (value - 1)) != 0);
}
