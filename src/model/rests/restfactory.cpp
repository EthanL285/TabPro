#include "restfactory.h"
#include "eighthrest.h"
#include "quarterrest.h"
#include "halfrest.h"

RestFactory::RestFactory(QWidget *parent)
    : QWidget{parent}
{}

// Create rest given a beat value
Rest *RestFactory::createRest(double beatValue)
{
    int value = beatValue * 2;
    switch (value)
    {
        case 1:
            return new EighthRest();  // 0.5 beat value
        case 2:
            return new QuarterRest(); // 1 beat value
        case 4:
            return new HalfRest();    // 2 beat value
        default:
            return nullptr;
    }
}

// Create rest given a note type
Rest *RestFactory::createRest(NoteType type)
{
    switch (type)
    {
        case NoteType::Semibreve:
            return new QuarterRest();
        case NoteType::Minim:
            return new QuarterRest();
        case NoteType::Crotchet:
            return new QuarterRest();
        case NoteType::Quaver:
            return new EighthRest();
        case NoteType::Semiquaver:
            return new EighthRest();
        default:
            return nullptr;
    }
}
