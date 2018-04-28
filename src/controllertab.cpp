#include "controllertab.h"
#include "launcherapplication.h"
#include "controllermanager.h"
#include "gameconfig.h"
#include "globals.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMap>

#include <QMetaEnum>

static QMap<ControllerConfig::Bind, QString> bindLabelTexts{
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
    if (rbApp->controllerManager()->activeController() == nullptr ||
        !rbApp->controllerManager()->activeController()->isXinput()) {
        return QString("%1").arg(((int)btn) + 1);
    }
    QMetaEnum metaEnum = QMetaEnum::fromType<ControllerConfig::Button>();
    return QString(metaEnum.valueToKey((int)btn));
}

class ControllerTab::BtnLineEdit : public QLineEdit {
   public:
    explicit BtnLineEdit(QWidget *parent = 0) : QLineEdit(parent) {
        setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setFixedSize(56, 24);
        setReadOnly(true);
    }
    ~BtnLineEdit() {}
};

class ControllerTab::BtnRow : public QWidget {
   public:
    explicit BtnRow(ControllerConfig::Bind bind, QWidget *parent = 0)
        : QWidget(parent) {
        _bind = bind;
        QHBoxLayout *lay = new QHBoxLayout;
        lay->setSpacing(0);
        lay->setMargin(0);
        lay->setAlignment(Qt::AlignVCenter);
        QLabel *lbl = new QLabel(bindLabelTexts[bind]);
        lay->addWidget(lbl);
        lay->addStretch(1);
        _le = new BtnLineEdit(this);
        lay->addWidget(_le);
        setLayout(lay);
    }
    ~BtnRow() {}

    ControllerConfig::Bind bind() const { return _bind; }
    BtnLineEdit *le() { return _le; }

   private:
    ControllerConfig::Bind _bind;
    BtnLineEdit *_le;
};

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

void ControllerTab::setConfig() {
    if (rbApp->controllerManager()->activeController() != nullptr) {
        rbApp->gameConfig()->controllerGuid =
            rbApp->controllerManager()->activeController()->guid();
    }
    // rest is already set above
}

void ControllerTab::resetButtonClicked() {}