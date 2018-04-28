#pragma once

#include <QMainWindow>
#include <QEvent>
#include <QCloseEvent>
#include "launcherapplication.h"

namespace Ui {
class LauncherWindow;
}

class GraphicsTab;
class ControllerTab;

class LauncherWindow : public QMainWindow {
    Q_OBJECT
   public:
    explicit LauncherWindow(QWidget *parent = nullptr);
    ~LauncherWindow();

   protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

   signals:

   public slots:

   private slots:
    void cancelRequested();
    void startGame();

   private:
    Ui::LauncherWindow *ui;
    GraphicsTab *_graphicsTab;
    ControllerTab *_controllerTab;
};
