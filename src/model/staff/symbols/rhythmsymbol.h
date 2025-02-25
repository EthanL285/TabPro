#ifndef RYTHMSYMBOL_H
#define RYTHMSYMBOL_H

#include <QWidget>

class RhythmSymbol : public QWidget
{
    Q_OBJECT
public:
    virtual double getBeatValue() = 0;
    virtual void toggleSelect() = 0;
    virtual QJsonObject toJson() const = 0;

public slots:
    void onWidthChange(int newWidth, int prevWidth);

signals:
    void widthChanged(int newWidth, int prevWidth);
};

#endif // RYTHMSYMBOL_H
