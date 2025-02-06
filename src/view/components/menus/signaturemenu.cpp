#include "signaturemenu.h"
#include "signaturebutton.h"

#include <QPushButton>
#include <QGridLayout>
#include <QWidgetAction>

SignatureMenu::SignatureMenu(MenuBar *menubar, QPushButton *menuButton, QWidget *parent)
    : menubar{menubar}, menuButton{menuButton}, QMenu{parent}
{
    QWidget *gridWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(gridWidget);
    gridLayout->setSpacing(10);

    // Default time signatures
    for (int i = 0; i < 3; i++)
    {
        QString text = QString("\uE09E%1\uE09F\uE084").arg(QChar(0xE084 - i));
        QPushButton *button = new SignatureButton(text, QSize(30,30), 25, "");
        gridLayout->addWidget(button, 0, i);
        connect(button, &QPushButton::clicked, menubar, &MenuBar::onTimeSignatureClick);
        connect(button, &QPushButton::clicked, this, &SignatureMenu::onSignatureChange);
        connect(button, &QPushButton::clicked, menubar, [this]() { close(); });
    }
    // Divider
    QWidget *divider = new QWidget();
    divider->setFixedSize(100, 1);
    divider->setStyleSheet("background-color: gray;");
    gridLayout->addWidget(divider, 1, 0, 1, 3, Qt::AlignCenter);

    // Custom time signature button
    QPushButton *customButton = new QPushButton("Custom");
    customButton->setCursor(Qt::PointingHandCursor);
    customButton->setStyleSheet
    (
        "QPushButton {"
        "   background: transparent;"
        "   color: gray;"
        "   font: 600 10pt Moon;"
        "}"
        "QPushButton:hover { "
        "   color: white;"
        "}"
    );
    gridLayout->addWidget(customButton, 2, 0, 1, 3);

    // Action widget
    QWidgetAction *action = new QWidgetAction(this);
    action->setDefaultWidget(gridWidget);
    addAction(action);

    // Custom time signature menu
    customMenu = createCustomSignatureMenu();
    connect(customButton, &QPushButton::clicked, this, &SignatureMenu::openCustomMenu);
    connect(menuButton, &QPushButton::clicked, this, &SignatureMenu::openMenu);
}

// Creates the custom time signature menu
QMenu *SignatureMenu::createCustomSignatureMenu()
{
    QMenu *menu = new QMenu(this);
    QWidget *gridWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(gridWidget);
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(10);

    // Time signature display
    signatureDisplay = new SignatureWidget(menubar->getSelectedSignature());
    signatureDisplay->setStyleSheet("border: none; outline: none; text-align: center");
    signatureDisplay->setCursor(Qt::ArrowCursor);
    gridLayout->addWidget(signatureDisplay, 0, 0);

    // Spinboxes
    QVBoxLayout *spinBoxLayout = new QVBoxLayout();
    QPair<int, int> timeSignature = MenuBar::parseSignature(menubar->getSelectedSignature());

    topSpinBox = new SpinBox(timeSignature.first, QPair(1, 32));
    bottomSpinBox = new SpinBox(timeSignature.second, QPair(1, 64));
    topSpinBox->setObjectName("TopSpinBox");
    bottomSpinBox->setObjectName("BottomSpinBox");

    connect(topSpinBox, &SpinBox::textChanged, this, &SignatureMenu::onSpinBoxChange);
    connect(bottomSpinBox, &SpinBox::textChanged, this, &SignatureMenu::onSpinBoxChange);

    spinBoxLayout->setContentsMargins(11, 6, 6, 6);
    spinBoxLayout->setSpacing(10);
    spinBoxLayout->addWidget(topSpinBox);
    spinBoxLayout->addWidget(bottomSpinBox);
    gridLayout->addLayout(spinBoxLayout, 0, 1);

    // Divider
    QWidget *divider = new QWidget();
    divider->setFixedSize(110, 1);
    divider->setStyleSheet("background-color: gray;");
    gridLayout->addWidget(divider, 1, 0, 1, 3, Qt::AlignCenter);

    // Confirm button
    QPushButton *confirmButton = new QPushButton("Confirm");
    confirmButton->setCursor(Qt::PointingHandCursor);
    confirmButton->setStyleSheet
    (
        "QPushButton {"
        "   background: transparent;"
        "   color: gray;"
        "   font: 600 10pt Moon;"
        "}"
        "QPushButton:hover { "
        "   color: white;"
        "}"
    );
    gridLayout->addWidget(confirmButton, 2, 0, 1, 3);

    // Action widget
    QWidgetAction *action = new QWidgetAction(menu);
    action->setDefaultWidget(gridWidget);
    menu->addAction(action);

    return menu;
}

// Opens the main menu
void SignatureMenu::openMenu()
{
    int x = menuButton->mapToGlobal(QPoint(0, 0)).x() + 5 + (menuButton->width() - sizeHint().width()) / 2;
    int y = menuButton->mapToGlobal(QPoint(0, 0)).y() + 7 + menuButton->height();
    exec(QPoint(x, y));
}

// Opens the custom menu
void SignatureMenu::openCustomMenu()
{
    close();
    int x = menuButton->mapToGlobal(QPoint(0, 0)).x() + 5 + (menuButton->width() - customMenu->sizeHint().width()) / 2;
    int y = menuButton->mapToGlobal(QPoint(0, 0)).y() + 7 + menuButton->height();
    customMenu->exec(QPoint(x, y));
}

// Updates the custom menu on signature change
void SignatureMenu::onSignatureChange()
{
    /*
    QPair<int, int> timeSignature = MenuBar::parseSignature(customSignatureDisplay->text());
    beatsPerMeasure->setValue(timeSignature.first);
    beatUnit->setValue(timeSignature.second);
    */
}

// Updates the custom menu on spin box change
void SignatureMenu::onSpinBoxChange(int value)
{
    SpinBox *senderSpinBox = qobject_cast<SpinBox*>(sender());
    QString topDigit = signatureDisplay->getTopDigit();
    QString bottomDigit = signatureDisplay->getBottomDigit();

    // Update time signature
    QString text;
    if (senderSpinBox->objectName() == "TopSpinBox")
        text = QString("\uE09E") + QString(QChar(0xE080 + value)) + "\uE09F" + bottomDigit;
    else
        text = QString("\uE09E") + topDigit + "\uE09F" + QString(QChar(0xE080 + value));

    signatureDisplay->changeSignature(text);
}
