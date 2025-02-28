#ifndef SCOREUPDATER_H
#define SCOREUPDATER_H

#include <QWidget>
#include <QVector>
#include <QBoxLayout>

class ScoreUpdater : public QWidget
{
    Q_OBJECT
public:
    explicit ScoreUpdater(QWidget *parent = nullptr);
    static void updateBarLines();

private:
    static bool isUpdating;
    static int removeBarLines(QHBoxLayout *layout);
    static int addBarLines(QHBoxLayout *layout, QVector<int> barLinePos, int layoutOffset, int barHeight);
    static QVector<int> getBarLinePos();

signals:
};

#endif // SCOREUPDATER_H
