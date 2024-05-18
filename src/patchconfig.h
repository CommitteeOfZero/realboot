#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QJsonObject>

class PatchConfig : public QObject {
    Q_OBJECT

   public:
    enum class SongSubs : uint32_t {
        Off = 0,
        All = 1,
        KaraOnly = 2,
        TLOnly = 3,
        Num
    };
    Q_ENUM(SongSubs)
    static QStringList SongSubsOptions;

    explicit PatchConfig(QObject* parent = 0);
    ~PatchConfig() {}

    void save();
    void loadDefaults();

    bool showAllSettings;
    bool controllerEnabled;
    bool hqFmvAudio;
    bool hasConsistency = false;
    bool consistency;
    bool honorifics;
    bool hasHonorifics = false;
    bool hasrineBlackNames = false;
    bool rineBlackNames;
    bool improveDialogueOutlines;
    bool hasCosplayPatch = true;
    bool cosplayPatch;
    bool rneMouseControls;
    bool scrollDownToAdvanceText;
    bool disableScrollDownToCloseBacklog;
    bool swimsuitPatch;
    int fontSize;
    QString fontPath;
    QString karaokeSubs;
    QString selectedController;

   private:
    QString _path;

    void migrate(QJsonObject& conf);
};