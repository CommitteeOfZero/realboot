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
#include <QTextStream>
#include <QProcess>

#include <windows.h>

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

    QString wine = "Not detected\n";
    typedef const char *(__cdecl * wine_info_proc)();
    wine_info_proc wine_get_version;
    wine_info_proc wine_get_build_id;
    typedef void(__cdecl * wine_get_host_version_proc)(const char **sysname,
                                                       const char **release);
    wine_get_host_version_proc wine_get_host_version;

    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    wine_get_host_version = (wine_get_host_version_proc)GetProcAddress(
        ntdll, "wine_get_host_version");
    wine_get_version =
        (wine_info_proc)GetProcAddress(ntdll, "wine_get_version");
    wine_get_build_id =
        (wine_info_proc)GetProcAddress(ntdll, "wine_get_build_id");

    if (wine_get_version != nullptr || wine_get_build_id != nullptr ||
        wine_get_host_version != nullptr) {
        wine = "Yes\n";
        if (wine_get_version != nullptr) {
            wine += "Wine version: " + QString::fromLatin1(wine_get_version()) +
                    "\n";
        }
        if (wine_get_build_id != nullptr) {
            wine +=
                "Wine build ID: " + QString::fromLatin1(wine_get_build_id()) +
                "\n";
        }
        if (wine_get_host_version != nullptr) {
            const char *sysname;
            const char *release;
            wine_get_host_version(&sysname, &release);
            wine +=
                "Wine host version sysname: " + QString::fromLatin1(sysname) +
                "\n";
            wine +=
                "Wine host version release: " + QString::fromLatin1(release) +
                "\n";
        }
    }

    troubleshootingData += "Wine: " + wine;  // no newline here!

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

    QString canLoadD3D = "No !!!";
    QString canLoadXAudio = "No !!!";

    HMODULE hD3D = LoadLibraryA("d3dx9_43.dll");
    HMODULE hXAudio = LoadLibraryA("XAudio2_7.dll");

    if (hD3D) {
        canLoadD3D = "Yes";
        FreeLibrary(hD3D);
    }
    if (hXAudio) {
        canLoadXAudio = "Yes";
        FreeLibrary(hXAudio);
    }

    troubleshootingData += "Can load d3dx9_43.dll: " + canLoadD3D + "\n";
    troubleshootingData += "Can load XAudio2_7.dll: " + canLoadXAudio + "\n";

    // This stuff isn't as easy to collect in LanguageBarrier, so we'll log it
    // persistently here.
    // Don't want to store log.txt redundantly, so we save before the cut
    // Also, kick off dxdiag, again for crash report or manual collection
    QFile launcherdiag(rbApp->patchConfigDirectory() + "/launcherdiag.txt");
    if (launcherdiag.open(QFile::WriteOnly)) {
        QTextStream launcherdiagOut(&launcherdiag);
        launcherdiagOut << troubleshootingData << endl;
    }
    QProcess::startDetached(
        "dxdiag", QStringList()
                      << "/whql:off"
                      << "/t" << rbApp->patchConfigDirectory() + "/dxdiag.txt");

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