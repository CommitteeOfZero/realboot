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

LauncherWindow::LauncherWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LauncherWindow) {
    setWindowFlag(Qt::FramelessWindowHint);
    ui->setupUi(this);

    if (QFileInfo(":/assets/start_button.png").exists()) {
        ui->startButton->setIcon(QIcon(":/assets/start_button.png"));
        ui->startButton->setIconSize(QSize(32, 32));
        ui->startButton->setProperty("hasIcon", true);
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

    QFile patchdefFile("languagebarrier/patchdef.json");
    if (!patchdefFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Launcher error",
                              "Couldn't open patch definition");
        QApplication::quit();
    }
    QByteArray patchdefData = patchdefFile.readAll();
    QJsonDocument patchdef = QJsonDocument::fromJson(patchdefData);
    QString version = patchdef.object()["patchVersion"].toString();

    ui->versionLabel->setTextFormat(Qt::RichText);
    ui->versionLabel->setOpenExternalLinks(true);
    ui->versionLabel->setText(
        QString("<a href='%1'><span style='font-weight: 600; text-decoration: "
                "underline; color: #fff'>Version:</span></a> %2")
            .arg(game_ReleaseUrl, version));

    _generalTab = new GeneralTab(this);
    ui->tabWidget->addTab(_generalTab, "General");
    _controllerTab = new ControllerTab(this);
    ui->tabWidget->addTab(_controllerTab, "Controller");
    TroubleshootingTab *troubleshootingTab = new TroubleshootingTab(this);
    ui->tabWidget->addTab(troubleshootingTab, "Troubleshooting");

    _allSettingsMode = rbApp->patchConfig()->showAllSettings;
    if (_allSettingsMode) {
        showFullLayout();
    } else {
        showMiniLayout();
    }

    startUpdateCheck();
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

    volatile void *ipc;
    HANDLE ipcFile;
    if (game_ipcEnabled) {
        ipcFile = CreateFileMappingW(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0,
                                     8, game_ipcName);
        ipc = MapViewOfFile(ipcFile, FILE_MAP_ALL_ACCESS, 0, 0, 8);
        ((volatile uint32_t *)ipc)[1] = game_ipcOut;
    }

    // allow URLs
    QProcess::startDetached("cmd", QStringList()
                                       << "/c"
                                       << "start " + game_LaunchCommand);

    if (game_ipcEnabled) {
        QElapsedTimer timer;
        bool started = false;
        timer.start();
        while (timer.elapsed() < 5000) {
            if (((volatile uint32_t *)ipc)[0] == game_ipcIn) {
                started = true;
                break;
            }
            Sleep(1);
        }
        UnmapViewOfFile((LPCVOID)ipc);
        CloseHandle(ipcFile);
        if (started) {
            QApplication::quit();
        } else {
            QMessageBox::critical(this, "Launcher error",
                                  "Couldn't start game");
            setEnabled(true);
        }
    } else {
        QApplication::quit();
    }
}

void LauncherWindow::saveChanges() {
    if (_allSettingsMode) {
        _generalTab->setConfig();
        _controllerTab->setConfig();
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
        if (rbApp->controllerManager()->activeController() != nullptr) {
            _controllerTab->reloadData();
        }
        _generalTab->reloadData();
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
    QFile versioninfoFile("languagebarrier/versioninfo.json");
    if (!versioninfoFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Launcher error",
                              "Couldn't open update info");
        ui->updateCheckLabel->setText("(update check failed)");
        return;
    }
    QByteArray versioninfoData = versioninfoFile.readAll();
    QJsonDocument versioninfo = QJsonDocument::fromJson(versioninfoData);
    _runningIntVersion = versioninfo.object()["intVersion"].toInt();
    _updateChannel = versioninfo.object()["channel"].toString();
    QUrl updateCheckUrl =
        QUrl(versioninfo.object()["updateCheckUrl"].toString());

    ui->updateCheckLabel->setText("(checking for updates...)");

    connect(&_qnam, &QNetworkAccessManager::finished, this,
            &LauncherWindow::updateCheckReplyReceived);
    _qnam.get(QNetworkRequest(updateCheckUrl));
}

void LauncherWindow::updateCheckReplyReceived(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->error();
        reply->deleteLater();
        ui->updateCheckLabel->setText("(update check failed)");
        return;
    }
    QByteArray updateData = reply->readAll();
    reply->deleteLater();
    QJsonDocument update = QJsonDocument::fromJson(updateData);
    if (!update.object().contains("channels") ||
        !update.object()["channels"].toObject().contains(_updateChannel) ||
        !update.object()["channels"]
             .toObject()[_updateChannel]
             .toObject()
             .contains("intVersion")) {
        ui->updateCheckLabel->setText("(update check failed)");
        return;
    } else {
        int latestVersion = update.object()["channels"]
                                .toObject()[_updateChannel]
                                .toObject()["intVersion"]
                                .toInt();
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