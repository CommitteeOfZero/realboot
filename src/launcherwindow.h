#pragma once

#include <QMainWindow>
#include <QEvent>
#include <QCloseEvent>
#include "launcherapplication.h"
#include <curl/curl.h>

namespace Ui {
class LauncherWindow;
}

class GeneralTab;
class ControllerTab;
class TroubleshootingTab;
class DxvkTab;

struct UpdateCheckReply {
    CURLcode responseCode;
    QByteArray responseBody;
};

class LauncherWindow : public QMainWindow {
    Q_OBJECT
   public:
    explicit LauncherWindow(QWidget *parent = nullptr);
    ~LauncherWindow();

    void afterShow();

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
    void updateCheckReplyReceived(const UpdateCheckReply &reply);

   private:
    Ui::LauncherWindow *ui;
    GeneralTab *_generalTab;
    ControllerTab *_controllerTab;
    TroubleshootingTab *_troubleshootingTab;
    DxvkTab *_dxvkTab;

    int _runningIntVersion;
    QString _updateChannel;

    bool _allSettingsMode;

    void showMiniLayout();
    void showFullLayout();
    void updateCheckFailed(const QString &error);
    void reloadData();
    void startUpdateCheck();
};
