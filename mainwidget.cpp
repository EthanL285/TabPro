#include "mainwidget.h"
#include <QPainter>

MainWidget::MainWidget(MainWindow *parent)
    : QWidget{parent}, mainWindow(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create sidebar
    sidebar = new QWidget();
    sidebar->setMinimumWidth(60);
    sidebar->setStyleSheet("background-color: rgb(23,23,23);");
    sidebar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setSpacing(20);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(sidebar);

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
    connect(sidebarIcon, &QCheckBox::stateChanged, this, &MainWidget::toggleSidebar);

    sidebarLayout->addWidget(sidebarIcon);
    sidebarLayout->setContentsMargins(10, 10, 0, 0);
    sidebarLayout->setAlignment(sidebarIcon, Qt::AlignLeft | Qt::AlignTop);
}

// Toggles the sidebar
void MainWidget::toggleSidebar()
{
    // Animation is still running
    if (sidebarAnimation && sidebarAnimation->state() == QAbstractAnimation::Running)
    {
        return;
    }
    // Create new animation object
    if (!sidebarAnimation)
    {
        sidebarAnimation = new QPropertyAnimation(sidebar, "minimumWidth");
        sidebarAnimation->setDuration(200);
        sidebarAnimation->setStartValue(60);
        sidebarAnimation->setEndValue(350);
        sidebarAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    }
    // Collapse sidebar
    if (sidebar->minimumWidth() == 60)
    {
        sidebarAnimation->setDirection(QAbstractAnimation::Forward);
    }
    // Expand sidebar
    else
    {
        sidebarAnimation->setDirection(QAbstractAnimation::Backward);
    }

    sidebarAnimation->start();
}

void MainWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // Set the background color
    painter.setRenderHint(QPainter::Antialiasing);
    QColor color(33,33,33);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);

    // Draw the background
    painter.drawRect(rect());
}
