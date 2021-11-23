#ifndef EDITOR_H
#define EDITOR_H

#include "calendar.h"
#include "clrscheme.h"
#include "sketchwindow.h"

#include <QCheckBox>
#include <QDate>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMediaPlayer>
#include <QPixmap>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>
#include <QTimer>
#include <QWidget>

class Editor: public QWidget
{
    Q_OBJECT

    public:
        Editor(QWidget *parent = 0);

        QDate getCurrentDate() const { return curDate; }

    private slots:
        void calendarShow();
        void clrSchemeShow();
        void sketchShow();
        void dateChanged(QDate newDate);

        void save();
        void setImage();

        void setColorScheme(const QString clrScheme, int btnNum);

        void boldStyle();
        void toBold();
        void italicStyle();
        void toItalic();
        void underlineStyle();
        void toUnderline();
        void checkStyle();

        void disableMusic();
        void enableMusic1();
        void enableMusic2();
        void startTimer(qint64 dur); // dur = duration
        void loopMusic();

    private:
        QString getText();
        void playMusic();

        Calendar *calendarPopup;
        ClrScheme *clrSchemePopup;
        SketchWindow *sketchWindow;

        QTimer *timer;

        QTextEdit *editor;

        QList<QString> placeholderTexts = {
            "How are you?",
            "What's on your mind?",
            "Write whatever here!"};

        QDate curDate;
        QLabel *date;
        QLabel *musicLbl;
        QLabel *sketchView;
        QPixmap sketchImage;

        QPushButton *editSketchBtn;
        QPushButton *saveBtn;
        QPushButton *chgDateBtn;
        QCheckBox *boldBtn;
        QCheckBox *italicBtn;
        QCheckBox *underlineBtn;
        QPushButton *colorSchBtn;
        QRadioButton *musicDisableBtn;
        QRadioButton *happyMusicBtn;
        QRadioButton *sadMusicBtn;

        QMediaPlayer *musicPlayer;
};

#endif
