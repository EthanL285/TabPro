#ifndef SIGNATURELABEL_H
#define SIGNATURELABEL_H

#include <QWidget>
#include <QLabel>
#include <QEvent>

class SignatureLabel : public QLabel
{
    Q_OBJECT
public:
    explicit SignatureLabel(QString text, QWidget *parent = nullptr);

signals:
};

#endif // SIGNATURELABEL_H
