#pragma once

#include <QMainWindow>
#include <QEvent>
#include <QCloseEvent>
#include "launcherapplication.h"

namespace Ui {
class LauncherWindow;
}

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

   private:
    Ui::LauncherWindow *ui;
};
