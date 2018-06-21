#include "generaltab.h"
#include "launcherapplication.h"
#include "gameconfig.h"
#include "patchconfig.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QRadioButton>

GeneralTab::GeneralTab(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(8);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    mainLayout->addSpacing(8);

    _fullscreenCb = new QCheckBox("Fullscreen", this);
    mainLayout->addWidget(_fullscreenCb);

    QHBoxLayout *resolutionRow = new QHBoxLayout(this);
    resolutionRow->setSpacing(8);
    resolutionRow->setMargin(0);
    QLabel *resolutionLabel = new QLabel("Resolution:", this);
    resolutionRow->addWidget(resolutionLabel);
    _resolutionGroup = new QButtonGroup(this);
    QRadioButton *r576pButton = new QRadioButton("1024x576", this);
    _resolutionGroup->addButton(r576pButton,
                                (int)GameConfig::Resolution::Res576p);
    resolutionRow->addWidget(r576pButton);
    QRadioButton *r720pButton = new QRadioButton("1280x720", this);
    _resolutionGroup->addButton(r720pButton,
                                (int)GameConfig::Resolution::Res720p);
    resolutionRow->addWidget(r720pButton);
    QRadioButton *r1080pButton = new QRadioButton("1920x1080", this);
    _resolutionGroup->addButton(r1080pButton,
                                (int)GameConfig::Resolution::Res1080p);
    resolutionRow->addWidget(r1080pButton);
    resolutionRow->addStretch(1);
    mainLayout->addLayout(resolutionRow);

    _outlineCb = new QCheckBox("Improve dialogue outlines", this);
    mainLayout->addWidget(_outlineCb);

    mainLayout->addSpacing(16);

    QLabel *fmvLabel = new QLabel(this);
    fmvLabel->setText("<b>Videos</b>");
    mainLayout->addWidget(fmvLabel);

    QHBoxLayout *movieQualityRow = new QHBoxLayout(this);
    movieQualityRow->setSpacing(8);
    movieQualityRow->setMargin(0);
    QLabel *movieQualityLabel = new QLabel("Quality:", this);
    movieQualityRow->addWidget(movieQualityLabel);
    _movieQualityGroup = new QButtonGroup(this);
    QRadioButton *qualityLowButton = new QRadioButton("Low (720p)", this);
    _movieQualityGroup->addButton(qualityLowButton,
                                  (int)GameConfig::MovieQuality::Low720p);
    movieQualityRow->addWidget(qualityLowButton);
    QRadioButton *qualityHighButton = new QRadioButton("High (1080p)", this);
    _movieQualityGroup->addButton(qualityHighButton,
                                  (int)GameConfig::MovieQuality::High1080p);
    movieQualityRow->addWidget(qualityHighButton);
    movieQualityRow->addStretch(1);
    mainLayout->addLayout(movieQualityRow);

    _karaokeCb = new QCheckBox("Karaoke subtitles for OP/ED", this);
    mainLayout->addWidget(_karaokeCb);

    _hqAudioCb = new QCheckBox("Use high-quality audio tracks for OP/ED", this);
    mainLayout->addWidget(_hqAudioCb);

    mainLayout->addStretch(1);

    reloadData();
}

void GeneralTab::setConfig() {
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

    rbApp->patchConfig()->improveDialogueOutlines = _outlineCb->isChecked();
    rbApp->patchConfig()->karaokeSubs =
        _karaokeCb->isChecked() ? "lowQuality" : "off";
    rbApp->patchConfig()->hqFmvAudio = _hqAudioCb->isChecked();
}

void GeneralTab::reloadData() {
    _resolutionGroup->button((int)rbApp->gameConfig()->resolution)
        ->setChecked(true);
    _fullscreenCb->setChecked(rbApp->gameConfig()->displayMode ==
                              GameConfig::DisplayMode::Fullscreen);
    _movieQualityGroup->button((int)rbApp->gameConfig()->movieQuality)
        ->setChecked(true);

    _outlineCb->setChecked(rbApp->patchConfig()->improveDialogueOutlines);
    _karaokeCb->setChecked(rbApp->patchConfig()->karaokeSubs == "lowQuality");
    _hqAudioCb->setChecked(rbApp->patchConfig()->hqFmvAudio);
}