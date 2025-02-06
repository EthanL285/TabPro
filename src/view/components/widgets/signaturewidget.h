#ifndef SIGNATUREWIDGET_H
#define SIGNATUREWIDGET_H

#include <QWidget>
#include <QLabel>

class SignatureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SignatureWidget(QString text, QWidget *parent = nullptr);
    QLabel *createDigit(QString text);
    void changeSignature(QString text);

    QString getTopDigit();
    QString getBottomDigit();

private:
    QLabel *topDigit;
    QLabel *bottomDigit;

signals:
};

#endif // SIGNATUREWIDGET_H
