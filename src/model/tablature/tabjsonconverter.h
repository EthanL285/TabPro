#ifndef TABJSONCONVERTER_H
#define TABJSONCONVERTER_H

#include "rhythmsymbol.h"
#include "tabcolumn.h"

#include <QObject>

class TabJsonConverter : public QObject
{
    Q_OBJECT
public:
    explicit TabJsonConverter(QObject *parent = nullptr);
    static QJsonObject convertTabtoJson(const QVector<RhythmSymbol*> &staff, const QVector<TabColumn*> &tab);

signals:
};

#endif // TABJSONCONVERTER_H
