#include "registerui.h"
#include "loginui.h"

RegisterUI::RegisterUI(MainWindow *parent) : mainWindow(parent)
{
    // Add rectangle layout
    QVBoxLayout *rectangleLayout = new QVBoxLayout(this);
    rectangleLayout->setAlignment(Qt::AlignHCenter);
    rectangleLayout->setSpacing(20);
    rectangleLayout->setContentsMargins(0, 70, 0, 50);

    // Add register box
    RectangleWidget *registerBox = new RectangleWidget(this);
    rectangleLayout->addWidget(registerBox);

    // Add widget layout
    QHBoxLayout *widgetLayout = new QHBoxLayout(registerBox);
    widgetLayout ->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    widgetLayout ->setContentsMargins(40, 50, 40, 70);
    widgetLayout ->setSpacing(30);

    // Add title
    QLabel *title = new QLabel("Create Account");
    title->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    title->setStyleSheet("color: white; font: 30pt Moon;");
    widgetLayout->addWidget(title);
}
