#ifndef CHORDWINDOW_H
#define CHORDWINDOW_H

#include "chorddiagram.h"
#include "chorddisplay.h"
#include "toggleswitch.h"
#include "guitar.h"

#include <QWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QHash>

enum Mode {
    Place,
    Drag,
    Delete,
    None
};

enum Status {
    Success,
    Duplicate,
    Empty,
    CharLimit
};

class ChordWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChordWindow(Guitar *parent = nullptr);
    void toggleContent();
    void animateAccordion(QWidget *widget);
    void toggleMode();
    void resetSwitch(ToggleSwitch *widget, bool &mode);
    void addDefaultChords();
    void addStatusMessage(Status status);
    void removeStatusMessage();
    void updateLayout(QVector<QPushButton*> hidden = QVector<QPushButton*>());

    Mode getModeFromName(QString name);
    QLineEdit *createField(QString text, bool dark, int width = 0);
    QLabel *createLabel(QString text);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void addChordToList();
    void addChordToStaff();
    void changeWindow();
    void deleteChord();
    void searchChord(QString input);

public slots:
    void updateScrollArea();

private:
    Guitar *guitar;
    ChordDiagram *diagram;
    QWidget *content;
    QWidget *header;
    QGridLayout *listLayout;
    QGridLayout *buttonLayout;
    QLineEdit *nameField;
    QLineEdit *searchField;
    QPushButton *accordionToggle;
    QPushButton *trash;
    QPushButton *back;
    QPushButton *newChord;
    QLabel *barPlacement;
    QComboBox *barDropdown;
    QIcon expandIcon;
    QIcon collapseIcon;
    QScrollArea *scrollArea;
    QStackedWidget *stackedWidget;
    QWidget *chordWindow = nullptr;
    ToggleSwitch *placeSwitch;
    ToggleSwitch *dragSwitch;
    ToggleSwitch *deleteSwitch;
    QHash<QString, QVector<int>> chords;
    QVector<ChordDisplay*> chordDisplays;
    QLabel *statusMessage = nullptr;
    QSpacerItem *spacer = nullptr;
    bool contentToggled = false;
};

#endif // CHORDWINDOW_H
