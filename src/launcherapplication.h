#pragma once

#include <QApplication>
#include <QCoreApplication>
#include <QStyle>

class LauncherWindow;
class GameConfig;
class PatchConfig;
class ControllerManager;

#define rbApp static_cast<::LauncherApplication*>(QCoreApplication::instance())

class LauncherApplication : public QApplication {
    Q_OBJECT

   public:
    explicit LauncherApplication(int& argc, char** argv);
    ~LauncherApplication();

    void showWindow();

    GameConfig* gameConfig() { return gc; }
    PatchConfig* patchConfig() { return pc; }
    LauncherWindow* window() { return w; }
    ControllerManager* controllerManager() { return cm; }

    QString gameConfigDirectory() const;
    QString patchConfigDirectory() const;

   private:
    LauncherWindow* w;
    GameConfig* gc;
    PatchConfig* pc;
    ControllerManager* cm;
};