#include "launcherwindow.h"
#include "ui_launcherwindow.h"
#include "launcherapplication.h"
#include "globals.h"

#include <QMouseEvent>
#include <QToolButton>

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

    /*connect(ui->cancelButton, &QAbstractButton::clicked, this,
            &InstallerWindow::cancelRequested);*/

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