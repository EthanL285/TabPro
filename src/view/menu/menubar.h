#ifndef MENUBAR_H
#define MENUBAR_H

#include "notetype.h"
#include "accidentaltype.h"

#include <QWidget>
#include <QMenuBar>
#include <QPushButton>
#include <QHBoxLayout>

class MenuBar : public QWidget
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = nullptr);
    NoteType getSelectedNote();
    AccidentalType getSelectedAccidental();
    static int getTimeSignature();

    // Testing Functions
    QPushButton *getNoteButton(NoteType note);

private:
    QPushButton *createButton(QString text, int fontSize);
    QHBoxLayout *createDivider();

    QWidget *bottomWidget;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QPair<NoteType, QPushButton*> selectedNote;
    QPair<AccidentalType, QPushButton*> selectedAccidental;
    QMap<QString, NoteType> notes;
    QMap<QString, AccidentalType> accidentals;

private slots:
    void clickNote();
    void clickAccidental();

signals:
};

#endif // MENUBAR_H
