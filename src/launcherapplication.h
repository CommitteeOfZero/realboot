#pragma once

#include <QApplication>
#include <QCoreApplication>
#include <QStyle>

class LauncherWindow;

#define rbApp static_cast<::LauncherApplication*>(QCoreApplication::instance())

class LauncherApplication : public QApplication {
    Q_OBJECT

   public:
    explicit LauncherApplication(int& argc, char** argv);
    ~LauncherApplication();

    void showWindow();

    LauncherWindow* window() { return w; }

   private:
    LauncherWindow* w;
};