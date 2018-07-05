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
    bool hqFmvAudio;
    bool consistency;
    bool improveDialogueOutlines;
    QString karaokeSubs;

   private:
    QString _path;

    void migrate(QJsonObject& conf);
};