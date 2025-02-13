#ifndef MENUBAR_H
#define MENUBAR_H

#include "notetype.h"
#include "accidentaltype.h"

#include <QWidget>
#include <QMenuBar>
#include <QPushButton>
#include <QHBoxLayout>

#define COMMON_TIME "\uE09E\uE084\uE09F\uE084"

class MenuBar : public QWidget
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = nullptr);
    NoteType getNote();
    AccidentalType getAccidental();
    QString getSignature();

    // Testing Functions
    QPushButton *getMenuButton(NoteType note);
    QPushButton *getMenuButton(QString buttonName);

private:
    QPushButton *createButton(QString text, int fontSize);
    QHBoxLayout *createDivider();

    QWidget *menuWidget;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QPushButton *timeSignature;
    QString selectedSignature = COMMON_TIME;
    QPair<NoteType, QPushButton*> selectedNote;
    QPair<AccidentalType, QPushButton*> selectedAccidental;
    QMap<QString, NoteType> notes;
    QMap<QString, AccidentalType> accidentals;

public slots:
    void onNoteClick();
    void onAccidentalClick();
    void onResetTabClick();
    void onTimeSignatureClick();

signals:
    void signatureChanged(int beatsPerMeasure, int beatUnit);
    void resetTab();
};

#endif // MENUBAR_H
