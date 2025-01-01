#include "notefactory.h"
#include "crotchet.h"
#include "quaver.h"

NoteFactory::NoteFactory(QWidget *parent)
    : QWidget{parent}
{}

// Create note given a note type
Note *NoteFactory::createNote(NoteType type, QVector<int> staffLines)
{
    switch (type)
    {
        case NoteType::Semibreve:
            return new Crotchet(staffLines);
        case NoteType::Minim:
            return new Crotchet(staffLines);
        case NoteType::Crotchet:
            return new Crotchet(staffLines);
        case NoteType::Quaver:
            return new Quaver(staffLines);
        case NoteType::Semiquaver:
            return new Quaver(staffLines);
        default:
            qCritical() << "Invalid note type passed to NoteFactory";
            return nullptr;
    }
}
