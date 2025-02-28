#include "tabjsonmanager.h"
#include "rhythmsymbol.h"
#include "tabcolumn.h"
#include "staff.h"
#include "tablature.h"

#include <QJsonArray>
#include <QJsonObject>

// Converts the JSON to tab
void TabJsonManager::createTab(const QJsonObject &json)
{
    QJsonArray notes = json["notes"].toArray();
    for (const auto &note : notes)
    {

    }
}

// Converts the tab to JSON
QJsonObject TabJsonManager::tabToJson()
{
    QVector<RhythmSymbol*> staff = Staff::getInstance()->getNotes();
    QVector<TabColumn*> tab = Tablature::getInstance()->getTab();

    if (staff.size() != tab.size())
        qFatal("Mismatch in staff and tab sizes. Staff size: %lld, Tab size: %lld", staff.size(), tab.size());

    QJsonArray notes;
    for (int i = 0; i < tab.size(); i++)
    {
        QJsonObject note;
        note["type"] = staff[i]->toJson();
        note["frets"] = tab[i]->toJson();
        notes.append(note);
    }
    return QJsonObject{{"notes", notes}};
}
