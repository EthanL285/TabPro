#ifndef LOGINUI_H
#define LOGINUI_H

#include "mainwindow.h"
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

// loginUI Class
class loginUI : public QWidget
{
    Q_OBJECT
public:
    explicit loginUI(MainWindow *parent = nullptr);

protected:
    MainWindow *mainWindow;

private slots:
    void loginSlot();
    void rememberSlot();
    void toggleVisibility();

private:
    QLineEdit *passwordField;
    bool visibility = false;
};

// RectangleWidget Class
class RectangleWidget : public QWidget
{
public:
    RectangleWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    QSize sizeHint() const override;

private:
    const double aspectRatio = 2.0 / 3.0;
    QLabel *iconLabel;
};

// ResizableImageLabel Class
class ResizableImageLabel : public QLabel {
public:
    explicit ResizableImageLabel(const QPixmap image, QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    const QPixmap image;
};


// ClickableLabel Class
class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(const QString &text, QWidget *parent = nullptr);

public slots:
    void forgotSlot();
    void registerAccount();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

#endif // LOGINUI_H
