#ifndef STAFF_H
#define STAFF_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPixmap>

#define UPDATE_LINE 11

class Staff : public QWidget
{
    Q_OBJECT
public:
    explicit Staff(QWidget *parent = nullptr);
    void updateLineLength(bool add);
    void updateHeight(int height, int line);
    QMap<int, QPair<int, int>> createStringMap();
    QMap<int, QString> createNoteMap();
    QPixmap getNotePixmap(QString note);
    void addNote(QString note, int string, int fretNumber, int index);
    void removeNote(int index);
    void addBlank(int index);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QHBoxLayout *mainLayout;
    int length = 95;
    int highestLine = UPDATE_LINE;
    QString selectedNote = "crotchet";
    QMap<QString, QPixmap> notePixmaps;
    QMap<int, QString> noteMap;
    QMap<int, QPair<int, int>> stringMap;

    QVector<QWidget*> notes;
    QVector<int> lines;

signals:

};

#endif // STAFF_H
