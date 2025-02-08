#ifndef TABLATUREINDENT_H
#define TABLATUREINDENT_H

#include <QWidget>

class TablatureIndent : public QWidget
{
    Q_OBJECT
public:
    explicit TablatureIndent(int width, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
};

#endif // TABLATUREINDENT_H
