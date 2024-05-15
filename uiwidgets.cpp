#include "uiwidgets.h"

TextField::TextField(const QString &text, const QString &imagePath, QWidget *parent) : QWidget(parent)
{
    // Add text field
    QHBoxLayout *layout = new QHBoxLayout(this);
    QLineEdit *field = new QLineEdit(this);
    field->setObjectName("field");     // Set object name for easy retrieval of text using 'findchild'
    field->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    field->setPlaceholderText(text);
    field->setMinimumSize(0, 50);
    field->setStyleSheet
    (
            "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(96, 94, 92, 100), stop: 1 rgba(32, 31, 30, 200));"
            "border: none;"
            "color: white;"
            "font: 12pt Muli;"
            "padding-left: 6px;"
    );

    // Add image
    QPixmap icon(imagePath);
    QSize iconSize(50, 50);
    icon = icon.scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QLabel *imageLabel = new QLabel();
    imageLabel->setObjectName("icon");
    imageLabel->setStyleSheet("background-color: rgba(0, 0, 0, 100);");
    imageLabel->setPixmap(icon);
    imageLabel->setFixedSize(50, 50);

    // Configure layout
    layout->addWidget(imageLabel);
    layout->addWidget(field);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Add visibility feature if password
    if (text == "Password")
    {
        passwordField = field;
        field->setEchoMode(QLineEdit::Password);

        QHBoxLayout *visibilityLayout = new QHBoxLayout(field);
        QCheckBox *visibility = new QCheckBox();
        visibility->setObjectName("visibility");
        visibility->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        visibility->setCursor(Qt::ArrowCursor);
        visibility->setCursor(Qt::PointingHandCursor);
        setCheckBoxStylesheet(visibility);
        visibilityLayout->addWidget(visibility);
        visibilityLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        connect(visibility, &QCheckBox::stateChanged, this, &TextField::toggleVisibility);
    }
}

void TextField::setCheckBoxStylesheet(QCheckBox *checkbox)
{
    checkbox->setStyleSheet
    (
        "QCheckBox { background: transparent; }"

        "QCheckBox:indicator:unchecked {"
        "border: none;"
        "background: none;"
        "image: url(:/FieldIcon/VisibilityUnchecked.png);"
        "width: 35px;"
        "height: 35px;"
        "}"
        "QCheckBox:indicator:checked {"
        "border: none;"
        "background: none;"
        "image: url(:/FieldIcon/VisibilityChecked.png);"
        "width: 35px;"
        "height: 35px;"
        "}"
    );
}

void TextField::toggleVisibility()
{
    if (visibility)
    {
        passwordField->setEchoMode(QLineEdit::Password);
        visibility = false;
    }
    else
    {
        passwordField->setEchoMode(QLineEdit::Normal);
        visibility = true;
    }
}

MainButton::MainButton(const QString &text, QWidget *parent) : QPushButton(parent)
{
    // Add main button
    setText(text);
    setMinimumSize(0, 50);
    setStyleSheet
    (
        "QPushButton {"
        "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(255, 255, 255, 200), stop: 1 rgba(96, 94, 92, 200));"
        "border: none;"
        "color: black;"
        "font: 15pt Moon;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(255, 255, 255, 200), stop: 0.2 rgba(243, 242, 241, 200));"
        "}"
        "QPushButton:pressed {"
        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba(32, 31, 30, 200), stop: 0.3 rgba(59, 58, 57, 200));" // Vertical gradient
        "color: white;"
        "font: 15pt Moon;"
        "}"
    );
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setCursor(Qt::PointingHandCursor);
    setFocusPolicy(Qt::NoFocus);
}
