#ifndef UIWIDGETS_H
#define UIWIDGETS_H

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

class TextField : public QWidget
{
    Q_OBJECT
public:
    explicit TextField(const QString &text, const QString &imagePath, QWidget *parent = nullptr);

public slots:
    void toggleVisibility();
    void setCheckBoxStylesheet(QCheckBox *checkbox);

private:
    QWidget *parentWidget;
    QLineEdit *passwordField;
    bool visibility = false;

};

class MainButton: public QPushButton
{
    Q_OBJECT
public:
    explicit MainButton(const QString &text, QWidget *parent = nullptr);
};

#endif // UIWIDGETS_H
