#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H

#include "sketch.h"

#include <QDate>
#include <QPushButton>
#include <QWidget>

class SketchWindow: public QWidget
{
    Q_OBJECT

    public:
        SketchWindow(QWidget *parent = 0);

        void setCurrentDate(const QDate newDate);
        void sketchActivate();
        void toOpenImage();

        QPushButton *saveBtn;

    signals:
        void imageSaved();

    private slots:
        void chgPenColor();
        void toSaveImage();

    private:
        Sketch *sketch;

        QDate curDate;

        QPushButton *chgPenColorBtn;
        QPushButton *clearBtn;
};

#endif
