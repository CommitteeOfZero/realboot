#include "patchconfig.h"
#include "globals.h"
#include "launcherapplication.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QDir>

QStringList PatchConfig::SongSubsOptions = QStringList() << "off"
                                                         << "all"
                                                         << "karaonly"
                                                         << "tlonly";

PatchConfig::PatchConfig(QObject* parent) : QObject(parent) {
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

        migrate(inJson);

        if (inJson["showAllSettings"].isBool())
            showAllSettings = inJson["showAllSettings"].toBool();
        if (inJson["controllerEnabled"].isBool())
            controllerEnabled = inJson["controllerEnabled"].toBool();
        if (inJson["hqFmvAudio"].isBool())
            hqFmvAudio = inJson["hqFmvAudio"].toBool();
        if (inJson["consistency"].isBool())
            consistency = inJson["consistency"].toBool();
        if (inJson["improveDialogueOutlines"].isBool())
            improveDialogueOutlines =
                inJson["improveDialogueOutlines"].toBool();
        if (inJson["karaokeSubs"].isString()) {
            QString karaokeSubs_ = inJson["karaokeSubs"].toString();
            if (PatchConfig::SongSubsOptions.contains(karaokeSubs_))
                karaokeSubs = karaokeSubs_;
        }
        if (inJson["selectedController"].isString())
            selectedController = inJson["selectedController"].toString();
    }
}

void PatchConfig::save() {
    QDir dir(rbApp->patchConfigDirectory());
    if (!dir.exists()) dir.mkpath(".");

    QFile outFile(_path);
    if (!outFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(0, "Launcher error",
                              QString("Couldn't save %1").arg(_path));
        QApplication::quit();
    }

    QJsonObject outJson;
    outJson["__schema_version"] = 4;
    outJson["showAllSettings"] = showAllSettings;
    outJson["controllerEnabled"] = controllerEnabled;
    outJson["hqFmvAudio"] = hqFmvAudio;
    outJson["consistency"] = consistency;
    outJson["improveDialogueOutlines"] = improveDialogueOutlines;
    outJson["karaokeSubs"] = karaokeSubs;
    outJson["selectedController"] = selectedController;

    QJsonDocument outJsonDocument(outJson);
    outFile.write(outJsonDocument.toJson());
}

void PatchConfig::loadDefaults() {
    showAllSettings = false;
    controllerEnabled = true;
    hqFmvAudio = true;
    consistency = true;
    improveDialogueOutlines = true;
    karaokeSubs = "all";
    selectedController = "";
}

void PatchConfig::migrate(QJsonObject& conf) {
    int oldVersion = conf["__schema_version"].toInt();
    if (oldVersion < 3) {
        if (conf["karaokeSubs"].isString() &&
            conf["karaokeSubs"].toString() == "lowQuality") {
            conf["karaokeSubs"] = "all";
        }
    }
    if (oldVersion < 4) {
        conf["controllerEnabled"] = true;
        conf["selectedController"] = "";
    }
}