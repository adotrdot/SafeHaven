#include <sketchwindow.h>

#include <QColorDialog>
#include <QHBoxLayout>
#include <QTextStream>
#include <QVBoxLayout>

SketchWindow::SketchWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *sketchLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout1 = new QHBoxLayout();
    QHBoxLayout *buttonLayout2 = new QHBoxLayout();
    sketchLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout1->setContentsMargins(5, 0, 5, 0);
    buttonLayout2->setContentsMargins(5, 0, 5, 0);

    sketch = new Sketch(this);
    sketch->setFixedSize(350, 350);
    sketch->hide();

    chgPenColorBtn = new QPushButton("Change Pen Color", this);
    clearBtn = new QPushButton("Clear", this);
    saveBtn = new QPushButton("Save", this);

    connect(chgPenColorBtn, &QPushButton::clicked,
        this, &SketchWindow::chgPenColor);
    connect(clearBtn, &QPushButton::clicked,
        sketch, &Sketch::clearImage);
    connect(saveBtn, &QPushButton::clicked,
        this, &SketchWindow::toSaveImage);

    buttonLayout1->addWidget(chgPenColorBtn);
    buttonLayout2->addWidget(clearBtn);
    buttonLayout2->addWidget(saveBtn);

    sketchLayout->addWidget(sketch);
    sketchLayout->addLayout(buttonLayout1);
    sketchLayout->addLayout(buttonLayout2);

    setWindowTitle("Sketch");
    setFixedSize(350, 425);
}

void SketchWindow::setCurrentDate(const QDate newDate)
{
    curDate = newDate;
}

void SketchWindow::sketchActivate()
{
    sketch->show();
}

void SketchWindow::toOpenImage()
{
    QTextStream out{stdout};

    QString fileName = "contents/sketches/"
        + curDate.toString("yyyyMMdd") + ".png";

    out << "Opening image..." << endl;
    sketch->openImage(fileName);
}

void SketchWindow::chgPenColor()
{
    QColor newColor = QColorDialog::getColor(
        sketch->getPenColor(),
        this, "Select New Color",
        QColorDialog::DontUseNativeDialog);
    if (newColor.isValid())
        sketch->setPenColor(newColor);
}

void SketchWindow::toSaveImage()
{
    QTextStream out{stdout};

    QString fileName = "contents/sketches/"
        + curDate.toString("yyyyMMdd") + ".png";

    out << "Saving image..." << endl;
    sketch->saveImage(fileName);
    emit imageSaved();
}
