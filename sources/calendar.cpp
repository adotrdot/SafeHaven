#include "calendar.h"

#include <QHBoxLayout>

Calendar::Calendar(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    calendar = new QCalendarWidget(this);
    calendar->setFixedSize(350, 230);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar->setDateRange(
        QDate(1900, 1, 1),
        QDate(2099, 12, 31));
    
    connect(calendar, &QCalendarWidget::clicked,
        this, &Calendar::dateSelect);

    calendar->hide();

    layout->addWidget(calendar);
}

void Calendar::calendarShow(const QDate curDate)
{
    calendar->setSelectedDate(curDate);
    calendar->show();
}

void Calendar::dateSelect(const QDate selectedDate)
{
    emit toChgDate(selectedDate);
}
