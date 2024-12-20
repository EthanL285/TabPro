#include "menubar.h"
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QDockWidget>
#include <QPushButton>
#include <QLabel>

MenuBar::MenuBar(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create menu bar
    QMenuBar *menuBar = new QMenuBar();
    menuBar->addMenu(tr("&File"));
    menuBar->addMenu(tr("&Edit"));
    menuBar->setStyleSheet("QMenuBar { background-color: rgb(23,23,23); border-bottom: 1px solid rgb(18,18,18); }");
    mainLayout->addWidget(menuBar);

    // Create bottom widget
    QWidget *bottomWidget = new QWidget();
    bottomWidget->setFixedHeight(40);
    bottomWidget->setStyleSheet("background-color: rgb(23,23,23); border-bottom: 1px solid rgb(18,18,18);");
    mainLayout->addWidget(bottomWidget);

    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->setSpacing(0);

    notes = {
        {"𝅝", NoteType::Semibreve},
        {"𝅗𝅥", NoteType::Minim},
        {"𝅘𝅥", NoteType::Crotchet},
        {"𝅘𝅥𝅮", NoteType::Quaver},
        {"𝅘𝅥𝅯", NoteType::Quaver},
        {"𝅘𝅥𝅰", NoteType::Quaver},
        {"𝅘𝅥𝅱", NoteType::Quaver}
    };
    accidentals = {
        {"♭", AccidentalType::Flat},
        {"♮", AccidentalType::Natural},
        {"♯", AccidentalType::Sharp}
    };

    // Add accidental buttons
    for (QMap<QString, AccidentalType>::iterator it = accidentals.begin(); it != accidentals.end(); ++it)
    {
        QPushButton *signButton = createButton(it.key(), 20);
        connect(signButton, &QPushButton::clicked, this, &MenuBar::clickAccidental);
        bottomLayout->addWidget(signButton);
    }
    // Add divider
    QHBoxLayout *divider = createDivider();
    bottomLayout->addLayout(divider);

    // Add note buttons
    for (QMap<QString, NoteType>::iterator it = notes.begin(); it != notes.end(); ++it)
    {
        QPushButton *noteButton = createButton(it.key(), 30);
        connect(noteButton, &QPushButton::clicked, this, &MenuBar::clickNote);
        bottomLayout->addWidget(noteButton);

        if (it.value() == NoteType::Crotchet)
        {
            noteButton->setChecked(true);
            selectedNote = qMakePair(NoteType::Crotchet, noteButton);
        }
    }
}

// Getter for selected note
NoteType MenuBar::getSelectedNote()
{
    return selectedNote.first;
}

// Getter for selected accidental
AccidentalType MenuBar::getSelectedAccidental()
{
    return selectedAccidental.first;
}

// Creates a menu bar button
QPushButton *MenuBar::createButton(QString text, int fontSize)
{
    QPushButton *button = new QPushButton(text);
    button->setCheckable(true);
    button->setFixedWidth(40);
    button->setCursor(Qt::PointingHandCursor);
    button->setStyleSheet(QString(
        "QPushButton { border: none; font-size: %1px; }"
        "QPushButton:hover { color: rgb(70, 129, 232); }"
        "QPushButton:checked { color: rgb(70, 129, 232); }"
    ).arg(fontSize));

    return button;
}

// Creates a menu bar divider
QHBoxLayout *MenuBar::createDivider()
{
    QHBoxLayout *dividerLayout = new QHBoxLayout();
    QSpacerItem *spacer1 = new QSpacerItem(15, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    QSpacerItem *spacer2 = new QSpacerItem(15, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);

    QWidget *divider = new QWidget();
    divider->setFixedSize(1, 25);
    divider->setStyleSheet("background-color: gray;");

    dividerLayout->addSpacerItem(spacer1);
    dividerLayout->addWidget(divider);
    dividerLayout->addSpacerItem(spacer2);

    return dividerLayout;
}

// Selects the corresponding note when button is clicked
void MenuBar::clickNote()
{
    QPushButton *senderButton = qobject_cast<QPushButton*>(sender());
    QPushButton *selectedButton = selectedNote.second;
    if (selectedButton == senderButton)
    {
        senderButton->setChecked(true);
        return;
    }
    selectedNote.second->setChecked(false);
    selectedNote = qMakePair(notes[senderButton->text()], senderButton);
}

// Selects the corresponding accidental when button is clicked
void MenuBar::clickAccidental()
{
    QPushButton *senderButton = qobject_cast<QPushButton*>(sender());
    QPushButton *selectedButton = selectedAccidental.second;

    if (selectedButton != nullptr)
    {
        if (selectedButton == senderButton)
        {
            selectedAccidental = qMakePair(AccidentalType::None, nullptr);
            return;
        }
        selectedButton->setChecked(false);
    }
    selectedAccidental = qMakePair(accidentals[senderButton->text()], senderButton);
    senderButton->setChecked(true);
}

