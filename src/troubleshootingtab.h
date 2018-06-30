#pragma once

#include <QWidget>
#include <QString>
#include <QPlainTextEdit>

class TroubleshootingTab : public QWidget {
    Q_OBJECT

   public:
    explicit TroubleshootingTab(QWidget* parent = 0);
    ~TroubleshootingTab() {}

   private slots:
    void copyInfo();

   private:
    QString md5sum(const QString& filePath);
    QPlainTextEdit* _infoTe;
};