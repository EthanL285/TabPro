#ifndef CHORDWINDOW_H
#define CHORDWINDOW_H

#include "chorddiagram.h"
#include "toggleswitch.h"

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

class ChordWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChordWindow(QWidget *parent = nullptr);
    void toggleContent();
    void animateAccordion(QWidget *widget);
    void toggleMode();
    void resetSwitch(ToggleSwitch *widget, bool &mode);
    Mode getModeFromName(QString name);

    QLineEdit *createField(QString text, bool dark, int width = 0);
    QLabel *createLabel(QString text);

private slots:
    void addChord();
    void changeWindow();

private:
    ChordDiagram *diagram;

    QWidget *content;
    QWidget *header;
    QLineEdit *searchField;
    QPushButton *accordionToggle;
    QPushButton *trash;
    QPushButton *back;
    QLabel *barPlacement;
    QComboBox *barDropdown;
    bool contentToggled = false;
    QIcon expandIcon;
    QIcon collapseIcon;
    QScrollArea *scrollArea;
    QStackedWidget *stackedWidget;
    QWidget *chordWindow = nullptr;
    ToggleSwitch *placeSwitch;
    ToggleSwitch *dragSwitch;
    ToggleSwitch *deleteSwitch;
};

#endif // CHORDWINDOW_H
