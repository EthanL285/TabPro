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
    QPixmap getNotePixmap(QString note);
    void addNote(QString note, int string, int fretNumber);
    void removeNote(int index);
    void addBlank(int index);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QHBoxLayout *mainLayout;
    int length = 95;
    QString selectedNote = "crotchet";
    QMap<QString, QPixmap> notePixmaps;
    QMap<int, QString> noteMap;
    QMap<int, QPair<int, int>> stringMap;
    QVector<QWidget*> notes;

signals:

};

#endif // STAFF_H
