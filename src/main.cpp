#include "launcherwindow.h"
#include "launcherapplication.h"
#include <curl/curl.h>

int main(int argc, char *argv[]) {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    QApplication::setDesktopSettingsAware(false);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    LauncherApplication a(argc, argv);

    a.showWindow();

    int code = a.exec();
    curl_global_cleanup();
    return code;
}
