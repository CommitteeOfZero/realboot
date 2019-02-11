#pragma once

#include "controllerconfig.h"
#include "dinputcontroller.h"
#include "btnlineedit.h"
#include "btnrow.h"

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QShowEvent>

class ControllerTab : public QWidget {
    Q_OBJECT

   public:
    explicit ControllerTab(QWidget* parent = 0);
    ~ControllerTab() {}

    void setConfig();
    void reloadData();

   private:
    QComboBox* _controllerBox;
    QPushButton* _resetButton;
    QCheckBox* _controllerCb;
    BtnRow* _binds[(int)ControllerConfig::Bind::Num];
    bool _firstShowCaught = false;
    QLabel* _axesLabel;

    BtnRow* findFocusedBtnRow();

    void showEvent(QShowEvent* e) override;

   private slots:
    void resetButtonClicked();
    void controllerSelected(int index);
    void onActiveControllerChanged(DinputController* oldController,
                                   DinputController* newController);
    void onButtonPressed(ControllerConfig::Button button);
    void updateAxesLabel();
};