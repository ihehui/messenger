
#include <QtGui>
#include <QApplication>
#include <QSplashScreen>
#include <iostream>

#include "./mainwindow/mainwindow.h"

//#include "login/login.h"
//#include "./networkmanager/clientresourcesmanager.h"

#include "HHSharedCore/hlogdebug.h"


using namespace std;
using namespace HEHUI;

void showSplashMessage(QSplashScreen *s, const QString &str)
{
    s->showMessage(str, Qt::AlignRight | Qt::AlignBottom, Qt::darkGreen);
}


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);


    cout << qPrintable(QString(APP_NAME) + " Build " + QString(APP_VERSION)) << endl << endl;


    for(int i = 0; i < argc; i++) {
        if(QString(argv[i]).toLower() == "-log") {
            qInstallMessageHandler(logDebug);
            qAddPostRoutine(closeDebugLog);
        }
    }


    QDate date = QDate::currentDate();
    if(date.year() != 2017) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Application has expired!"));
        qDebug() << "Application has expired!";
        return 0;
    }

    app.addLibraryPath(QCoreApplication::applicationDirPath());
    app.addLibraryPath(QCoreApplication::applicationDirPath() + QDir::separator () + QString(PLUGINS_MAIN_DIR));
    app.addLibraryPath(QCoreApplication::applicationDirPath() + QDir::separator () + QString(MYLIBS_DIR));
    qDebug() << "----main(int argc, char *argv[])~~Library Paths" << app.libraryPaths ();


    //创建Splash Screen
    //Create Splash Screen
    //QSplashScreen *splash = new QSplashScreen( QPixmap(QString(RESOURCE_PATH)+QString(APP_SPLASH_IMAGE_PATH)) );
    //splash->show();

    //设置程序信息
    //Setup the program information
    //showSplashMessage(splash, QObject::tr("Init Application Info"));
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);
    app.setWindowIcon(QIcon(QString(RESOURCE_PATH) + QString(APP_ICON_PATH)));
    app.setOrganizationName(APP_ORG);
    app.setOrganizationDomain(APP_ORG_DOMAIN);
    //showSplashMessage(splash, QString(APP_NAME) + " " + QString(APP_VERSION));



    //创建主窗口
    //Create the main window
    MainWindow mw;
    mw.show();

    //结束Splash Screen
    //Finish the Splash Screen
    //splash->finish(&w);
    //delete splash;


    return app.exec();

}






