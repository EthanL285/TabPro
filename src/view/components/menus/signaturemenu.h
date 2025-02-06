#ifndef SIGNATUREMENU_H
#define SIGNATUREMENU_H

#include "menubar.h"
#include "spinbox.h"
#include "signaturewidget.h"

#include <QObject>
#include <QMenu>

class SignatureMenu : public QMenu
{
    Q_OBJECT
public:
    explicit SignatureMenu(MenuBar *menubar, QPushButton *menuButton, QWidget *parent = nullptr);
    QMenu *createCustomSignatureMenu();

private:
    MenuBar *menubar;
    QPushButton *menuButton;

    QMenu *customMenu;
    SignatureWidget *signatureDisplay;
    SpinBox *topSpinBox;
    SpinBox *bottomSpinBox;

private slots:
    void openMenu();
    void openCustomMenu();
    void onSignatureChange();
    void onSpinBoxChange(int value);
    void onConfirmClick();

signals:
    void signatureChanged(int topDigit, int bottomDigit);
};

#endif // SIGNATUREMENU_H
