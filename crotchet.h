#ifndef CROTCHET_H
#define CROTCHET_H

#include "note.h"

#include <QWidget>

// ========== Crotchet Class ==========
class Crotchet : public Note
{
    Q_OBJECT
public:
    explicit Crotchet(QVector<int> staffLines);
};

#endif // CROTCHET_H
