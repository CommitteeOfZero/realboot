#include "launcherwindow.h"
#include "launcherapplication.h"

int main(int argc, char *argv[]) {
    QApplication::setDesktopSettingsAware(false);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    LauncherApplication a(argc, argv);

    a.showWindow();

    return a.exec();
}
