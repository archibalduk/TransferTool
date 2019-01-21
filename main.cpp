#include "mainwindow.h"
#include <QApplication>
#include "ui/style_sheet.h"

int main(int argc, char *argv[])
{
    // HIDPI support
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Application details
    QCoreApplication::setApplicationName("Transfer Tool");
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setOrganizationName("EHM The Blue Line");
    QCoreApplication::setOrganizationDomain("ehmtheblueline.com");

    // Application
    QApplication a(argc, argv);

    // Stylesheet
    StyleSheet *s = new StyleSheet();
    s->get();
    delete s;

    // Main window
    MainWindow w;
    w.show();

    return a.exec();
}
