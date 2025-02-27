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
    void updateWidth(int noteWidth);

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
    void widthChanged(int newWidth, int prevWidth);
};

#endif // TABLATUREBUTTON_H
