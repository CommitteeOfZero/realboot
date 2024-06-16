#pragma once

#include <QWidget>
#include <QString>
#include <QPlainTextEdit>
#include <QCheckBox>

class DxvkTab : public QWidget {
    Q_OBJECT

   public:
    explicit DxvkTab(QWidget* parent = 0);
    ~DxvkTab() {}

    void setConfig();
    void reloadData();

   private:
    QCheckBox* _enableDxvkCb;
};