#pragma once

#include <QMainWindow>
#include <QEvent>
#include <QCloseEvent>
#include "launcherapplication.h"

namespace Ui {
class LauncherWindow;
}

class GeneralTab;
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
    void saveChanges();
    void resetToDefaults();
    void toggleSettings();

   private:
    Ui::LauncherWindow *ui;
    GeneralTab *_generalTab;
    ControllerTab *_controllerTab;

    bool _allSettingsMode;

    void showMiniLayout();
    void showFullLayout();
    void reloadData();
};
