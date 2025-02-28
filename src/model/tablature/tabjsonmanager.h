#ifndef TABJSONMANAGER_H
#define TABJSONMANAGER_H

#include <QObject>

class TabJsonManager : public QObject
{
    Q_OBJECT
public:
    explicit TabJsonManager(QObject *parent = nullptr) = delete;
    static void createTab(const QJsonObject &json);
    static QJsonObject tabToJson();

signals:
};

#endif // TABJSONMANAGER_H
