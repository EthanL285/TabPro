#ifndef TABLATURE_H
#define TABLATURE_H

#include <QWidget>
#include <QLabel>

class Tablature : public QLabel
{
    Q_OBJECT
public:
    explicit Tablature(QWidget *parent = nullptr);

signals:
};

#endif // TABLATURE_H
