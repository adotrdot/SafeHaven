#include "editor.h"

#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFontDatabase::addApplicationFont(":/contents/ui/VarelaRound-Regular.ttf");
    QFont newFont("Varela Round");
    app.setFont(newFont);
    app.setWindowIcon(QIcon(":/contents/ui/app_icon.svg"));
    app.setStyle(QStyleFactory::create("Fusion"));

    Editor window;
    
    window.setGeometry(150, 150, 600, 300);
    window.setWindowFlags(Qt::Dialog);
    window.setWindowTitle("SafeHaven");
    window.show();

    return app.exec();
}
