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
#if defined(GAME_ROBOTICSNOTESELITE) || defined(GAME_ROBOTICSNOTESDASH)
    mainLayout->addSpacing(8);
    _rneMouseControls = new QCheckBox("Enable mouse controls", this);
    mainLayout->addWidget(_rneMouseControls);
    _scrollDownToAdvanceText =
        new QCheckBox("Use scroll wheel for advancing text", this);
    mainLayout->addWidget(_scrollDownToAdvanceText);
    _disableScrollDownToCloseBacklog =
        new QCheckBox("Use scroll wheel for exiting the backlog", this);
    mainLayout->addWidget(_disableScrollDownToCloseBacklog);
#endif
#if defined(GAME_ROBOTICSNOTESD)
    mainLayout->addSpacing(8);
    _swimsuitPatch = new QCheckBox(
        "Enable Swimsuit Patch\n(Force characters to wear swimsuits at all "
        "times)",
        this);
    mainLayout->addWidget(_swimsuitPatch);
#endif

    if (rbApp->patchConfig()->hasConsistency) {
        _consistencyCb = new QCheckBox(
            "Edit text for consistency with other SciADV "
            "localizations\n(Change "
            "name order to Japanese, use established terms, etc.)",
            this);
        mainLayout->addWidget(_consistencyCb);
    }

    if (rbApp->patchConfig()->hasHonorifics) {
        _honorificsCb =
            new QCheckBox("Edit text to use Japanese honorifics", this);
        mainLayout->addWidget(_honorificsCb);
    }

#if !defined(GAME_ROBOTICSNOTESELITE) && !defined(GAME_ROBOTICSNOTESDASH)
    _outlineCb = new QCheckBox("Improve dialogue outlines", this);
    mainLayout->addWidget(_outlineCb);
#endif

    if (rbApp->patchConfig()->hasrineBlackNames) {
        _rineBlackNamesCb =
            new QCheckBox("Use solid black text for names in RINE", this);
        mainLayout->addWidget(_rineBlackNamesCb);
    }

    mainLayout->addSpacing(16);

    QLabel *fmvLabel = new QLabel(this);
    fmvLabel->setText("<b>Videos</b>");
    mainLayout->addWidget(fmvLabel);

#if !defined(GAME_ROBOTICSNOTESELITE) && !defined(GAME_ROBOTICSNOTESDASH)
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
#endif

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

#ifndef GAME_ROBOTICSNOTESELITE
    _hqAudioCb = new QCheckBox("Use high-quality audio tracks for OP/ED", this);
    mainLayout->addWidget(_hqAudioCb);
#endif

    mainLayout->addStretch(1);
    if (rbApp->patchConfig()->hasCosplayPatch) {
        _cosplayPatch = new QCheckBox(
            "Enable Cosplay Patch\n(Mayuri's Tutturu beam has been fired?! Its "
            "effect forces everyone to cosplay!)",
            this);
        mainLayout->addWidget(_cosplayPatch);
    }
    mainLayout->addStretch(1);
    reloadData();
}

void GeneralTab::setConfig() {
    rbApp->gameConfig()->resolution =
        (GameConfig::Resolution)_resolutionComboBox->currentData().toInt();
    rbApp->gameConfig()->displayMode = _fullscreenCb->isChecked()
                                           ? GameConfig::DisplayMode::Fullscreen
                                           : GameConfig::DisplayMode::Windowed;
#ifndef GAME_ROBOTICSNOTESELITE
    rbApp->gameConfig()->movieQuality =
        (GameConfig::MovieQuality)_movieQualityGroup->checkedId();
    rbApp->patchConfig()->hqFmvAudio = _hqAudioCb->isChecked();
#endif
    rbApp->patchConfig()->improveDialogueOutlines = _outlineCb->isChecked();
    if (rbApp->patchConfig()->hasConsistency) {
        rbApp->patchConfig()->consistency = _consistencyCb->isChecked();
    }
    if (rbApp->patchConfig()->hasHonorifics) {
        rbApp->patchConfig()->honorifics = _honorificsCb->isChecked();
    }
    if (rbApp->patchConfig()->hasrineBlackNames) {
        rbApp->patchConfig()->rineBlackNames = _rineBlackNamesCb->isChecked();
    }
    rbApp->patchConfig()->karaokeSubs =
        PatchConfig::SongSubsOptions[_songSubsComboBox->currentData().toInt()];
    if (rbApp->patchConfig()->hasCosplayPatch) {
        rbApp->patchConfig()->cosplayPatch = _cosplayPatch->isChecked();
    }
#ifdef defined(GAME_ROBOTICSNOTESELITE) || defined(GAME_ROBOTICSNOTESDASH)
    rbApp->patchConfig()->rneMouseControls = _rneMouseControls->isChecked();
    rbApp->patchConfig()->scrollDownToAdvanceText =
        _scrollDownToAdvanceText->isChecked();
    rbApp->patchConfig()->disableScrollDownToCloseBacklog =
        !_disableScrollDownToCloseBacklog->isChecked();
#endif
#if defined(GAME_ROBOTICSNOTESDASH)
    rbApp->patchConfig()->swimsuitPatch = _swimsuitPatch->isChecked();
#endif
}

void GeneralTab::reloadData() {
    _resolutionComboBox->setCurrentIndex(
        _resolutionComboBox->findData((int)rbApp->gameConfig()->resolution));
    _fullscreenCb->setChecked(rbApp->gameConfig()->displayMode ==
                              GameConfig::DisplayMode::Fullscreen);
#if !defined(GAME_ROBOTICSNOTESELITE) && !defined(GAME_ROBOTICSNOTESDASH)
    _movieQualityGroup->button((int)rbApp->gameConfig()->movieQuality)
        ->setChecked(true);
#endif

    _outlineCb->setChecked(rbApp->patchConfig()->improveDialogueOutlines);
    if (rbApp->patchConfig()->hasConsistency) {
        _consistencyCb->setChecked(rbApp->patchConfig()->consistency);
    }
    if (rbApp->patchConfig()->hasHonorifics) {
        _honorificsCb->setChecked(rbApp->patchConfig()->honorifics);
    }
    if (rbApp->patchConfig()->hasrineBlackNames) {
        _rineBlackNamesCb->setChecked(rbApp->patchConfig()->rineBlackNames);
    }
    _songSubsComboBox->setCurrentIndex(
        _songSubsComboBox->findData(PatchConfig::SongSubsOptions.indexOf(
            rbApp->patchConfig()->karaokeSubs)));
    if (rbApp->patchConfig()->hasCosplayPatch) {
        _cosplayPatch->setChecked(rbApp->patchConfig()->cosplayPatch);
    }
#if defined(GAME_ROBOTICSNOTESELITE) || defined(GAME_ROBOTICSNOTESDASH)
    _rneMouseControls->setChecked(rbApp->patchConfig()->rneMouseControls);
    _scrollDownToAdvanceText->setChecked(
        rbApp->patchConfig()->scrollDownToAdvanceText);
    _disableScrollDownToCloseBacklog->setChecked(
        !rbApp->patchConfig()->disableScrollDownToCloseBacklog);
#endif
#if defined(GAME_ROBOTICSNOTESDASH)
    _swimsuitPatch->setChecked(rbApp->patchConfig()->swimsuitPatch);
#endif
#if !defined(GAME_ROBOTICSNOTESELITE) && !defined(GAME_ROBOTICSNOTESDASH)
    _hqAudioCb->setChecked(rbApp->patchConfig()->hqFmvAudio);
#endif
}