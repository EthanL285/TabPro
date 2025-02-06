#include "signaturewidget.h"

#include <QVBoxLayout>

static const QPoint TOP_DIGIT_POS = QPoint(-40, -60);
static const QPoint BOTTOM_DIGIT_POS = QPoint(-40, -34);

SignatureWidget::SignatureWidget(QString text, QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(40, 60);
    setStyleSheet("border: 1px solid green;");

    topDigit = createDigit(text.at(1));
    bottomDigit = createDigit(text.at(3));

    topDigit->move(TOP_DIGIT_POS);
    bottomDigit->move(BOTTOM_DIGIT_POS);
}

QLabel *SignatureWidget::createDigit(QString text)
{
    QLabel *digit = new QLabel(text, this);
    digit->setAlignment(Qt::AlignCenter);
    digit->setFont(QFont("Bravura Text", 50));
    digit->setFixedSize(100, 100);
    digit->setStyleSheet
    (
        "QLabel {"
        "   border: none;"
        "   outline: none;"
        "}"
    );
    return digit;
}

// Getter for top digit value
QString SignatureWidget::getTopDigit()
{
    return topDigit->text();
}

// Getter for bottom digit value
QString SignatureWidget::getBottomDigit()
{
    return bottomDigit->text();
}

// Changes the time signature
void SignatureWidget::changeSignature(QString text)
{
    // Map beats to their respective horizontal offsets
    std::unordered_map<int, int> offsets = { {9, 2}, {8, 2}, {7, 2}, {6, 2}, {5, 3}, {3, 2}, {2, 2}, {1, 4} };

    QChar base(0xE080);
    int beatsPerMeasure = text.at(1).unicode() - base.unicode();
    int beatUnit = text.at(3).unicode() - base.unicode();

    // Update positions
    if (offsets.count(beatsPerMeasure))
        topDigit->move(TOP_DIGIT_POS + QPoint(offsets[beatsPerMeasure], 0));

    if (offsets.count(beatUnit))
        bottomDigit->move(BOTTOM_DIGIT_POS + QPoint(offsets[beatUnit], 0));

    // Set time signature
    topDigit->setText(text.at(1));
    bottomDigit->setText(text.at(3));
}

