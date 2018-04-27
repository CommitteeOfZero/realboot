#include "graphicstab.h"
#include "launcherapplication.h"
#include "gameconfig.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QRadioButton>

GraphicsTab::GraphicsTab(QWidget *parent) : QWidget(parent) {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(8);
    setLayout(mainLayout);

    QSizePolicy colSp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    colSp.setHorizontalStretch(1);

    QWidget *left = new QWidget(this);
    left->setSizePolicy(colSp);
    QVBoxLayout *leftLayout = new QVBoxLayout(this);
    left->setLayout(leftLayout);
    leftLayout->setSpacing(8);
    leftLayout->setMargin(0);
    mainLayout->addWidget(left);

    QLabel *leftLabel = new QLabel(left);
    leftLabel->setText("<b>Game</b>");
    leftLayout->addWidget(leftLabel);

    _fullscreenCb = new QCheckBox("Fullscreen", left);
    leftLayout->addWidget(_fullscreenCb);

    QHBoxLayout *resolutionRow = new QHBoxLayout(left);
    resolutionRow->setSpacing(4);
    QLabel *resolutionLabel = new QLabel("Resolution:");
    resolutionRow->addWidget(resolutionLabel);
    _resolutionGroup = new QButtonGroup(left);
    QRadioButton *r576pButton = new QRadioButton("576p", left);
    _resolutionGroup->addButton(r576pButton,
                                (int)GameConfig::Resolution::Res576p);
    resolutionRow->addWidget(r576pButton);
    QRadioButton *r720pButton = new QRadioButton("720p", left);
    _resolutionGroup->addButton(r720pButton,
                                (int)GameConfig::Resolution::Res720p);
    resolutionRow->addWidget(r720pButton);
    QRadioButton *r1080pButton = new QRadioButton("1080p", left);
    _resolutionGroup->addButton(r1080pButton,
                                (int)GameConfig::Resolution::Res1080p);
    resolutionRow->addWidget(r1080pButton);
    leftLayout->addLayout(resolutionRow);

    QWidget *right = new QWidget(this);
    right->setSizePolicy(colSp);
    QVBoxLayout *rightLayout = new QVBoxLayout(this);
    right->setLayout(rightLayout);
    rightLayout->setSpacing(8);
    rightLayout->setMargin(0);
    mainLayout->addWidget(right);

    QLabel *rightLabel = new QLabel(right);
    rightLabel->setText("<b>Videos</b>");
    rightLayout->addWidget(rightLabel);

    QHBoxLayout *movieQualityRow = new QHBoxLayout(right);
    movieQualityRow->setSpacing(4);
    QLabel *movieQualityLabel = new QLabel("Quality:");
    movieQualityRow->addWidget(movieQualityLabel);
    _movieQualityGroup = new QButtonGroup(right);
    QRadioButton *qualityLowButton = new QRadioButton("Low (720p)", right);
    _movieQualityGroup->addButton(qualityLowButton,
                                  (int)GameConfig::MovieQuality::Low720p);
    movieQualityRow->addWidget(qualityLowButton);
    QRadioButton *qualityHighButton = new QRadioButton("High (1080p)", right);
    _movieQualityGroup->addButton(qualityHighButton,
                                  (int)GameConfig::MovieQuality::High1080p);
    movieQualityRow->addWidget(qualityHighButton);
    rightLayout->addLayout(movieQualityRow);

    leftLayout->addStretch(1);
    rightLayout->addStretch(1);

    _resolutionGroup->button((int)rbApp->gameConfig()->resolution)
        ->setChecked(true);
    _fullscreenCb->setChecked(rbApp->gameConfig()->displayMode ==
                              GameConfig::DisplayMode::Fullscreen);
    _movieQualityGroup->button((int)rbApp->gameConfig()->movieQuality)
        ->setChecked(true);
}

void GraphicsTab::setConfig() {
    rbApp->gameConfig()->resolution =
        (GameConfig::Resolution)_resolutionGroup->checkedId();
    switch (rbApp->gameConfig()->resolution) {
        case GameConfig::Resolution::Res576p:
            rbApp->gameConfig()->width = 1024;
            rbApp->gameConfig()->height = 576;
            break;
        case GameConfig::Resolution::Res720p:
            rbApp->gameConfig()->width = 1280;
            rbApp->gameConfig()->height = 720;
            break;
        case GameConfig::Resolution::Res1080p:
            rbApp->gameConfig()->width = 1920;
            rbApp->gameConfig()->height = 1080;
            break;
    }
    rbApp->gameConfig()->displayMode = _fullscreenCb->isChecked()
                                           ? GameConfig::DisplayMode::Fullscreen
                                           : GameConfig::DisplayMode::Windowed;
    rbApp->gameConfig()->movieQuality =
        (GameConfig::MovieQuality)_movieQualityGroup->checkedId();
}