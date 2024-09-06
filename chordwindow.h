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
    explicit ChordWindow(QWidget *parent = nullptr);
    void toggleContent();
    void animateAccordion(QWidget *widget);
    void toggleMode();
    void resetSwitch(ToggleSwitch *widget, bool &mode);
    void addStatusMessage(Status status);
    void removeStatusMessage();

    Mode getModeFromName(QString name);
    QLineEdit *createField(QString text, bool dark, int width = 0);
    QLabel *createLabel(QString text);

private slots:
    void addChordToList();
    void addChordToStaff();
    void changeWindow();

private:
    int chordCount = 0;
    bool contentToggled = false;
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

    QLabel *statusMessage = nullptr;
    QSpacerItem *spacer = nullptr;
};

#endif // CHORDWINDOW_H
