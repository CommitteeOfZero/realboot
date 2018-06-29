#pragma once

#include <QObject>
#include <QString>
#include <QStringList>

class PatchConfig : public QObject {
    Q_OBJECT

   public:
    static QStringList KaraokeSubsOptions;

    explicit PatchConfig(QObject *parent = 0);
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
};