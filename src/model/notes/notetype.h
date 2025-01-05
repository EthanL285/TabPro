#ifndef NOTETYPE_H
#define NOTETYPE_H

#include <QString>

enum class NoteType {
    Semibreve,
    Minim,
    Crotchet,
    Quaver,
    Semiquaver
};

inline NoteType stringToNoteType(QString noteString)
{
    if (noteString == "SB")
    {
        return NoteType::Semibreve;
    }
    else if (noteString == "M")
    {
        return NoteType::Minim;
    }
    else if (noteString == "C")
    {
        return NoteType::Crotchet;
    }
    else if (noteString == "Q")
    {
        return NoteType::Quaver;
    }
    else if (noteString == "SQ")
    {
        return NoteType::Semiquaver;
    }
    else
    {
        throw std::invalid_argument("Invalid note string");
    }
}

#endif // NOTETYPE_H

