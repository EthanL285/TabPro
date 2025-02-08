#ifndef TABLATUREBUTTON_H
#define TABLATUREBUTTON_H

#include <QWidget>
#include <QPushButton>

class TablatureButton : public QPushButton
{
    Q_OBJECT
public:
    explicit TablatureButton(QWidget *parent = nullptr);
    void toggleSelect();
    void updateText(QString text);

private:
    QStringList fretNumbers;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    bool isHovered = false;
    bool isSelected = false;

signals:
};

#endif // TABLATUREBUTTON_H
