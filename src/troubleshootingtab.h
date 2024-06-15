#pragma once

#include <QWidget>
#include <QString>
#include <QPlainTextEdit>
#include <QCheckBox>

class TroubleshootingTab : public QWidget {
    Q_OBJECT

   public:
    explicit TroubleshootingTab(QWidget* parent = 0);
    ~TroubleshootingTab() {}

    void setConfig();
    void reloadData();

   private slots:
    void copyInfo();

   private:
    QString md5sum(const QString& filePath);
    QPlainTextEdit* _infoTe;
    QCheckBox* _enableDxvkCb;
};