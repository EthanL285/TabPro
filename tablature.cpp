#include "tablature.h"

Tablature::Tablature(QWidget *parent)
    : QLabel{parent}
{
    // Create Tablature
    setStyleSheet("color: white; font: 20pt Consolas;");
    setText
    (
        "E|------------------------|\n"
        "B|------------------------|\n"
        "G|------------------------|\n"
        "D|------------------------|\n"
        "A|------------------------|\n"
        "E|------------------------|\n"
    );
}
