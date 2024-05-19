#include "controllerconfig.h"
#include "launcherapplication.h"
#include "globals.h"
#include <QFileInfo>
#include <QMessageBox>

ControllerConfig::ControllerConfig(const QString& guid, QObject* parent)
    : QObject(parent) {
#if !defined(GAME_ANONYMOUSCODE)
    _path = rbApp->gameConfigDirectory() + "/" + guid;

    loadDefaults();

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
        for (int i = 0; i < (int)Bind::Num; i++) {
            uint8_t curBtn = *pCurBtn++;
            if (curBtn < (uint8_t)Button::Num ||
                curBtn == (uint8_t)Button::Invalid)
                binds[i] = (Button)curBtn;
        }
    }
#endif
}

void ControllerConfig::loadDefaults() {
#if !defined(GAME_ANONYMOUSCODE)
    preset = Preset::Default;

    binds[(int)Bind::Enter] = Button::A;
    binds[(int)Bind::Cancel] = Button::B;
    binds[(int)Bind::AutoMode] = Button::X;
    binds[(int)Bind::Skip] = Button::RB;
    binds[(int)Bind::SystemMenu] = Button::START;
    binds[(int)Bind::Tips] = Button::BACK;
    binds[(int)Bind::ForceSkip] = Button::LB;
    binds[(int)Bind::Backlog] = Button::Y;
    binds[(int)Bind::QuickSave] = Button::RS;
    binds[(int)Bind::Custom1] = Button::LT;
    binds[(int)Bind::Custom2] = Button::RT;
#endif
}

void ControllerConfig::save() {
#if !defined(GAME_ANONYMOUSCODE)
    QFile outFile(_path);
    if (!outFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(0, "Launcher error",
                              QString("Couldn't save %1").arg(_path));
        QApplication::quit();
    }
    outFile.seek(4);
    outFile.write((const char*)&preset, 4);
    outFile.write((const char*)&binds, (qint64)Bind::Num);
#endif
#if defined(GAME_CHAOSCHILD)
    outFile.write((const char*)game_ExtraControllerData,
                  sizeof(game_ExtraControllerData));
#endif
}