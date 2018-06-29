#include "patchconfig.h"
#include "globals.h"
#include "launcherapplication.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

QStringList PatchConfig::KaraokeSubsOptions = QStringList() << "off"
                                                            << "lowQuality";

PatchConfig::PatchConfig(QObject *parent) : QObject(parent) {
    _path = rbApp->patchConfigDirectory() + "/config.json";

    loadDefaults();

    if (QFileInfo(_path).exists()) {
        QFile inFile(_path);
        if (!inFile.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Launcher error",
                                  QString("Couldn't read %1").arg(_path));
            QApplication::quit();
        }

        QByteArray inData = inFile.readAll();
        QJsonDocument inJsonDoc = QJsonDocument::fromJson(inData);
        if (!inJsonDoc.isObject()) return;
        QJsonObject inJson = inJsonDoc.object();

        if (inJson["showAllSettings"].isBool())
            showAllSettings = inJson["showAllSettings"].toBool();
        if (inJson["hqFmvAudio"].isBool())
            hqFmvAudio = inJson["hqFmvAudio"].toBool();
        if (inJson["consistency"].isBool())
            consistency = inJson["consistency"].toBool();
        if (inJson["improveDialogueOutlines"].isBool())
            improveDialogueOutlines =
                inJson["improveDialogueOutlines"].toBool();
        if (inJson["karaokeSubs"].isString()) {
            QString karaokeSubs_ = inJson["karaokeSubs"].toString();
            if (PatchConfig::KaraokeSubsOptions.contains(karaokeSubs_))
                karaokeSubs = karaokeSubs_;
        }
    }
}

void PatchConfig::save() {
    QFile outFile(_path);
    if (!outFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(0, "Launcher error",
                              QString("Couldn't save %1").arg(_path));
        QApplication::quit();
    }

    QJsonObject outJson;
    outJson["__schema_version"] = 2;
    outJson["showAllSettings"] = showAllSettings;
    outJson["hqFmvAudio"] = hqFmvAudio;
    outJson["consistency"] = consistency;
    outJson["improveDialogueOutlines"] = improveDialogueOutlines;
    outJson["karaokeSubs"] = karaokeSubs;

    QJsonDocument outJsonDocument(outJson);
    outFile.write(outJsonDocument.toJson());
}

void PatchConfig::loadDefaults() {
    showAllSettings = false;
    hqFmvAudio = true;
    consistency = true;
    improveDialogueOutlines = true;
    karaokeSubs = "lowQuality";
}