#include "troubleshootingtab.h"
#include "launcherapplication.h"
#include "gameconfig.h"
#include "patchconfig.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QCryptographicHash>
#include <QClipboard>
#include <QDir>
#include <QSizePolicy>

TroubleshootingTab::TroubleshootingTab(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(8);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    mainLayout->addSpacing(8);

    QPushButton *openGameFolderButton =
        new QPushButton("Open game installation folder", this);
    openGameFolderButton->setFixedWidth(350);
    connect(openGameFolderButton, &QPushButton::clicked,
            []() { QDesktopServices::openUrl(QUrl::fromLocalFile(".")); });
    mainLayout->addWidget(openGameFolderButton);
    mainLayout->setAlignment(openGameFolderButton, Qt::AlignHCenter);
    QPushButton *openSaveFolderButton =
        new QPushButton("Open game save folder", this);
    openSaveFolderButton->setFixedWidth(350);
    connect(openSaveFolderButton, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(
            QUrl::fromLocalFile(rbApp->gameConfigDirectory()));
    });
    mainLayout->addWidget(openSaveFolderButton);
    mainLayout->setAlignment(openSaveFolderButton, Qt::AlignHCenter);
    QPushButton *openPatchConfigFolderButton =
        new QPushButton("Open patch config folder", this);
    openPatchConfigFolderButton->setFixedWidth(350);
    connect(openPatchConfigFolderButton, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(
            QUrl::fromLocalFile(rbApp->patchConfigDirectory()));
    });
    mainLayout->addWidget(openPatchConfigFolderButton);
    mainLayout->setAlignment(openPatchConfigFolderButton, Qt::AlignHCenter);

    mainLayout->addSpacing(12);

    _infoTe = new QPlainTextEdit(this);

    QPushButton *copyInfoButton =
        new QPushButton("Copy below info to clipboard", this);
    copyInfoButton->setFixedWidth(230);
    connect(copyInfoButton, &QPushButton::clicked, this,
            &TroubleshootingTab::copyInfo);
    mainLayout->addWidget(copyInfoButton);

    QString troubleshootingData = "";
    troubleshootingData +=
        "Operating System: " + QSysInfo::prettyProductName() + " (" +
        QSysInfo::kernelVersion() + ")\n";
    troubleshootingData += "Game path: " + QDir(".").canonicalPath() + "\n";
    troubleshootingData +=
        "Save path: " + QDir(rbApp->gameConfigDirectory()).canonicalPath() +
        "\n";
    troubleshootingData += "Patch config path: " +
                           QDir(rbApp->patchConfigDirectory()).canonicalPath() +
                           "\n";
    troubleshootingData += "Game.exe hash: " + md5sum("Game.exe") + "\n";
    troubleshootingData +=
        "enscript.mpk hash: " + md5sum("languagebarrier/enscript.mpk") + "\n";
    troubleshootingData +=
        "enscript_c.mpk hash: " + md5sum("languagebarrier/enscript_c.mpk") +
        "\n";

    troubleshootingData += "\n";
    troubleshootingData += "------------------------------------------------\n";
    troubleshootingData += "\n";

    QFile languagebarrierLog("languagebarrier/log.txt");
    if (!languagebarrierLog.open(QIODevice::ReadOnly)) {
        troubleshootingData += "Couldn't read LanguageBarrier log";
    } else {
        troubleshootingData += languagebarrierLog.readAll();
    }

    _infoTe->setReadOnly(true);
    _infoTe->setPlainText(troubleshootingData);
    mainLayout->addWidget(_infoTe);

    mainLayout->addStretch(1);
}

void TroubleshootingTab::copyInfo() {
    rbApp->clipboard()->setText(_infoTe->toPlainText());
}

QString TroubleshootingTab::md5sum(const QString &filePath) {
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly)) {
        return "Couldn't open file";
    }
    QCryptographicHash hash(QCryptographicHash::Md5);
    if (!hash.addData(&f)) {
        return "Couldn't read data";
    }

    return QString(hash.result().toHex());  // toHex() is lowercase
}