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

    QHBoxLayout *resolutionRow = new QHBoxLayout(this);
    resolutionRow->setSpacing(8);
    resolutionRow->setMargin(0);
    QLabel *resolutionLabel = new QLabel("Resolution:", this);
    resolutionRow->addWidget(resolutionLabel);
    _resolutionComboBox = new QComboBox(this);
    _resolutionComboBox->addItem("1024x576",
                                 (int)GameConfig::Resolution::Res576p);
    _resolutionComboBox->addItem("1280x720",
                                 (int)GameConfig::Resolution::Res720p);
    _resolutionComboBox->addItem("1920x1080",
                                 (int)GameConfig::Resolution::Res1080p);
    resolutionRow->addWidget(_resolutionComboBox);
    resolutionRow->addSpacing(4);
    _fullscreenCb = new QCheckBox("Fullscreen", this);
    resolutionRow->addWidget(_fullscreenCb);
    resolutionRow->addStretch(1);
    mainLayout->addLayout(resolutionRow);

    _consistencyCb = new QCheckBox(
        "Edit text for consistency with other SciADV localizations\n(Change "
        "name order to Japanese, use established terms, etc.)",
        this);
    mainLayout->addWidget(_consistencyCb);

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

    QHBoxLayout *songSubsRow = new QHBoxLayout(this);
    songSubsRow->setSpacing(8);
    songSubsRow->setMargin(0);
    QLabel *songSubsLabel = new QLabel("Song subtitles:", this);
    songSubsRow->addWidget(songSubsLabel);
    _songSubsComboBox = new QComboBox(this);
    _songSubsComboBox->addItem("Off", (int)PatchConfig::SongSubs::Off);
    _songSubsComboBox->addItem("Karaoke + translation",
                               (int)PatchConfig::SongSubs::All);
    _songSubsComboBox->addItem("Karaoke only",
                               (int)PatchConfig::SongSubs::KaraOnly);
    _songSubsComboBox->addItem("Translation only",
                               (int)PatchConfig::SongSubs::TLOnly);
    songSubsRow->addWidget(_songSubsComboBox);
    songSubsRow->addStretch(1);
    mainLayout->addLayout(songSubsRow);

    _hqAudioCb = new QCheckBox("Use high-quality audio tracks for OP/ED", this);
    mainLayout->addWidget(_hqAudioCb);

    mainLayout->addStretch(1);

    reloadData();
}

void GeneralTab::setConfig() {
    rbApp->gameConfig()->resolution =
        (GameConfig::Resolution)_resolutionComboBox->currentData().toInt();
    rbApp->gameConfig()->displayMode = _fullscreenCb->isChecked()
                                           ? GameConfig::DisplayMode::Fullscreen
                                           : GameConfig::DisplayMode::Windowed;
    rbApp->gameConfig()->movieQuality =
        (GameConfig::MovieQuality)_movieQualityGroup->checkedId();

    rbApp->patchConfig()->improveDialogueOutlines = _outlineCb->isChecked();
    rbApp->patchConfig()->consistency = _consistencyCb->isChecked();
    rbApp->patchConfig()->karaokeSubs =
        PatchConfig::SongSubsOptions[_songSubsComboBox->currentData().toInt()];
    rbApp->patchConfig()->hqFmvAudio = _hqAudioCb->isChecked();
}

void GeneralTab::reloadData() {
    _resolutionComboBox->setCurrentIndex(
        _resolutionComboBox->findData((int)rbApp->gameConfig()->resolution));
    _fullscreenCb->setChecked(rbApp->gameConfig()->displayMode ==
                              GameConfig::DisplayMode::Fullscreen);
    _movieQualityGroup->button((int)rbApp->gameConfig()->movieQuality)
        ->setChecked(true);

    _outlineCb->setChecked(rbApp->patchConfig()->improveDialogueOutlines);
    _consistencyCb->setChecked(rbApp->patchConfig()->consistency);
    _songSubsComboBox->setCurrentIndex(
        _songSubsComboBox->findData(PatchConfig::SongSubsOptions.indexOf(
            rbApp->patchConfig()->karaokeSubs)));
    _hqAudioCb->setChecked(rbApp->patchConfig()->hqFmvAudio);
}