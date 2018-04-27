#include "launcherapplication.h"
#include "launcherwindow.h"
#include <QFile>
#include <QTextStream>
#include <QStyleFactory>

LauncherApplication::LauncherApplication(int& argc, char** argv)
    : QApplication(argc, argv) {
    w = new LauncherWindow(0);

    // we do not set these globally so that we can have unthemed dialogs
    w->setStyle(QStyleFactory::create("windows"));
    QFile qssFile(":/kofuna/style.qss");
    qssFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&qssFile);
    w->setStyleSheet(ts.readAll());

    w->show();
}

LauncherApplication::~LauncherApplication() { delete w; }

void LauncherApplication::showWindow() { w->show(); }