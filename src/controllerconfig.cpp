#include "controllerconfig.h"
#include "launcherapplication.h"
#include "globals.h"
#include <QFileInfo>
#include <QMessageBox>

ControllerConfig::ControllerConfig(const QString& guid, QObject* parent)
    : QObject(parent) {
    _path = rbApp->gameConfigDirectory() + "/" + guid;

    if (QFileInfo(_path).exists()) {
        QFile inFile(_path);
        if (!inFile.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(
                0, "Launcher error",
                QString("Couldn't read controller config %1").arg(_path));
            QApplication::quit();
        }
        QByteArray inByteArray = inFile.readAll();
        const char* data = inByteArray.constData();

        uint32_t preset_ = *(const uint32_t*)(data + 4);
        if (preset_ < (uint32_t)Preset::Num) preset = (Preset)preset_;

        const uint8_t* pCurBtn = (const uint8_t*)(data + 8);
        uint8_t curBtn = *pCurBtn++;
        if (curBtn < (uint8_t)Button::Num) enter = (Button)curBtn;
        curBtn = *pCurBtn++;
        if (curBtn < (uint8_t)Button::Num) cancel = (Button)curBtn;
        curBtn = *pCurBtn++;
        if (curBtn < (uint8_t)Button::Num) autoMode = (Button)curBtn;
        curBtn = *pCurBtn++;
        if (curBtn < (uint8_t)Button::Num) skip = (Button)curBtn;
        curBtn = *pCurBtn++;
        if (curBtn < (uint8_t)Button::Num) systemMenu = (Button)curBtn;
        curBtn = *pCurBtn++;
        if (curBtn < (uint8_t)Button::Num) tips = (Button)curBtn;
        curBtn = *pCurBtn++;
        if (curBtn < (uint8_t)Button::Num) forceSkip = (Button)curBtn;
        curBtn = *pCurBtn++;
        if (curBtn < (uint8_t)Button::Num) backlog = (Button)curBtn;
        curBtn = *pCurBtn++;
        if (curBtn < (uint8_t)Button::Num) quickSave = (Button)curBtn;
        curBtn = *pCurBtn++;
        if (curBtn < (uint8_t)Button::Num) custom1 = (Button)curBtn;
        curBtn = *pCurBtn++;
        if (curBtn < (uint8_t)Button::Num) custom2 = (Button)curBtn;
    }
}

void ControllerConfig::save() {
    QFile outFile(_path);
    if (!outFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(0, "Launcher error",
                              QString("Couldn't save %1").arg(_path));
        QApplication::quit();
    }
    outFile.seek(4);
    outFile.write((const char*)&preset, 4);
    outFile.write((const char*)&enter, 1);
    outFile.write((const char*)&cancel, 1);
    outFile.write((const char*)&autoMode, 1);
    outFile.write((const char*)&skip, 1);
    outFile.write((const char*)&systemMenu, 1);
    outFile.write((const char*)&tips, 1);
    outFile.write((const char*)&forceSkip, 1);
    outFile.write((const char*)&backlog, 1);
    outFile.write((const char*)&quickSave, 1);
    outFile.write((const char*)&custom1, 1);
    outFile.write((const char*)&custom2, 1);
    outFile.write((const char*)game_ExtraControllerData,
                  sizeof(game_ExtraControllerData));
}