#include "patchconfig.h"
#include "globals.h"
#include "launcherapplication.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QDir>

QStringList PatchConfig::SongSubsOptions =
    QStringList() << "off" << "all" << "karaonly" << "tlonly";

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

#if defined(GAME_ROBOTICSNOTESELITE) || defined(GAME_ROBOTICSNOTESDASH)
        if (inJson["rneMouseControls"].isBool())
            rneMouseControls = inJson["rneMouseControls"].toBool();
        if (inJson["scrollDownToAdvanceText"].isBool())
            scrollDownToAdvanceText =
                inJson["scrollDownToAdvanceText"].toBool();
        if (inJson["disableScrollDownToCloseBacklog"].isBool())
            disableScrollDownToCloseBacklog =
                inJson["disableScrollDownToCloseBacklog"].toBool();
#endif
#if !defined(GAME_ROBOTICSNOTESELITE) && !defined(GAME_ROBOTICSNOTESDASH)
        if (inJson["hqFmvAudio"].isBool())
            hqFmvAudio = inJson["hqFmvAudio"].toBool();
        if (inJson["consistency"].isBool()) {
            hasConsistency = true;
            consistency = inJson["consistency"].toBool();
        }
        if (inJson["honorifics"].isBool()) {
            hasHonorifics = true;
            honorifics = inJson["honorifics"].toBool();
        }
        if (inJson["rineBlackNames"].isBool()) {
            hasrineBlackNames = true;
            rineBlackNames = inJson["rineBlackNames"].toBool();
        }
        if (inJson["improveDialogueOutlines"].isBool())
            improveDialogueOutlines =
                inJson["improveDialogueOutlines"].toBool();

        if (inJson["cosplayPatch"].isBool()) hasCosplayPatch = true;
        cosplayPatch = inJson["cosplayPatch"].toBool();
#endif
#if defined(GAME_ROBOTICSNOTESDASH)
        if (inJson["swimsuitPatch"].isBool())
            swimsuitPatch = inJson["swimsuitPatch"].toBool();
#endif
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
#if !defined(GAME_ROBOTICSNOTESELITE) && !defined(GAME_ROBOTICSNOTESDASH)
    outJson["controllerEnabled"] = controllerEnabled;
    outJson["hqFmvAudio"] = hqFmvAudio;
#endif
    if (hasConsistency) outJson["consistency"] = consistency;
    if (hasHonorifics) outJson["honorifics"] = honorifics;
    if (hasrineBlackNames) outJson["rineBlackNames"] = rineBlackNames;
#if !defined(GAME_ROBOTICSNOTESELITE) && !defined(GAME_ROBOTICSNOTESDASH)
    outJson["improveDialogueOutlines"] = improveDialogueOutlines;
#endif
    if (hasCosplayPatch) outJson["cosplayPatch"] = cosplayPatch;
#if defined(GAME_ROBOTICSNOTESELITE) || defined(GAME_ROBOTICSNOTESDASH)
    outJson["rneMouseControls"] = rneMouseControls;
    outJson["scrollDownToAdvanceText"] = scrollDownToAdvanceText;
    outJson["disableScrollDownToCloseBacklog"] =
        disableScrollDownToCloseBacklog;
#endif
    outJson["karaokeSubs"] = karaokeSubs;
#if !defined(GAME_ROBOTICSNOTESELITE) && !defined(GAME_ROBOTICSNOTESDASH)
    outJson["selectedController"] = selectedController;
#endif

    QJsonDocument outJsonDocument(outJson);
    outFile.write(outJsonDocument.toJson());
}

void PatchConfig::loadDefaults() {
    showAllSettings = false;
    controllerEnabled = true;
    hqFmvAudio = true;
    consistency = true;
    honorifics = true;
    rneMouseControls = true;
    scrollDownToAdvanceText = false;
    disableScrollDownToCloseBacklog = false;
    swimsuitPatch = false;
    improveDialogueOutlines = true;
    cosplayPatch = false;
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