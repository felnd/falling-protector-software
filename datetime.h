#ifndef DATETIME_H
#define DATETIME_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class DateTime;
}

class DateTime : public QWidget
{
    Q_OBJECT
signals:
    void sendTime(QDateTime time);
public:
    explicit DateTime(QWidget *parent = 0);
    ~DateTime();

private slots:
    void on_calendarWidget_selectionChanged();

private:
    Ui::DateTime *ui;
};

#endif // DATETIME_H
