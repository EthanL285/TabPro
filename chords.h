#ifndef CHORDS_H
#define CHORDS_H

#include <QWidget>
#include <QPushButton>

class Chords : public QWidget
{
    Q_OBJECT
public:
    explicit Chords(QWidget *parent = nullptr);
    void toggleContent(bool checked);

private:
    QWidget *content;
    QPushButton *button;
    bool contentVisible = false;

signals:
};

#endif // CHORDS_H
