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

    bool hqFmvAudio = true;
    bool improveDialogueOutlines = true;
    QString karaokeSubs = "lowQuality";

   private:
    QString _path;
};