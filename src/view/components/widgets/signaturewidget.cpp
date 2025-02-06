#include "signaturewidget.h"
#include "signaturehelper.h"

#include <QVBoxLayout>

SignatureWidget::SignatureWidget(QString text, int fontSize, QPoint topDigitPos, QPoint bottomDigitPos, QWidget *parent)
    : fontSize{fontSize}, topDigitPos{topDigitPos}, bottomDigitPos{bottomDigitPos}, QWidget{parent}
{
    setFixedSize(fontSize + 7, fontSize + 10);
    topDigit = createDigit(text.at(1));
    bottomDigit = createDigit(text.at(3));

    topDigit->move(topDigitPos);
    bottomDigit->move(bottomDigitPos);
}

// Creates a digit label
QLabel *SignatureWidget::createDigit(QString text)
{
    QLabel *digit = new QLabel(text, this);
    digit->setAlignment(Qt::AlignCenter);
    digit->setFont(QFont("Bravura Text", fontSize));
    digit->setFixedSize(fontSize + 50, fontSize + 50);

    return digit;
}

// Getter for top digit value
int SignatureWidget::getTopDigit()
{
    return SignatureHelper::unicodeToNum(topDigit->text());
}

// Getter for bottom digit value
int SignatureWidget::getBottomDigit()
{
    return SignatureHelper::unicodeToNum(bottomDigit->text());
}

// Changes the time signature
void SignatureWidget::changeSignature(int beatsPerMeasure, int beatUnit)
{
    // Map beats to their respective horizontal offsets
    std::unordered_map<int, int> offsets = { {9, 2}, {8, 2}, {7, 2}, {6, 2}, {5, 3}, {4, 0}, {3, 2}, {2, 2}, {1, 4} };

    // Get unicode representations
    QString topDigitText = SignatureHelper::numToUnicode(beatsPerMeasure);
    QString bottomDigitText = SignatureHelper::numToUnicode(beatUnit);

    // Update positions
    topDigit->move(topDigitPos + QPoint(offsets[beatsPerMeasure], 0));
    bottomDigit->move(bottomDigitPos + QPoint(offsets[beatUnit], 0));

    // Set time signature
    topDigit->setText(topDigitText);
    bottomDigit->setText(bottomDigitText);
}

