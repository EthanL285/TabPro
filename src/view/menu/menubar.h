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

    // Testing Functions
    QPushButton *getMenuButton(NoteType note);
    QPushButton *getMenuButton(QString buttonName);

private:
    QPushButton *createButton(QString text, int fontSize);
    QHBoxLayout *createDivider();

    QWidget *menuWidget;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QPair<NoteType, QPushButton*> selectedNote;
    QPair<AccidentalType, QPushButton*> selectedAccidental;
    QMap<QString, NoteType> notes;
    QMap<QString, AccidentalType> accidentals;

private slots:
    void onNoteClick();
    void onAccidentalClick();
    void onResetTabClick();
    void onTimeSignatureClick();

signals:
    void timeSignatureChanged(int beatsPerMeasure, int beatUnit);
    void resetTab();
};

#endif // MENUBAR_H
