#pragma once

#include <QApplication>
#include <QCoreApplication>
#include <QStyle>

class LauncherWindow;
class GameConfig;

#define rbApp static_cast<::LauncherApplication*>(QCoreApplication::instance())

class LauncherApplication : public QApplication {
    Q_OBJECT

   public:
    explicit LauncherApplication(int& argc, char** argv);
    ~LauncherApplication();

    void showWindow();

    GameConfig* gameConfig() { return gc; }
    LauncherWindow* window() { return w; }

    QString gameConfigDirectory() const;

   private:
    LauncherWindow* w;
    GameConfig* gc;
};