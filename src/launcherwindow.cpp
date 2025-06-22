#include "launcherwindow.h"
#include "ui_launcherwindow.h"
#include "launcherapplication.h"
#include "globals.h"
#include "gameconfig.h"
#include "patchconfig.h"
#include "controllerconfig.h"
#include "controllermanager.h"
#include "generaltab.h"
#include "controllertab.h"
#include "troubleshootingtab.h"
#include "dxvktab.h"

#include <QMouseEvent>
#include <QToolButton>
#include <QProcess>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFileInfo>
#include <QDesktopServices>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QMovie>

// https://curl.haxx.se/libcurl/c/getinmemory.html
struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

LauncherWindow::LauncherWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LauncherWindow) {
    setWindowFlag(Qt::FramelessWindowHint);
    ui->setupUi(this);

    if (QFileInfo(":/assets/start_button.png").exists()) {
#if defined(GAME_ANONYMOUSCODE)
        QMovie *gifIcon = new QMovie(this);
        gifIcon->setFileName(":/assets/start_button.png");
        connect(gifIcon, &QMovie::frameChanged,
                [=] { ui->startButton->setIcon(gifIcon->currentPixmap()); });
        gifIcon->start();
#else
        ui->startButton->setIcon(QIcon(":/assets/start_button.png"));
        ui->startButton->setIconSize(QSize(32, 32));
        ui->startButton->setProperty("hasIcon", true);
#endif
        // following is needed for dynamic property based styles to update
        ui->startButton->style()->unpolish(ui->startButton);
        ui->startButton->style()->polish(ui->startButton);
    }
    if (QFileInfo(":/assets/settings_button.png").exists()) {
        ui->toggleSettingsButton->setIcon(
            QIcon(":/assets/settings_button.png"));
        ui->toggleSettingsButton->setIconSize(QSize(32, 32));
        ui->toggleSettingsButton->setProperty("hasIcon", true);
        // following is needed for dynamic property based styles to update
        ui->toggleSettingsButton->style()->unpolish(ui->toggleSettingsButton);
        ui->toggleSettingsButton->style()->polish(ui->toggleSettingsButton);
    }
    if (QFileInfo(":/assets/reset_button.png").exists()) {
        ui->resetButton->setIcon(QIcon(":/assets/reset_button.png"));
        ui->resetButton->setIconSize(QSize(24, 24));
        ui->resetButton->setProperty("hasIcon", true);
        // following is needed for dynamic property based styles to update
        ui->resetButton->style()->unpolish(ui->resetButton);
        ui->resetButton->style()->polish(ui->resetButton);
    }

    ui->headerImage->setStyleSheet("background-color: #000");

    ui->headerImage->installEventFilter(this);
    ui->headerImage->setPixmap(QPixmap(":/assets/game_header.png"));

    // gross but I haven't found any way to overlay a container over other
    // widgets while passing through input events that don't hit any of its
    // child widgets
    QToolButton *crossButton = new QToolButton(this);
    crossButton->setStyleSheet(
        "QToolButton { image: url(':/assets/cross.png'); }"
        "QToolButton:hover { image: url(':/assets/cross_hover.png'); }");
    crossButton->setCursor(QCursor(Qt::PointingHandCursor));
    crossButton->resize(18, 18);
    crossButton->move(width() - (crossButton->width() + 12), 12);
    connect(crossButton, &QAbstractButton::clicked, this,
            &LauncherWindow::cancelRequested);

    ui->bottomBarLayout->setAlignment(Qt::AlignVCenter);

    connect(ui->startButton, &QAbstractButton::clicked, this,
            &LauncherWindow::startGame);
    connect(ui->resetButton, &QAbstractButton::clicked, this,
            &LauncherWindow::resetToDefaults);
    connect(ui->toggleSettingsButton, &QAbstractButton::clicked, this,
            &LauncherWindow::toggleSettings);

    setWindowTitle(game_LauncherTitle);
    ui->techSupportLabel->setTextFormat(Qt::RichText);
    ui->techSupportLabel->setOpenExternalLinks(true);
    ui->techSupportLabel->setText(
        QString("<a href='%1'><span style='font-weight: 600; text-decoration: "
                "underline; color: #fff'>Technical Support</span></a>")
            .arg(game_TechSupportUrl));
#if defined(GAME_STEINSGATEVSO)
    QString version = "1.0.0";
#elif defined(GAME_ANONYMOUSCODE)
    QString version = "1.0.1";
#else
    QFile patchdefFile("languagebarrier/patchdef.json");
    if (!patchdefFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Launcher error",
                              "Couldn't open patch definition");
        QApplication::quit();
    }
    QByteArray patchdefData = patchdefFile.readAll();
    QJsonDocument patchdef = QJsonDocument::fromJson(patchdefData);
    QString version = patchdef.object()["patchVersion"].toString();
#endif

    ui->versionLabel->setTextFormat(Qt::RichText);
    ui->versionLabel->setOpenExternalLinks(true);
    ui->versionLabel->setText(
        QString("<a href='%1'><span style='font-weight: 600; text-decoration: "
                "underline; color: #fff'>Version:</span></a> %2")
            .arg(game_ReleaseUrl, version));

#if !defined(GAME_STEINSGATEVSO)
    _generalTab = new GeneralTab(this);
    ui->tabWidget->addTab(_generalTab, "General");
#endif
#if !defined(GAME_CHAOSHEADNOAH) && !defined(GAME_STEINSGATEELITE) &&        \
    !defined(GAME_STEINSGATEVSO) && !defined(GAME_ROBOTICSNOTESELITE) &&     \
    !defined(GAME_ROBOTICSNOTESDASH) && !defined(GAME_ANONYMOUSCODE)
    _controllerTab = new ControllerTab(this);
    ui->tabWidget->addTab(_controllerTab, "Controller");
#endif
    _troubleshootingTab = new TroubleshootingTab(this);
    ui->tabWidget->addTab(_troubleshootingTab, "Troubleshooting");
#if !defined(GAME_STEINSGATEVSO)
    _dxvkTab = new DxvkTab(this);
    ui->tabWidget->addTab(_dxvkTab, "DXVK");
#endif

    _allSettingsMode = rbApp->patchConfig()->showAllSettings;
    // We respect the user's choice in afterShow(), but we show the full layout first
    // so that's what the window manager uses for setting initial window position
    showFullLayout();

    startUpdateCheck();
}

void LauncherWindow::afterShow() {
    // Window has been positioned, we can switch now
    if (!_allSettingsMode) {
        showMiniLayout();
    }
}

LauncherWindow::~LauncherWindow() { delete ui; }

bool LauncherWindow::eventFilter(QObject *watched, QEvent *event) {
    static QPoint lastPos;
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        lastPos = mouseEvent->globalPos();
        event->accept();
        return true;
    }
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        move(pos() + (mouseEvent->globalPos() - lastPos));
        lastPos = mouseEvent->globalPos();
        event->accept();
        return true;
    }

    return false;
}

void LauncherWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    cancelRequested();
}

void LauncherWindow::cancelRequested() {
    saveChanges();
    QApplication::quit();
}

void LauncherWindow::startGame() {
    setEnabled(false);

    saveChanges();

#if defined(IPC_ENABLED)
    volatile void *ipc;
    HANDLE ipcFile;
    ipcFile = CreateFileMappingW(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, 8,
                                 game_ipcName);
    ipc = MapViewOfFile(ipcFile, FILE_MAP_ALL_ACCESS, 0, 0, 8);
    ((volatile uint32_t *)ipc)[1] = game_ipcOut;
#endif

#if !defined(GAME_STEINSGATEVSO)
#if defined(GAME_CHAOSHEADNOAH)
    QString game = "./HEAD NOAH/";
#elif defined(GAME_STEINSGATE)
    QString game = "./GATE/";
#elif defined(GAME_ROBOTICSNOTESELITE)
    QString game = "./NOTES ELITE/";
#elif defined(GAME_CHAOSCHILD)
    QString game = "./CHILD/";
#elif defined(GAME_STEINSGATE0)
    QString game = "./GATE 0/";
#elif defined(GAME_ROBOTICSNOTESDASH)
    QString game = "./NOTES DaSH/";
#else
    QString game = "./";
#endif
    QString dxvkFiles[6] = {"d3d9",      "d3d10", "d3d10_1",
                            "d3d10core", "d3d11", "dxgi"};
#if defined(GAME_STEAM)
    if (rbApp->patchConfig()->enableDxvk) {
        for (int i = 0; i < 6; i++) {
            QString path = game % dxvkFiles[i];
            QFile file(path);
            file.rename(path % ".dll");
        }
    } else {
        for (int i = 0; i < 6; i++) {
            QString path = game % dxvkFiles[i];
            QFile file(path % ".dll");
            file.rename(path);
        }
    }
#else
    // Making it work with both GOG ways of installation
    if (rbApp->patchConfig()->enableDxvk) {
        for (int i = 0; i < 6; i++) {
            QString pathWithSemicolon = game % dxvkFiles[i];
            QFile fileWithSemicolon(pathWithSemicolon);
            fileWithSemicolon.rename(pathWithSemicolon % ".dll");
            QString path = "./" % dxvkFiles[i];
            QFile file(path);
            file.rename(path % ".dll");
        }
    } else {
        for (int i = 0; i < 6; i++) {
            QString pathWithSemicolon = game % dxvkFiles[i];
            QFile fileWithSemicolon(pathWithSemicolon % ".dll");
            fileWithSemicolon.rename(pathWithSemicolon);
            QString path = "./" % dxvkFiles[i];
            QFile file(path % ".dll");
            file.rename(path);
        }
    }
#endif
#endif

#if defined(GAME_ANONYMOUSCODE)
    if (rbApp->patchConfig()->voiceSubs) {
        QFile file("./c0_subs_disabled.nut");
        file.rename("./c0_subs.nut");
        file.remove("./c0_subs_disabled.nut");
    } else {
        QFile file("./c0_subs.nut");
        file.rename("./c0_subs_disabled.nut");
        file.remove("./c0_subs.nut");
    }
    if (rbApp->patchConfig()->displayMode == "windowed") {
        if (rbApp->patchConfig()->resolution == "1080") {
            QProcess::startDetached(
                "cmd", QStringList()
                           << "/c"
                           << "start " + game_LaunchCommand +
                                  " --window-size=1920,1080 --fullscreen=0");
        } else if (rbApp->patchConfig()->resolution == "720") {
            QProcess::startDetached(
                "cmd", QStringList()
                           << "/c"
                           << "start " + game_LaunchCommand +
                                  " --window-size=1280,720 --fullscreen=0");

        } else {
            QProcess::startDetached(
                "cmd", QStringList()
                           << "/c"
                           << "start " + game_LaunchCommand +
                                  " --window-size=1024,576 --fullscreen=0");
        }
    } else {
        if (rbApp->patchConfig()->resolution == "1080") {
            QProcess::startDetached(
                "cmd", QStringList()
                           << "/c"
                           << "start " + game_LaunchCommand +
                                  " --window-size=1920,1080 --fullscreen=1");
        } else if (rbApp->patchConfig()->resolution == "720") {
            QProcess::startDetached(
                "cmd", QStringList()
                           << "/c"
                           << "start " + game_LaunchCommand +
                                  " --window-size=1280,720 --fullscreen=1");

        } else {
            QProcess::startDetached(
                "cmd", QStringList()
                           << "/c"
                           << "start " + game_LaunchCommand +
                                  " --window-size=1024,576 --fullscreen=1");
        }
    }
#else
    // allow URLs
    QProcess::startDetached(
        "cmd", QStringList() << "/c" << "start " + game_LaunchCommand);
#endif

#if defined(IPC_ENABLED)
    UnmapViewOfFile((LPCVOID)ipc);
    CloseHandle(ipcFile);
    QApplication::quit();
#else
    QApplication::quit();
#endif
}

void LauncherWindow::saveChanges() {
    if (_allSettingsMode) {
#if !defined(GAME_STEINSGATEVSO)
        _generalTab->setConfig();
        _dxvkTab->setConfig();
#endif
#if !defined(GAME_CHAOSHEADNOAH) && !defined(GAME_STEINSGATEELITE) &&        \
    !defined(GAME_STEINSGATEVSO) && !defined(GAME_ROBOTICSNOTESELITE) &&     \
    !defined(GAME_ROBOTICSNOTESDASH) && !defined(GAME_ANONYMOUSCODE)
        _controllerTab->setConfig();
#endif
    } else {
        ui->miniSettingsWidget->setConfig();
    }
    rbApp->gameConfig()->save();
    rbApp->patchConfig()->save();
    if (rbApp->controllerManager()->activeController() != nullptr) {
        rbApp->controllerManager()->activeController()->config()->save();
    }
}

void LauncherWindow::reloadData() {
    if (_allSettingsMode) {
#if !defined(GAME_CHAOSHEADNOAH) && !defined(GAME_STEINSGATEELITE) &&        \
    !defined(GAME_STEINSGATEVSO) && !defined(GAME_ROBOTICSNOTESELITE) &&     \
    !defined(GAME_ROBOTICSNOTESDASH) && !defined(GAME_ANONYMOUSCODE)
        if (rbApp->controllerManager()->activeController() != nullptr) {
            _controllerTab->reloadData();
        }
#endif
#if !defined(GAME_STEINSGATEVSO)
        _generalTab->reloadData();
        _dxvkTab->reloadData();
#endif
    } else {
        ui->miniSettingsWidget->reloadData();
    }
}

void LauncherWindow::resetToDefaults() {
    rbApp->gameConfig()->loadDefaults();
    rbApp->patchConfig()->loadDefaults();
    if (rbApp->controllerManager()->activeController() != nullptr) {
        rbApp->controllerManager()
            ->activeController()
            ->config()
            ->loadDefaults();
    }
    reloadData();
}

void LauncherWindow::showMiniLayout() {
    ui->tabWidget->setVisible(false);
    ui->miniSettingsWidget->setVisible(true);
    ui->bottomButtonsWidget->setVisible(false);
    ui->miniBottomSpacer->changeSize(1, 1, QSizePolicy::Minimum,
                                     QSizePolicy::Expanding);
    ui->miniBottomSpacer->invalidate();

    ui->toggleSettingsButton->setText("More Settings");

    setFixedSize(500, 320);
    adjustSize();
}

void LauncherWindow::showFullLayout() {
    ui->tabWidget->setVisible(true);
    ui->miniSettingsWidget->setVisible(false);
    ui->bottomButtonsWidget->setVisible(true);
    ui->miniBottomSpacer->changeSize(0, 0, QSizePolicy::Fixed,
                                     QSizePolicy::Fixed);
    ui->miniBottomSpacer->invalidate();

    ui->toggleSettingsButton->setText("Less Settings");

    setFixedSize(500, 600);
    adjustSize();
}

void LauncherWindow::toggleSettings() {
    saveChanges();
    if (_allSettingsMode) {
        _allSettingsMode = false;
        showMiniLayout();
    } else {
        _allSettingsMode = true;
        showFullLayout();
    }
    rbApp->patchConfig()->showAllSettings = _allSettingsMode;
    rbApp->patchConfig()->save();
    reloadData();
}

void LauncherWindow::startUpdateCheck() {
#if defined(GAME_STEINSGATEVSO) || defined(GAME_ANONYMOUSCODE)
    QFile versioninfoFile("./versioninfo.json");
#else
    QFile versioninfoFile("languagebarrier/versioninfo.json");
#endif
    if (!versioninfoFile.open(QIODevice::ReadOnly)) {
        updateCheckFailed("Couldn't open update info");
        return;
    }
    QByteArray versioninfoData = versioninfoFile.readAll();
    QJsonDocument versioninfo = QJsonDocument::fromJson(versioninfoData);
    if (versioninfo.isNull()) {
        updateCheckFailed("Couldn't parse update info");
        return;
    }
    QJsonObject const &obj = versioninfo.object();
    if (obj["intVersion"].type() != QJsonValue::Double ||
        obj["channel"].type() != QJsonValue::String ||
        obj["updateCheckUrl"].type() != QJsonValue::String) {
        updateCheckFailed("Update info is missing required data");
        return;
    }
    _runningIntVersion = obj["intVersion"].toInt();
    _updateChannel = obj["channel"].toString();
    QString updateCheckUrl = obj["updateCheckUrl"].toString();

    ui->updateCheckLabel->setText("(checking for updates...)");

    QFutureWatcher<UpdateCheckReply> *watcher =
        new QFutureWatcher<UpdateCheckReply>(this);
    QFuture<UpdateCheckReply> future = QtConcurrent::run([=]() {
        CURL *curl = curl_easy_init();
        MemoryStruct chunk;
        chunk.memory = (char *)malloc(1);
        chunk.size = 0;
        curl_easy_setopt(curl, CURLOPT_URL,
                         updateCheckUrl.toUtf8().constData());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        UpdateCheckReply result;
        result.responseCode = curl_easy_perform(curl);
        if (result.responseCode == CURLE_OK) {
            result.responseBody = QByteArray(chunk.memory, chunk.size);
        } else {
            result.responseBody =
                QByteArray(curl_easy_strerror(result.responseCode));
        }
        free(chunk.memory);
        // leak the handle because freeing it apparently crashes on wine
        // curl_easy_cleanup(curl);
        return result;
    });
    connect(watcher, &QFutureWatcher<UpdateCheckReply>::finished, [=]() {
        updateCheckReplyReceived(watcher->result());
        delete watcher;
    });
    watcher->setFuture(future);
}

void LauncherWindow::updateCheckFailed(const QString &error) {
    if (!error.isEmpty()) QMessageBox::critical(this, "Launcher error", error);
    ui->updateCheckLabel->setText("(update check failed)");
}

void LauncherWindow::updateCheckReplyReceived(const UpdateCheckReply &reply) {
    if (reply.responseCode != CURLE_OK) {
        updateCheckFailed(QString("Update check request error: %1")
                              .arg(QString(reply.responseBody)));
        return;
    }
    QJsonDocument update = QJsonDocument::fromJson(reply.responseBody);
    if (!update.isObject()) {
        updateCheckFailed(
            "Update check got malformed response (not a JSON object)");
        return;
    }
    const QJsonObject &obj = update.object();
    if (obj["channels"].type() != QJsonValue::Object ||
        obj["channels"].toObject()[_updateChannel].type() !=
            QJsonValue::Object) {
        updateCheckFailed("Update check failed (no such channel)");
        return;
    }
    const QJsonObject &channel =
        update.object()["channels"].toObject()[_updateChannel].toObject();
    if (channel["intVersion"].type() != QJsonValue::Double) {
        updateCheckFailed(
            "Update check got malformed response (version is not a number)");
        return;
    } else {
        int latestVersion = channel["intVersion"].toInt();
        if (latestVersion > _runningIntVersion) {
            ui->updateCheckLabel->setText("(update available!)");
            QMessageBox::StandardButton reply = QMessageBox::question(
                this, "New version available",
                "A newer version of the patch is available.\n"
                "Close launcher and open download page?",
                QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                QDesktopServices::openUrl(QUrl(game_ReleaseUrl));
                cancelRequested();
            }
        } else {
            ui->updateCheckLabel->setText("(latest version)");
        }
    }
}