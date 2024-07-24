#ifndef NOTE_H
#define NOTE_H

#include <QWidget>

class Note : public QWidget
{
    Q_OBJECT
public:
    explicit Note(QWidget *parent = nullptr);
    void updateLineLength(bool add);

private:
    int length = 95;

signals:
};

#endif // NOTE_H
