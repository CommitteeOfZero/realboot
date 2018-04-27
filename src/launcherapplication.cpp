#include "launcherapplication.h"
#include "launcherwindow.h"
#include "gameconfig.h"
#include "globals.h"
#include <QFile>
#include <QTextStream>
#include <QStyleFactory>
#include <QDir>
#include "Windows.h"
#include "Shlobj.h"

LauncherApplication::LauncherApplication(int& argc, char** argv)
    : QApplication(argc, argv) {
    QDir(gameConfigDirectory()).mkpath(".");

    gc = new GameConfig(this);
    w = new LauncherWindow(0);

    // I would *like* to apply the style to the whole application
    // (for unparented MessageBoxes), but that breaks the X button image...
    w->setStyle(QStyleFactory::create("windows"));
    QFile qssFile(":/kofuna/style.qss");
    qssFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&qssFile);
    w->setStyleSheet(ts.readAll());

    w->show();
}

LauncherApplication::~LauncherApplication() { delete w; }

QString LauncherApplication::gameConfigDirectory() const {
    PWSTR myDocumentsPath;
    SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &myDocumentsPath);
    QString myGamesPath =
        QString::fromWCharArray(myDocumentsPath) + "/My Games/";
    CoTaskMemFree(myDocumentsPath);
    return myGamesPath + game_GameConfPath;
}

void LauncherApplication::showWindow() { w->show(); }