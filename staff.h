#ifndef STAFF_H
#define STAFF_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPixmap>

class Staff : public QWidget
{
    Q_OBJECT
public:
    explicit Staff(QWidget *parent = nullptr);
    void updateLineLength(bool add);
    QMap<int, QPair<int, int>> createStringMap();
    QMap<int, QString> createNoteMap();
    void addNote(QString note, int string, int fretNumber);
    QPixmap getNotePixmap(QString note);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QHBoxLayout *mainLayout;
    int length = 95;
    QString selectedNote = "crotchet";
    QMap<QString, QPixmap> notePixmaps;
    QMap<int, QString> noteMap;
    QMap<int, QPair<int, int>> stringMap;

signals:

};

#endif // STAFF_H
