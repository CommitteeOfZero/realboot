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
    Q_ENUM(Resolution)

    explicit GameConfig(QObject *parent = 0);
    ~GameConfig() {}

    void save();

    int width = 1280;
    int height = 720;
    DisplayMode displayMode = DisplayMode::Windowed;
    Resolution resolution = Resolution::Res720p;
    int startWindowX = 0;
    int startWindowY = 0;
    MovieQuality movieQuality = MovieQuality::Low720p;
    QString controllerGuid = QString();

   private:
    QString _path;
};