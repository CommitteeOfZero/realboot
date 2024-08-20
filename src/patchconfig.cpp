#include "patchconfig.h"
#include "globals.h"
#include "launcherapplication.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QDir>

#if !defined(GAME_ANONYMOUSCODE)
QStringList PatchConfig::SongSubsOptions =
    QStringList() << "off" << "all" << "karaonly" << "tlonly";
#endif

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
#if !defined(GAME_CHAOSHEADNOAH) && !defined(GAME_ROBOTICSNOTESELITE) && \
    !defined(GAME_ROBOTICSNOTESDASH) !defined(GAME_ANONYMOUSCODE)
        if (inJson["hqFmvAudio"].isBool())
            hqFmvAudio = inJson["hqFmvAudio"].toBool();
        if (inJson["consistency"].isBool()) {
            hasConsistency = true;
            consistency = inJson["consistency"].toBool();
        }
        if (inJson["rineBlackNames"].isBool()) {
            hasrineBlackNames = true;
            rineBlackNames = inJson["rineBlackNames"].toBool();
        }
        if (inJson["improveDialogueOutlines"].isBool())
            improveDialogueOutlines =
                inJson["improveDialogueOutlines"].toBool();
#endif
#if defined(GAME_STEINSGATE)
        if (inJson["cosplayPatch"].isBool()) {
            cosplayPatch = inJson["cosplayPatch"].toBool();
        }
#endif
#if defined(GAME_ROBOTICSNOTESDASH)
        if (inJson["swimsuitPatch"].isBool())
            swimsuitPatch = inJson["swimsuitPatch"].toBool();
#endif
#if !defined(GAME_ANONYMOUSCODE)
        if (inJson["karaokeSubs"].isString()) {
            QString karaokeSubs_ = inJson["karaokeSubs"].toString();
            if (PatchConfig::SongSubsOptions.contains(karaokeSubs_))
                karaokeSubs = karaokeSubs_;
        }
#endif
#if !defined(GAME_CHAOSHEADNOAH) && !defined(GAME_ROBOTICSNOTESELITE) && \
    !defined(GAME_ROBOTICSNOTESDASH) !defined(GAME_ANONYMOUSCODE)
        if (inJson["selectedController"].isString())
            selectedController = inJson["selectedController"].toString();
#endif
        if (inJson["enableDxvk"].isBool())
            enableDxvk = inJson["enableDxvk"].toBool();
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
#if defined(GAME_CHAOSHEADNOAH) || defined(GAME_ROBOTICSNOTESELITE) || \
    defined(GAME_ROBOTICSNOTESDASH) || defined(GAME_ANONYMOUSCODE)
    outJson["__schema_version"] = 5;
#else
    outJson["__schema_version"] = 4;
#endif
    outJson["showAllSettings"] = showAllSettings;
#if !defined(GAME_CHAOSHEADNOAH) && !defined(GAME_ROBOTICSNOTESELITE) && \
    !defined(GAME_ROBOTICSNOTESDASH) !defined(GAME_ANONYMOUSCODE)
    outJson["controllerEnabled"] = controllerEnabled;
    outJson["hqFmvAudio"] = hqFmvAudio;
#endif
    if (hasConsistency) outJson["consistency"] = consistency;
    if (hasrineBlackNames) outJson["rineBlackNames"] = rineBlackNames;
#if !defined(GAME_CHAOSHEADNOAH) && !defined(GAME_ROBOTICSNOTESELITE) && \
    !defined(GAME_ROBOTICSNOTESDASH) !defined(GAME_ANONYMOUSCODE)
    outJson["improveDialogueOutlines"] = improveDialogueOutlines;
#endif

#if defined(GAME_STEINSGATE)
    outJson["cosplayPatch"] = cosplayPatch;
#endif
#if defined(GAME_ROBOTICSNOTESDASH)
    outJson["swimsuitPatch"] = swimsuitPatch;
#endif
#if defined(GAME_ROBOTICSNOTESELITE) || defined(GAME_ROBOTICSNOTESDASH)
    outJson["rneMouseControls"] = rneMouseControls;
    outJson["scrollDownToAdvanceText"] = scrollDownToAdvanceText;
    outJson["disableScrollDownToCloseBacklog"] =
        disableScrollDownToCloseBacklog;
#endif
#if !defined(GAME_ANONYMOUSCODE)
    outJson["karaokeSubs"] = karaokeSubs;
#endif
#if !defined(GAME_CHAOSHEADNOAH) && !defined(GAME_ROBOTICSNOTESELITE) && \
    !defined(GAME_ROBOTICSNOTESDASH) !defined(GAME_ANONYMOUSCODE)
    outJson["selectedController"] = selectedController;
#endif
    outJson["enableDxvk"] = enableDxvk;

    QJsonDocument outJsonDocument(outJson);
    outFile.write(outJsonDocument.toJson());
}

void PatchConfig::loadDefaults() {
    showAllSettings = false;
    controllerEnabled = true;
    hqFmvAudio = true;
    consistency = true;
    rneMouseControls = true;
    scrollDownToAdvanceText = false;
    disableScrollDownToCloseBacklog = false;
    improveDialogueOutlines = true;
    cosplayPatch = false;
    swimsuitPatch = false;
    karaokeSubs = "all";
    selectedController = "";
    displayMode = "windowed";
    resolution = "720";
    voiceSubs = true;
    enableDxvk = false;
}

void PatchConfig::migrate(QJsonObject& conf) {
    int oldVersion = conf["__schema_version"].toInt();
    if (oldVersion < 3) {
#if defined(GAME_ANONYMOUSCODE)
        if (conf["karaokeSubs"].isString() &&
            conf["karaokeSubs"].toString() == "lowQuality") {
            conf["karaokeSubs"] = "all";
        }
#endif
    }
    if (oldVersion < 4) {
        conf["controllerEnabled"] = true;
        conf["selectedController"] = "";
    }
}