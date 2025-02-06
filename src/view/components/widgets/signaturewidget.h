#ifndef SIGNATUREWIDGET_H
#define SIGNATUREWIDGET_H

#include <QWidget>
#include <QLabel>

class SignatureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SignatureWidget(QString text, int fontSize, QPoint topDigitPos, QPoint bottomDigitPos, QWidget *parent = nullptr);
    QLabel *createDigit(QString text);
    void changeSignature(int beatsPerMeasure, int beatUnit);

    int getTopDigit();
    int getBottomDigit();

private:
    int fontSize;
    QLabel *topDigit;
    QLabel *bottomDigit;
    QPoint topDigitPos;
    QPoint bottomDigitPos;

signals:
};

#endif // SIGNATUREWIDGET_H
