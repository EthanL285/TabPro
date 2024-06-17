#include "sidebar.h"
#include <QLabel>

Sidebar::Sidebar(QWidget *parent)
    : QWidget{parent}
{
    // Create sidebar
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setAlignment(Qt::AlignLeft);

    sidebar = new QWidget();
    sidebar->setMinimumWidth(60);
    sidebar->setStyleSheet("background-color: rgb(23,23,23);");
    sidebar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    mainLayout->addWidget(sidebar);

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setSpacing(20);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);

    // Add sidebar checkbox
    QCheckBox *sidebarIcon = new QCheckBox();
    sidebarIcon->setFixedSize(40, 40);
    sidebarIcon->setCursor(Qt::ArrowCursor);
    sidebarIcon->setCursor(Qt::PointingHandCursor);
    sidebarIcon->setFocusPolicy(Qt::NoFocus);

    sidebarIcon->setStyleSheet
        (
            "QCheckBox {"
            "    background: transparent;"
            "    border-radius: 5px;"
            "}"

            "QCheckBox::indicator {"
            "    border: none;"
            "    background: none;"
            "    image: url(:/Sidebar/Icons/Sidebar/SideBarIcon.png);"
            "    width: 40px;"
            "    height: 40px;"
            "}"

            "QCheckBox:hover {"
            "    background: rgb(45,45,45);"
            "}"
            );
    connect(sidebarIcon, &QCheckBox::stateChanged, this, &Sidebar::toggleSidebar);

    sidebarLayout->addWidget(sidebarIcon);
    sidebarLayout->setContentsMargins(10, 10, 0, 0);
    sidebarLayout->setAlignment(sidebarIcon, Qt::AlignLeft | Qt::AlignTop);
}

// Toggles the sidebar
void Sidebar::toggleSidebar()
{
    // Animation is still running
    if (sidebarAnimation != nullptr && sidebarAnimation->state() == QAbstractAnimation::Running)
    {
        return;
    }
    // Create new animation object
    if (sidebarAnimation == nullptr)
    {
        sidebarAnimation = new QPropertyAnimation(sidebar, "minimumWidth");
        sidebarAnimation->setDuration(150);
        sidebarAnimation->setStartValue(60);
        sidebarAnimation->setEndValue(350);
        sidebarAnimation->setEasingCurve(QEasingCurve::Linear);
    }
    // Expand sidebar
    if (sidebar->minimumWidth() == 60)
    {
        sidebarAnimation->setDirection(QAbstractAnimation::Forward);
    }
    // Collapse sidebar
    else
    {
        sidebarAnimation->setDirection(QAbstractAnimation::Backward);
    }

    sidebarAnimation->start();
}

