#ifndef CHORDS_H
#define CHORDS_H

#include <QWidget>
#include <QPushButton>

class Chords : public QWidget
{
    Q_OBJECT
public:
    explicit Chords(QWidget *parent = nullptr);
    void toggleContent();
    void animateAccordion(QWidget *widget);

private:
    QWidget *content;
    QWidget *header;
    QPushButton *button;
    bool contentToggled = false;
    QIcon expandIcon;
    QIcon collapseIcon;
};

//////////////////// Toggle Switch Class ////////////////////

class ToggleSwitch : public QWidget {
    Q_OBJECT

public:
    ToggleSwitch(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QWidget *handle;
    bool toggled = false;

private slots:
    void animateHandle();
};


#endif // CHORDS_H
