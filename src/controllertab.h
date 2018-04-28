#pragma once

#include "controllerconfig.h"

#include <QWidget>
#include <QComboBox>
#include <QPushButton>

class ControllerTab : public QWidget {
    Q_OBJECT

   public:
    explicit ControllerTab(QWidget* parent = 0);
    ~ControllerTab() {}

    void setConfig();

   private:
    class BtnRow;
    class BtnLineEdit;
    QComboBox* _controllerBox;
    QPushButton* _resetButton;
    BtnRow* _binds[(int)ControllerConfig::Bind::Num];

   private slots:
    void resetButtonClicked();
};