#pragma once

#include <QObject>

class GameConfig : public QObject {
    Q_OBJECT

   public:
    enum class MovieQuality : uint32_t { High1080p = 0, Low720p = 1, Num };
    Q_ENUM(MovieQuality)
    enum class DisplayMode : uint32_t { Windowed = 0, Fullscreen = 1, Num };
    Q_ENUM(DisplayMode)
    enum class Resolution : uint32_t {
        Res576p = 0,
        Res720p = 1,
        Res1080p = 2,
        Num
    };

    enum class FontSize : uint32_t {
        Size100 = 100,
        Size125 = 125,
        Size150 = 150,
        Size175 = 175,
        Size200 = 200

    };

    Q_ENUM(Resolution)

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
    MovieQuality movieQuality;
    QString controllerGuid = QString();

   private:
    QString _path;
};