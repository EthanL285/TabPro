#include "tabjsonconverter.h"

#include <QJsonArray>
#include <QJsonObject>

TabJsonConverter::TabJsonConverter(QObject *parent)
    : QObject{parent}
{}

// Converts the tablature to JSON
QJsonObject TabJsonConverter::convertTabtoJson(const QVector<RhythmSymbol*> &staff, const QVector<TabColumn*> &tab)
{
    if (staff.size() != tab.size())
        qFatal("Mismatch in staff and tab sizes. Staff size: %lld, Tab size: %lld", staff.size(), tab.size());

    QJsonArray notes;
    for (int i = 0; i < tab.size(); i++)
    {
        QJsonObject note;
        note["type"] = staff[i]->toJson();
        // note["frets"] = tab[i]->toJson();
        notes.append(note);
    }
    return QJsonObject{{"notes", notes}};
}
