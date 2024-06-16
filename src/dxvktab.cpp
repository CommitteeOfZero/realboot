#include "dxvktab.h"
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

DxvkTab::DxvkTab(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(8);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    mainLayout->addSpacing(8);

    QLabel *enableDxvkLabel = new QLabel(this);
    enableDxvkLabel->setText("<b>Common graphic errors resolution:</b>");
    mainLayout->addWidget(enableDxvkLabel);
    _enableDxvkCb = new QCheckBox("Enable DXVK", this);
    mainLayout->addWidget(_enableDxvkCb);
    mainLayout->addStretch(1);

    reloadData();
}
void DxvkTab::setConfig() {
    rbApp->patchConfig()->enableDxvk = _enableDxvkCb->isChecked();
}

void DxvkTab::reloadData() {
    _enableDxvkCb->setChecked(rbApp->patchConfig()->enableDxvk);
}
