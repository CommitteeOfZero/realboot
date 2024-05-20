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
    bool hasCosplayPatch = false;
    bool cosplayPatch;
    bool rneMouseControls;
    bool scrollDownToAdvanceText;
    bool disableScrollDownToCloseBacklog;
    bool swimsuitPatch;
    int fontSize;
    QString fontPath;
#if !defined(GAME_ANONYMOUSCODE)
    QString karaokeSubs;
#endif
    QString selectedController;
#if defined(GAME_ANONYMOUSCODE)
    QString language;
#endif
    QString displayMode;
    QString resolution;
    bool voiceSubs;

   private:
    QString _path;

    void migrate(QJsonObject& conf);
};