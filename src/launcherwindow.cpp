#include "launcherwindow.h"
#include "ui_launcherwindow.h"
#include "launcherapplication.h"
#include "globals.h"

#include <QMouseEvent>
#include <QToolButton>
#include <QProcess>
#include <QElapsedTimer>
#include <QMessageBox>

LauncherWindow::LauncherWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LauncherWindow) {
    setWindowFlag(Qt::FramelessWindowHint);
    ui->setupUi(this);

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
    connect(ui->cancelButton, &QAbstractButton::clicked, this,
            &LauncherWindow::cancelRequested);

    setWindowTitle(game_LauncherTitle);
    ui->techSupportLabel->setTextFormat(Qt::RichText);
    ui->techSupportLabel->setText(
        QString("<a href='%1'><span style='font-weight: 600; text-decoration: "
                "underline; color: #fff'>Technical Support</span></a>")
            .arg(game_TechSupportUrl));
    // TODO: read version from patchdef.json
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

void LauncherWindow::cancelRequested() { QApplication::quit(); }

void LauncherWindow::startGame() {
    setEnabled(false);
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