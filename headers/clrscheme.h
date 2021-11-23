#ifndef CLRSCHEME_H
#define CLRSCHEME_H

#include <QRadioButton>
#include <QWidget>

class ClrScheme: public QWidget
{
    Q_OBJECT

    public:
        ClrScheme(QWidget *parent = 0);

        void setActiveBtn(int btnNum);

    signals:
        void clrSchemeSelected(const QString clrScheme, int btnNum);

    private slots:
        void clrSch1Select()
            { emit clrSchemeSelected(clrSch1, 1); }
        void clrSch2Select()
            { emit clrSchemeSelected(clrSch2, 2); }
        void clrSch3Select()
            { emit clrSchemeSelected(clrSch3, 3); }
        void clrSch4Select()
            { emit clrSchemeSelected(clrSch4, 4); }
        void clrSch5Select()
            { emit clrSchemeSelected(clrSch5, 5); }
        void clrSch6Select()
            { emit clrSchemeSelected(clrSch6, 6); }

    private:
        QRadioButton *clrSch1Btn;
        QRadioButton *clrSch2Btn;
        QRadioButton *clrSch3Btn;
        QRadioButton *clrSch4Btn;
        QRadioButton *clrSch5Btn;
        QRadioButton *clrSch6Btn;

        QString clrSch1 = "#eeeeee";
        QString clrSch2 = "#fbe597";
        QString clrSch3 = "#bddffe";
        QString clrSch4 = "#ed3a54";
        QString clrSch5 = "#7bbb67";
        QString clrSch6 = "#d2aff8";
};

#endif
