#include "filemenu.h"

FileMenu::FileMenu(QWidget *parent)
    : QMenu{tr("&File"), parent}
{
    QAction *newAction = addAction(tr("&New Tablature"));
    QAction *openAction = addAction(tr("&Open Tablature") + QString(10, QChar(' ')));
    addSeparator();
    QAction *saveAction = addAction(tr("&Save"));
    QAction *saveAsAction = addAction(tr("Save &As"));

    newAction->setShortcut(QKeySequence("Ctrl+N"));
    openAction->setShortcut(QKeySequence("Ctrl+O"));
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    saveAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));

    newAction->setIcon(QIcon(":/menu/menu/new.png"));
    openAction->setIcon(QIcon(":/menu/menu/open.png"));
    saveAction->setIcon(QIcon(":/menu/menu/save.png"));
    saveAsAction->setIcon(QIcon(":/menu/menu/save as.png"));

    setStyleSheet
    (
        "QMenu { background-color: rgb(23,23,23); border: 1px solid rgb(18,18,18); }"
        "QMenu::item { color: white; padding: 5px 10px; }"
        "QMenu::item:selected { background-color: rgb(40,40,40); }"
        "QMenu::icon { padding-left: 12px; }"
        "QMenu::separator { background-color: rgb(18,18,18); height: 2px; margin: 5px 5px; }"
    );
}
