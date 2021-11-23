#ifndef CALENDAR_H
#define CALENDAR_H

#include <QCalendarWidget>
#include <QWidget>

class Calendar: public QWidget
{
    Q_OBJECT

    public:
        Calendar(QWidget *parent = 0);

        void calendarShow(const QDate curDate);

    signals:
        void toChgDate(const QDate newDate);

    private slots:
        void dateSelect(const QDate selectedDate);

    private:
        QCalendarWidget *calendar;
};

#endif
