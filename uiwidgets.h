#ifndef UIWIDGETS_H
#define UIWIDGETS_H

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>

class TextField : public QWidget
{
    Q_OBJECT
public:
    explicit TextField(const QString &text, const QString &imagePath, QWidget *parent = nullptr);

public slots:
    void toggleVisibility();


private:
    QWidget *parentWidget;
    QLineEdit *passwordField;
    bool visibility = false;
};

#endif // UIWIDGETS_H
