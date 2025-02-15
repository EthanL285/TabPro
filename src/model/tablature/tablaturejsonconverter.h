#ifndef TABLATUREJSONCONVERTER_H
#define TABLATUREJSONCONVERTER_H

#include <QObject>

class TablatureJsonConverter : public QObject
{
    Q_OBJECT
public:
    explicit TablatureJsonConverter(QObject *parent = nullptr);

signals:
};

#endif // TABLATUREJSONCONVERTER_H
