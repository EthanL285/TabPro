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
    mainLayout->setSpacing(50);

    // Add sidebar
    sidebar = new Sidebar();
    mainLayout->addWidget(sidebar);

    // Add guitar widget
    guitar = new Guitar();
    mainLayout->addWidget(guitar, Qt::AlignHCenter);
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
