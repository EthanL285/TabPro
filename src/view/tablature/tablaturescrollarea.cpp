#include "tablaturescrollarea.h"

#include <QScrollBar>

TablatureScrollArea::TablatureScrollArea(Tablature *tab, QWidget *parent)
    : QScrollArea{parent}
{
    setWidgetResizable(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setStyleSheet
    (
        "QScrollBar:horizontal {"                       // Horizontal scrollbar
        "    border: 1px solid #222222;"
        "    background: #333333;"
        "    height: 15px;"
        "    margin: 0 22px 0 22px;"
        "}"
        "QScrollBar::handle:horizontal {"               // Scrollbar handle
        "    background: rgb(80,80,80);"
        "    min-width: 20px;"
        "    border: 1px solid rgb(70,70,70);"
        "    border-right: 1px solid rgb(40,40,40);"
        "    border-left: 1px solid rgb(40,40,40);"
        "    border-radius: 1px;"
        "}"
        "QScrollBar::handle:horizontal:hover {"
        "    background: rgb(100,100,100);"
        "}"
        "QScrollBar::handle:horizontal:pressed {"
        "    background: rgb(85,85,85);"
        "}"
        "QScrollBar::add-line:horizontal {"             // Buttons at the end of the scrollbar
        "    border-right: 1px solid rgb(15,15,15);"
        "    border-bottom: 1px solid rgb(15,15,15);"
        "    background: rgb(25,25,25);"
        "    width: 20px;"
        "    subcontrol-position: right;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::sub-line:horizontal {"
        "    border-left: 1px solid rgb(15,15,15);"
        "    border-bottom: 1px solid rgb(15,15,15);"
        "    background: rgb(25,25,25);"
        "    width: 20px;"
        "    subcontrol-position: left;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::add-line:horizontal:hover, QScrollBar::sub-line:horizontal:hover {"
        "    background: rgb(80,80,80);"
        "}"
        "QScrollBar::add-line:horizontal:pressed, QScrollBar::sub-line:horizontal:pressed {"
        "    background: rgb(40,40,40);"
        "}"
        "QScrollBar::left-arrow:horizontal {"                                   // Arrows of the end buttons
        "    width: 8px;"
        "    height: 8px;"
        "    image: url(:/scroll/scroll/scroll left.png);"
        "}"
        "QScrollBar::right-arrow:horizontal {"
        "    width: 8px;"
        "    height: 8px;"
        "    image: url(:/scroll/scroll/scroll right.png);"
        "}"
        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {"    // Regions to the left and right of the handle
        "    background: none;"
        "}"
    );
    // Connect scroll bar signals to slots
    QScrollBar *hScrollBar = horizontalScrollBar();
    hScrollBar->setSingleStep(Tablature::DEFAULT_BUTTON_WIDTH);
    connect(hScrollBar, &QScrollBar::rangeChanged, [hScrollBar](int max) { if (max > 0) hScrollBar->setValue(max); });
    connect(hScrollBar, &QScrollBar::sliderPressed, tab, &Tablature::pauseTab);
}
