#include "editor.h"

#include <stdlib.h>
#include <time.h>

#include <QButtonGroup>
#include <QDir>
#include <QFileInfo>
#include <QFont>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QPainter>
#include <QSettings>
#include <QShortcut>
#include <QTextStream>
#include <QUrl>
#include <QVBoxLayout>

Editor::Editor(QWidget *parent)
    : QWidget(parent)
{
    QTextStream out{stdout};

    // Randomize seed, to randomly select placeholder text later
    srand (time(NULL));

    // Preparing texts directory
    QDir dir;
    if (!dir.exists("safehaven_data/texts")) {
        out << "Creating texts directory..." << endl;
        if (dir.mkpath("safehaven_data/texts")) {
            out << "Successfully created texts directory" << endl;
        } else {
            out << "Failed to create texts directory" << endl;
        }
    } else {
        out << "Texts directory found" << endl;
    }

    // Preparing sketches directory
    if (!dir.exists("safehaven_data/sketches")) {
        out << "Creating sketches directory..." << endl;
        if (dir.mkpath("safehaven_data/sketches")) {
            out << "Successfully created sketches directory" << endl;
        } else {
            out << "Failed to create sketches directory" << endl;
        }
    } else {
        out << "Sketches directory found" << endl;
    }

    // Preparing color scheme config
    QSettings config(QString("safehaven_data/config.ini"), QSettings::IniFormat);
    int clrScheme = config.value("ui/color-scheme", 1).toInt();

    // Preparing clrscheme popup,
    // calendar popup, and sketch window and misc
    clrSchemePopup = new ClrScheme(this);
    clrSchemePopup->setWindowFlags(Qt::Dialog);
    clrSchemePopup->setWindowModality(Qt::WindowModal);
    clrSchemePopup->setWindowTitle("Select Color Scheme");

    connect(clrSchemePopup, &ClrScheme::clrSchemeSelected,
        this, &Editor::setColorScheme);

    calendarPopup = new Calendar(this);
    calendarPopup->setWindowFlags(Qt::Popup);
    calendarPopup->setWindowModality(Qt::WindowModal);
    calendarPopup->setWindowTitle("Calendar");

    connect(calendarPopup, &Calendar::toChgDate,
        this, &Editor::dateChanged);

    calendarPopup->hide();

    sketchWindow = new SketchWindow(this);
    sketchWindow->setWindowFlags(Qt::Dialog);
    sketchWindow->setWindowModality(Qt::WindowModal);
    sketchWindow->hide();

    connect(sketchWindow, &SketchWindow::imageSaved,
        this, &Editor::setImage);

    clrSchemePopup->setActiveBtn(clrScheme);

    // Layout Declarations
    QHBoxLayout *hbox = new QHBoxLayout(this);
    QHBoxLayout *toolBar = new QHBoxLayout();
    QHBoxLayout *musicBar = new QHBoxLayout();
    QVBoxLayout *vbox1 = new QVBoxLayout();
    QVBoxLayout *vbox2 = new QVBoxLayout();

    // Application's Middle Side (Date + Editor + ToolBar + MusicBar) -- BEGIN

    // Date
    curDate = QDate::currentDate();
    date = new QLabel(curDate.toString("MMMM dd, yyyy"), this);

    // Editor
    QTextDocument *text = new QTextDocument();
    text->setHtml(getText());
    editor = new QTextEdit(this);
    editor->setStyleSheet("background-color: #ffffff;");
    editor->setDocument(text);

    connect(editor, &QTextEdit::cursorPositionChanged,
        this, &Editor::checkStyle);
    connect(editor, &QTextEdit::textChanged,
        this, &Editor::save);

    // Editor's Placeholder Text
    editor->setPlaceholderText(placeholderTexts.at(rand() % 3));

    // ToolBar
    boldBtn = new QCheckBox(this);
    QLabel *boldBtnText = new QLabel("<b>B</b>", this);
    italicBtn = new QCheckBox(this);
    QLabel *italicBtnText = new QLabel("<i>I</i>", this);
    underlineBtn = new QCheckBox(this);
    QLabel *underlineBtnText = new QLabel("<u>U</u>", this);
    colorSchBtn = new QPushButton("Color Scheme", this);

    connect(boldBtn, &QCheckBox::stateChanged,
        this, &Editor::boldStyle);
    connect(italicBtn, &QCheckBox::stateChanged,
        this, &Editor::italicStyle);
    connect(underlineBtn, &QCheckBox::stateChanged,
        this, &Editor::underlineStyle);
    connect(colorSchBtn, &QPushButton::clicked,
        this, &Editor::clrSchemeShow);

    // ToolBar's Layout Setting
    toolBar->addWidget(boldBtn);
    toolBar->addWidget(boldBtnText);
    toolBar->addWidget(italicBtn);
    toolBar->addWidget(italicBtnText);
    toolBar->addWidget(underlineBtn);
    toolBar->addWidget(underlineBtnText);
    toolBar->addWidget(colorSchBtn, 3);

    // MusicBar
    QButtonGroup *musicBarBtnGroup = new QButtonGroup(this);
    musicLbl = new QLabel("BG Music : ", this);
    musicDisableBtn = new QRadioButton("None", this);
    happyMusicBtn = new QRadioButton("Happy", this);
    sadMusicBtn = new QRadioButton("Sad", this);

    // MusicBar Settings
    musicPlayer = new QMediaPlayer();
    musicPlayer->setVolume(75);

    musicBarBtnGroup->addButton(musicDisableBtn);
    musicBarBtnGroup->addButton(happyMusicBtn);
    musicBarBtnGroup->addButton(sadMusicBtn);

    musicDisableBtn->setChecked(true);

    timer = new QTimer(this);

    connect(musicDisableBtn, &QRadioButton::toggled, this, &Editor::disableMusic);
    connect(happyMusicBtn, &QRadioButton::toggled, this, &Editor::enableMusic1);
    connect(sadMusicBtn, &QRadioButton::toggled, this, &Editor::enableMusic2);
    connect(musicPlayer, &QMediaPlayer::durationChanged, this, &Editor::startTimer);
    connect(timer, &QTimer::timeout, this, &Editor::loopMusic);

    // MusicBar's Layout Setting
    musicBar->addWidget(musicLbl);
    musicBar->addWidget(musicDisableBtn);
    musicBar->addWidget(happyMusicBtn);
    musicBar->addWidget(sadMusicBtn);
    musicBar->addStretch(1);

    // Layout Setting
    vbox1->addWidget(date);
    vbox1->addSpacing(5);
    vbox1->addWidget(editor);
    vbox1->addLayout(toolBar);
    vbox1->addLayout(musicBar);

    // -- END
    
    // Application's Right Side (SaveBtn + ChgDateBtn + SketchBtn&View) -- BEGIN
    
    // SaveButton
    saveBtn = new QPushButton("Save", this);
    connect(saveBtn, &QPushButton::clicked, this, &Editor::save);

    // ChangeDateBtn
    chgDateBtn = new QPushButton("Change Date", this);
    connect(chgDateBtn, &QPushButton::clicked,
        this, &Editor::calendarShow);

    // EditSketchBtn
    editSketchBtn = new QPushButton("Edit Sketch", this);
    connect(editSketchBtn, &QPushButton::clicked,
        this, &Editor::sketchShow);

    // SketchView
    sketchView = new QLabel(this);
    setImage();

    // Layout Setting
    vbox2->setSpacing(5);
    vbox2->addWidget(saveBtn);
    vbox2->addWidget(chgDateBtn);
    vbox2->addWidget(sketchView);
    vbox2->addWidget(editSketchBtn);
    vbox2->addStretch(1);

    // -- END

    // Overall Layout Setting
    hbox->addLayout(vbox1);
    hbox->addSpacing(15);
    hbox->addLayout(vbox2);

    // Application's Shortcuts
    QShortcut *toBold = new QShortcut(
        QKeySequence(Qt::CTRL + Qt::Key_B), this);
    QShortcut *toItalic = new QShortcut(
        QKeySequence(Qt::CTRL + Qt::Key_I), this);
    QShortcut *toUnderline = new QShortcut(
        QKeySequence(Qt::CTRL + Qt::Key_U), this);
    QShortcut *toSave = new QShortcut(
        QKeySequence(Qt::CTRL + Qt::Key_S), this);

    connect(toBold, &QShortcut::activated, this, &Editor::toBold);
    connect(toItalic, &QShortcut::activated, this, &Editor::toItalic);
    connect(toUnderline, &QShortcut::activated, this, &Editor::toUnderline);
    connect(toSave, &QShortcut::activated, this, &Editor::save);
}


void Editor::calendarShow()
{
    QTextStream out{stdout};

    int x = this->geometry().x() + this->width();
    int y = this->geometry().y();

    out << "Showing calendar popup" << endl;
    calendarPopup->setGeometry(
        x, y, 350, 230);
    calendarPopup->show();
    calendarPopup->calendarShow(curDate);
}

void Editor::clrSchemeShow()
{
    QTextStream out{stdout};

    clrSchemePopup->setMinimumWidth(475);
    clrSchemePopup->setMinimumHeight(75);

    int w = clrSchemePopup->minimumWidth();
    int h = clrSchemePopup->minimumHeight();

    clrSchemePopup->setGeometry(
        this->geometry().x() + (this->width() / 2) - (w / 2),
        this->geometry().y() + (this->height() / 2) - (h / 2),
        w, h);

    out << "Showing color scheme selection popup" << endl;
    clrSchemePopup->show();
}

void Editor::sketchShow()
{
    QTextStream out {stdout};

    sketchWindow->setGeometry(
        this->x() + this->width(),
        this->y(), 500, 500);
    sketchWindow->setCurrentDate(curDate);
    sketchWindow->toOpenImage();
    out << "Showing sketch window" << endl;
    sketchWindow->show();
    sketchWindow->sketchActivate();
}

void Editor::dateChanged(QDate newDate)
{
    curDate = newDate;
    date->setText(curDate.toString("MMMM dd, yyyy"));

    QTextDocument *newText = new QTextDocument();
    newText->setHtml(getText());
    editor->setDocument(newText);
    editor->setPlaceholderText(placeholderTexts.at(rand() % 3));

    setImage();
}

void Editor::save()
{
    QTextStream out{stdout};

    QString fileName = "safehaven_data/texts/" + curDate.toString("yyyyMMdd") + ".txt";

    // Saving text file
    QFile f1{fileName};
    if (f1.open(QIODevice::WriteOnly)) {
        QTextStream out{&f1};

        QString text = editor->toHtml();
        out << text;
    } else {
        qWarning("Could not save text file");
    }

    out << "Text file saved successfully" << endl;
}

void Editor::setImage()
{
    QTextStream out{stdout};

    int h = 200;
    int w = 200;
    QPixmap newImage(w, h);
    QString fileName = "safehaven_data/sketches/" + curDate.toString("yyyyMMdd") + ".png";

    QFile f{fileName};
    if (!f.exists()) {
        qWarning("Image file not found");
        newImage.fill(qRgb(255, 255, 255));
        sketchView->setPixmap(newImage);
        return;
    }

    newImage.load(fileName);
    out << "Image file openend successfully" << endl;

    sketchView->setPixmap(
        newImage.scaled(w, h,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation));
    out << "Image set successfully" << endl;
}

void Editor::setColorScheme(const QString clrScheme, int btnNum)
{
    setStyleSheet(
        "background-color: " + clrScheme + ";");
    clrSchemePopup->setStyleSheet(
        "background-color: " + clrScheme + ";");
    sketchWindow->setStyleSheet(
        "background-color: " + clrScheme + ";");

    calendarPopup->close();
    calendarPopup = new Calendar(this);
    calendarPopup->setWindowFlags(Qt::Popup);
    calendarPopup->setWindowModality(Qt::WindowModal);
    calendarPopup->setWindowTitle("Calendar");

    connect(calendarPopup, &Calendar::toChgDate,
        this, &Editor::dateChanged);

    calendarPopup->hide();

    calendarPopup->setStyleSheet(
        "QCalendarWidget QWidget#qt_calendar_navigationbar {background-color: " + clrScheme + ";}"
        "QCalendarWidget QWidget#qt_calendar_prevmonth {qproperty-icon: url(\":/contents/ui/left_arrow.png\");}"
        "QCalendarWidget QWidget#qt_calendar_nextmonth {qproperty-icon: url(\":/contents/ui/right_arrow.png\");}"
        "QCalendarWidget QMenu QSpinBox {background-color: " + clrScheme + ";}"
        "QCalendarWidget QToolButton {background-color: " + clrScheme + "; color: black;}"
        "QCalendarWidget QWidget {alternate-background-color: " + clrScheme + ";}"
        "QCalendarWidget QAbstractItemView {background-color: white; selection-color: black;"
        "selection-background-color: " + clrScheme + ";}");

    QSettings config(QString("safehaven_data/config.ini"), QSettings::IniFormat);
    config.setValue("ui/color-scheme", btnNum);
}

void Editor::boldStyle()
{
    QTextStream out{stdout};

    if (boldBtn->checkState()) {
        editor->setFontWeight(QFont::Bold);
        out << "Bold true" << endl;
    } else {
        editor->setFontWeight(QFont::Normal);
        out << "Bold false" << endl;
    }
}

void Editor::toBold()
{
    QTextStream out{stdout};

    boldBtn->setChecked(!boldBtn->isChecked());
    out << "Bold status changed" << endl;
}

void Editor::italicStyle()
{
    QTextStream out{stdout};

    if (italicBtn->checkState()) {
        editor->setFontItalic(true);
        out << "Italic true" << endl;
    } else {
        editor->setFontItalic(false);
        out << "Italic false" << endl;
    }
}

void Editor::toItalic()
{
    QTextStream out{stdout};

    italicBtn->setChecked(!italicBtn->isChecked());
    out << "Italic status changed" << endl;
}

void Editor::underlineStyle()
{
    QTextStream out{stdout};

    if (underlineBtn->checkState()) {
        editor->setFontUnderline(true);
        out << "Underline true" << endl;
    } else {
        editor->setFontUnderline(false);
        out << "Underline false" << endl;
    }
}

void Editor::toUnderline()
{
    QTextStream out{stdout};

    underlineBtn->setChecked(!underlineBtn->isChecked());
    out << "Underline status changed" << endl;
}

void Editor::checkStyle()
{
    QTextCharFormat curCharFormat = editor->currentCharFormat();

    boldBtn->setChecked(curCharFormat.fontWeight() == QFont::Bold);
    italicBtn->setChecked(curCharFormat.fontItalic());
    underlineBtn->setChecked(curCharFormat.fontUnderline());
}

void Editor::disableMusic()
{
    QTextStream out{stdout};

    out << "Stopping timer" << endl;
    timer->stop();
    out << "Timer stopped" << endl;
    
    out << "Stopping music" << endl;
    musicPlayer->stop();
    musicPlayer->setMedia(NULL);
    out << "Music stopped" << endl;
}

void Editor::enableMusic1()
{
    QTextStream out{stdout};

    QString fileName = ":/contents/audio/audio1.mp3";
    if (!QFile{fileName}.exists()) {
        qWarning("Audio 1 file does not exist");
    } else {
        musicPlayer->setMedia(QUrl("qrc:///contents/audio/audio1.mp3"));
        out << "Music 1 set" << endl;

        playMusic();
    }
}

void Editor::enableMusic2()
{
    QTextStream out{stdout};

    QString fileName = ":/contents/audio/audio2.mp3";
    if (!QFile{fileName}.exists()) {
        qWarning("Audio 2 file does not exist");
    } else {
        musicPlayer->setMedia(QUrl("qrc:///contents/audio/audio2.mp3"));
        out << "Music 2 set" << endl;

        playMusic();
    }
}

void Editor::startTimer(qint64 dur)
{
    QTextStream out{stdout};

    out << "Starting timer..." << endl;
    timer->start(dur + 1000);
}

void Editor::loopMusic()
{
    QTextStream out{stdout};

    out << "Looping music..." << endl;
    musicPlayer->stop();
    musicPlayer->play();
}

QString Editor::getText()
{
    QTextStream out{stdout};

    QString fileName = "safehaven_data/texts/" + curDate.toString("yyyyMMdd") + ".txt";

    QFile f{fileName};
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning("Could not open text file");
        return ("");
    }

    QTextStream in{&f};
    QString text = in.readAll();

    out << "Text file read successfully" << endl;
    return text;
}

void Editor::playMusic()
{
    QTextStream out{stdout};

    out << "Playing music..." << endl;
    musicPlayer->play();
}
