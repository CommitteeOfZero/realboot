#include "controllertab.h"
#include "launcherapplication.h"
#include "controllermanager.h"
#include "gameconfig.h"
#include "globals.h"
#include "launcherwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMap>

#include <QMetaEnum>

QMap<ControllerConfig::Bind, QString> bindLabelTexts{
    {ControllerConfig::Bind::Enter, "Enter / Send message"},
    {ControllerConfig::Bind::Cancel, "Cancel / Hide text"},
    {ControllerConfig::Bind::AutoMode, "Auto mode"},
    {ControllerConfig::Bind::Skip, "Skip / Turn page (Right)"},
    {ControllerConfig::Bind::SystemMenu, "System menu"},
    {ControllerConfig::Bind::Tips, "Tips"},
    {ControllerConfig::Bind::ForceSkip, "Force skip / Turn page (Left)"},
    {ControllerConfig::Bind::Backlog, "Backlog"},
    {ControllerConfig::Bind::QuickSave, "Quick save"},
    {ControllerConfig::Bind::Custom1, game_Custom1ButtonLabel},
    {ControllerConfig::Bind::Custom2, game_Custom2ButtonLabel}};

QString buttonToText(ControllerConfig::Button btn) {
    if (btn == ControllerConfig::Button::Invalid) return "";

    if (rbApp->controllerManager()->activeController() == nullptr ||
        !rbApp->controllerManager()->activeController()->isXinput()) {
        return QString("%1").arg(((int)btn) + 1);
    }
    QMetaEnum metaEnum = QMetaEnum::fromType<ControllerConfig::Button>();
    return QString(metaEnum.valueToKey((int)btn));
}

ControllerTab::ControllerTab(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    QHBoxLayout *controllerRow = new QHBoxLayout(this);
    controllerRow->setSpacing(8);
    controllerRow->setMargin(0);
    controllerRow->setAlignment(Qt::AlignVCenter);
    controllerRow->addStretch(1);
    QLabel *controllerLabel = new QLabel("Controller:", this);
    controllerRow->addWidget(controllerLabel);
    if (rbApp->controllerManager()->controllers().empty()) {
        QLabel *noControllersLabel = new QLabel("No controllers found.", this);
        controllerRow->addWidget(noControllersLabel);
    } else {
        _controllerBox = new QComboBox(this);
        _controllerBox->setEditable(false);
        for (DinputController *controller :
             rbApp->controllerManager()->controllers()) {
            _controllerBox->addItem(controller->deviceName(),
                                    QVariant(controller->guid()));
        }
        connect(_controllerBox,
                QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                &ControllerTab::controllerSelected);
        controllerRow->addWidget(_controllerBox);

        _resetButton = new QPushButton("Reset bindings", this);
        controllerRow->addWidget(_resetButton);
    }
    controllerRow->addStretch(1);
    mainLayout->addLayout(controllerRow);

    if (!rbApp->controllerManager()->controllers().empty()) {
        mainLayout->addSpacing(12);

        QSizePolicy spCol(QSizePolicy::Preferred, QSizePolicy::Preferred);
        spCol.setHorizontalStretch(1);

        QHBoxLayout *btnsLayout = new QHBoxLayout(this);
        btnsLayout->setSpacing(12);
        btnsLayout->setMargin(0);
        mainLayout->addLayout(btnsLayout);

        int rightNum = (int)ControllerConfig::Bind::Num / 2;
        int leftNum = (int)ControllerConfig::Bind::Num - rightNum;

        QWidget *leftCol = new QWidget(this);
        leftCol->setSizePolicy(spCol);
        QVBoxLayout *leftColLayout = new QVBoxLayout(leftCol);
        leftColLayout->setSpacing(12);
        leftColLayout->setMargin(0);
        leftCol->setLayout(leftColLayout);

        for (int i = 0; i < leftNum; i++) {
            _binds[i] = new BtnRow((ControllerConfig::Bind)i, leftCol);
            leftColLayout->addWidget(_binds[i]);
        }

        leftColLayout->addStretch(1);

        QWidget *rightCol = new QWidget(this);
        rightCol->setSizePolicy(spCol);
        QVBoxLayout *rightColLayout = new QVBoxLayout(rightCol);
        rightColLayout->setSpacing(12);
        rightColLayout->setMargin(0);
        rightCol->setLayout(rightColLayout);

        for (int i = leftNum; i < leftNum + rightNum; i++) {
            _binds[i] = new BtnRow((ControllerConfig::Bind)i, rightCol);
            rightColLayout->addWidget(_binds[i]);
        }

        rightColLayout->addStretch(1);

        btnsLayout->addWidget(leftCol);
        btnsLayout->addWidget(rightCol);
    }

    mainLayout->addStretch(1);
}

void ControllerTab::showEvent(QShowEvent *e) {
    QWidget::showEvent(e);
    if (_firstShowCaught) return;
    _firstShowCaught = true;

    // We need the native window to exist for startTracking()
    // So we can't track active controller changes in the constructor
    connect(rbApp->controllerManager(),
            &ControllerManager::activeControllerChanged, this,
            &ControllerTab::onActiveControllerChanged);

    if (!rbApp->controllerManager()->controllers().empty()) {
        // make sure we activate *some* controller
        // but preferably the one specified in config.dat
        int gcIndex = _controllerBox->findData(
            QVariant(rbApp->gameConfig()->controllerGuid));
        if (gcIndex > -1) {
            _controllerBox->setCurrentIndex(gcIndex);
        } else {
            _controllerBox->setCurrentIndex(0);
        }
    }
}

void ControllerTab::setConfig() {
    if (rbApp->controllerManager()->activeController() != nullptr) {
        rbApp->gameConfig()->controllerGuid =
            rbApp->controllerManager()->activeController()->guid();
    }
    // rest is already set above
}

void ControllerTab::reloadData() {
    if (rbApp->controllerManager()->activeController() != nullptr) {
        for (int i = 0; i < (int)ControllerConfig::Bind::Num; i++) {
            _binds[i]->le()->setText(buttonToText(rbApp->controllerManager()
                                                      ->activeController()
                                                      ->config()
                                                      ->binds[i]));
        }
    }
}

BtnRow *ControllerTab::findFocusedBtnRow() {
    QWidget *fw = rbApp->focusWidget();
    BtnRow *br = nullptr;
    while (fw != nullptr && (br = qobject_cast<BtnRow *>(fw)) == nullptr) {
        fw = fw->parentWidget();
    }
    return br;
}

void ControllerTab::resetButtonClicked() {
    rbApp->controllerManager()->activeController()->config()->loadDefaults();
    reloadData();
}

void ControllerTab::controllerSelected(int index) {
    rbApp->controllerManager()->setActiveController(
        _controllerBox->currentData().toString());
}

void ControllerTab::onActiveControllerChanged(DinputController *oldController,
                                              DinputController *newController) {
    if (oldController != nullptr) {
        disconnect(oldController, 0, this, 0);
        oldController->stopTracking();
    }
    if (newController != nullptr) {
        connect(newController, &DinputController::buttonPressed, this,
                &ControllerTab::onButtonPressed);
        newController->startTracking((HWND)rbApp->window()->winId());
    }
    reloadData();
}

void ControllerTab::onButtonPressed(ControllerConfig::Button button) {
    auto &confBinds =
        rbApp->controllerManager()->activeController()->config()->binds;
    for (int i = 0; i < (int)ControllerConfig::Bind::Num; i++) {
        if (confBinds[i] == button) {
            confBinds[i] = ControllerConfig::Button::Invalid;
            _binds[i]->le()->setText("");
        }
    }

    BtnRow *br = findFocusedBtnRow();
    if (br != nullptr) {
        confBinds[(int)br->bind()] = button;
        br->le()->setText(buttonToText(button));
        int nextBind = ((int)br->bind() + 1) % (int)ControllerConfig::Bind::Num;
        _binds[nextBind]->le()->setFocus();
    }
}