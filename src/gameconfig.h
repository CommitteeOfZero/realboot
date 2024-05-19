#pragma once

#include <QObject>

class GameConfig : public QObject {
    Q_OBJECT

   public:
#ifndef GAME_ANONYMOUSCODE
    enum class MovieQuality : uint32_t{High1080p = 0, Low720p = 1, Num};
    Q_ENUM(MovieQuality)
#endif
    enum class DisplayMode : uint32_t { Windowed = 0, Fullscreen = 1, Num };
    Q_ENUM(DisplayMode)
    enum class Resolution : uint32_t {
        Res576p = 0,
        Res720p = 1,
        Res1080p = 2,
        Num
    };
    Q_ENUM(Resolution)

    enum class Language : uint32_t {
#if defined(GAME_CHAOSHEADNOAH)
        Japanese = 0,
        English = 1,
#elif defined(GAME_ANONYMOUSCODE)
        Sub = 0,
        Dub = 1,
#endif
        Num
    };
    Q_ENUM(Language)

    enum class FontSize : uint32_t {
        Size100 = 100,
        Size125 = 125,
        Size150 = 150,
        Size175 = 175,
        Size200 = 200

    };

    explicit GameConfig(QObject *parent = 0);
    ~GameConfig() {}

    void save();
    void loadDefaults();

    int width;
    int height;
    DisplayMode displayMode;
    Resolution resolution;
    int startWindowX;
    int startWindowY;
    Language language;
#ifndef GAME_ANONYMOUSCODE
    MovieQuality movieQuality;
#endif
    QString controllerGuid = QString();

   private:
    QString _path;
};