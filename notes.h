#ifndef NOTES_H
#define NOTES_H

#include <QWidget>

// ========== Blank Class ==========
class Blank : public QWidget
{
    Q_OBJECT
public:
    explicit Blank(QWidget *parent = nullptr);
};


// ========== Crotchet Class ==========
class Crotchet : public QWidget
{
    Q_OBJECT
public:
    explicit Crotchet(int staffLine, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int staffLine;
    bool flip;
    bool addLines;
};

#endif // NOTES_H
