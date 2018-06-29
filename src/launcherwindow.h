#pragma once

#include <QMainWindow>
#include <QEvent>
#include <QCloseEvent>
#include <QtNetwork>
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
    void updateCheckReplyReceived(QNetworkReply *reply);

   private:
    Ui::LauncherWindow *ui;
    GeneralTab *_generalTab;
    ControllerTab *_controllerTab;

    QNetworkAccessManager _qnam;
    int _runningIntVersion;
    QString _updateChannel;

    bool _allSettingsMode;

    void showMiniLayout();
    void showFullLayout();
    void reloadData();
    void startUpdateCheck();
};
