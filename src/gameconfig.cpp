#include "gameconfig.h"
#include "launcherapplication.h"
#include <QFileInfo>
#include <QMessageBox>

GameConfig::GameConfig(QObject* parent) : QObject(parent) {
    _path = rbApp->gameConfigDirectory() + "/config.dat";

    if (QFileInfo(_path).exists()) {
        QFile inFile(_path);
        if (!inFile.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Launcher error",
                                  QString("Couldn't read %1").arg(_path));
            QApplication::quit();
        }
        QByteArray inByteArray = inFile.readAll();
        const char* data = inByteArray.constData();

        controllerGuid = QString::fromWCharArray((const wchar_t*)(data + 4));
        width = *(const int*)(data + 0x2C);
        height = *(const int*)(data + 0x30);
        displayMode = *(const DisplayMode*)(data + 0x34);
        resolution = *(const Resolution*)(data + 0x38);
        startWindowX = *(const int*)(data + 0x3C);
        startWindowY = *(const int*)(data + 0x40);
        movieQuality = *(const MovieQuality*)(data + 0x44);
    }
}

void GameConfig::save() {
    QFile outFile(_path);
    if (!outFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(0, "Launcher error",
                              QString("Couldn't save %1").arg(_path));
        QApplication::quit();
    }
    outFile.seek(4);
    wchar_t outGuid[40] = {0};
    controllerGuid.toWCharArray(outGuid);
    outFile.write((const char*)outGuid, 40);
    outFile.write((const char*)&width, 4);
    outFile.write((const char*)&height, 4);
    outFile.write((const char*)&displayMode, 4);
    outFile.write((const char*)&resolution, 4);
    outFile.write((const char*)&startWindowX, 4);
    outFile.write((const char*)&startWindowY, 4);
    outFile.write((const char*)&movieQuality, 4);
    outFile.seek(0x6C);  // padding
}