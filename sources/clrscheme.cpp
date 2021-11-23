#include "clrscheme.h"

#include <QButtonGroup>
#include <QGridLayout>
#include <QTextStream>

ClrScheme::ClrScheme(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);

    QButtonGroup *clrSchBtnGroup = new QButtonGroup(this);
    clrSch1Btn = new QRadioButton("Color Scheme 1", this);
    clrSch2Btn = new QRadioButton("Color Scheme 2", this);
    clrSch3Btn = new QRadioButton("Color Scheme 3", this);
    clrSch4Btn = new QRadioButton("Color Scheme 4", this);
    clrSch5Btn = new QRadioButton("Color Scheme 5", this);
    clrSch6Btn = new QRadioButton("Color Scheme 6", this);

    clrSchBtnGroup->addButton(clrSch1Btn);
    clrSchBtnGroup->addButton(clrSch2Btn);
    clrSchBtnGroup->addButton(clrSch3Btn);
    clrSchBtnGroup->addButton(clrSch4Btn);
    clrSchBtnGroup->addButton(clrSch5Btn);
    clrSchBtnGroup->addButton(clrSch6Btn);

    connect(clrSch1Btn, &QRadioButton::toggled,
        this, &ClrScheme::clrSch1Select);
    connect(clrSch2Btn, &QRadioButton::toggled,
        this, &ClrScheme::clrSch2Select);
    connect(clrSch3Btn, &QRadioButton::toggled,
        this, &ClrScheme::clrSch3Select);
    connect(clrSch4Btn, &QRadioButton::toggled,
        this, &ClrScheme::clrSch4Select);
    connect(clrSch5Btn, &QRadioButton::toggled,
        this, &ClrScheme::clrSch5Select);
    connect(clrSch6Btn, &QRadioButton::toggled,
        this, &ClrScheme::clrSch6Select);

    layout->addWidget(clrSch1Btn, 0, 0);
    layout->addWidget(clrSch2Btn, 0, 1);
    layout->addWidget(clrSch3Btn, 0, 2);
    layout->addWidget(clrSch4Btn, 1, 0);
    layout->addWidget(clrSch5Btn, 1, 1);
    layout->addWidget(clrSch6Btn, 1, 2);
}

void ClrScheme::setActiveBtn(int btnNum)
{
    QTextStream out{stdout};
    switch (btnNum) {
        case 1:
            clrSch1Btn->setChecked(true);
            break;
        case 2:
            clrSch2Btn->setChecked(true);
            break;
        case 3:
            clrSch3Btn->setChecked(true);
            break;
        case 4:
            clrSch4Btn->setChecked(true);
            break;
        case 5:
            clrSch5Btn->setChecked(true);
            break;
        case 6:
            clrSch6Btn->setChecked(true);
            break;
        default:
            out << "Color scheme option doesn't exist." << endl;
    }
}
