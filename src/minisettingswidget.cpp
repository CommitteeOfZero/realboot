#include "minisettingswidget.h"
#include "launcherapplication.h"
#include "gameconfig.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QRadioButton>

MiniSettingsWidget::MiniSettingsWidget(QWidget *parent) : QWidget(parent) {
#if !defined(GAME_STEINSGATEVSO)
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(8);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    mainLayout->addSpacing(8);

    QHBoxLayout *resolutionRow = new QHBoxLayout(this);
    resolutionRow->setSpacing(8);
    resolutionRow->setMargin(0);
    resolutionRow->addStretch(1);
    QLabel *resolutionLabel = new QLabel("Resolution:", this);
    resolutionRow->addWidget(resolutionLabel);
    _resolutionComboBox = new QComboBox(this);
#if !defined(GAME_CHAOSHEADNOAH)
    _resolutionComboBox->addItem("1024x576",
                                 (int)GameConfig::Resolution::Res576p);
#endif
    _resolutionComboBox->addItem("1280x720",
                                 (int)GameConfig::Resolution::Res720p);
#if defined(GAME_CHAOSHEADNOAH)
    _resolutionComboBox->addItem("1600x900",
                                 (int)GameConfig::Resolution::Res900p);
#endif
    _resolutionComboBox->addItem("1920x1080",
                                 (int)GameConfig::Resolution::Res1080p);
    resolutionRow->addWidget(_resolutionComboBox);
    resolutionRow->addSpacing(4);
    _fullscreenCb = new QCheckBox("Fullscreen", this);
    resolutionRow->addWidget(_fullscreenCb);
    resolutionRow->addStretch(1);
    mainLayout->addLayout(resolutionRow);
#if defined(GAME_CHAOSHEADNOAH)
    QHBoxLayout *languageRow = new QHBoxLayout(this);
    languageRow->setSpacing(8);
    languageRow->setMargin(0);
    languageRow->addStretch(1);
    QLabel *languageLabel = new QLabel("Language:", this);
    languageRow->addWidget(languageLabel);
    _languageGroup = new QButtonGroup(this);
    QRadioButton *japaneseButton = new QRadioButton("日本語", this);
    _languageGroup->addButton(japaneseButton,
                              (int)GameConfig::Language::Japanese);
    languageRow->addWidget(japaneseButton);
    QRadioButton *englishButton = new QRadioButton("English", this);
    _languageGroup->addButton(englishButton,
                              (int)GameConfig::Language::English);
    languageRow->addWidget(englishButton);
    languageRow->addStretch(1);
    mainLayout->addLayout(languageRow);
    mainLayout->addStretch(1);
#endif
#if !defined(GAME_CHAOSHEADNOAH) && !defined(GAME_STEINSGATEELITE) &&        \
    !defined(GAME_STEINSGATEVSO) && !defined(GAME_ROBOTICSNOTESELITE) &&     \
    !defined(GAME_ROBOTICSNOTESDASH) && !defined(GAME_ANONYMOUSCODE)
    QHBoxLayout *movieQualityRow = new QHBoxLayout(this);
    movieQualityRow->setSpacing(8);
    movieQualityRow->setMargin(0);
    movieQualityRow->addStretch(1);
    QLabel *movieQualityLabel = new QLabel("Video Quality:", this);
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
    mainLayout->addStretch(1);
#endif
#endif

    reloadData();
}

void MiniSettingsWidget::setConfig() {
#if !defined(GAME_STEINSGATEVSO)
    rbApp->gameConfig()->resolution =
        (GameConfig::Resolution)_resolutionComboBox->currentData().toInt();
    rbApp->gameConfig()->displayMode = _fullscreenCb->isChecked()
                                           ? GameConfig::DisplayMode::Fullscreen
                                           : GameConfig::DisplayMode::Windowed;
#if defined(GAME_CHAOSHEADNOAH)
    rbApp->gameConfig()->language =
        (GameConfig::Language)_languageGroup->checkedId();
#endif
#if !defined(GAME_CHAOSHEADNOAH) && !defined(GAME_STEINSGATEELITE) &&        \
    !defined(GAME_STEINSGATEVSO) && !defined(GAME_ROBOTICSNOTESELITE) &&     \
    !defined(GAME_ROBOTICSNOTESDASH) && !defined(GAME_ANONYMOUSCODE)
    rbApp->gameConfig()->movieQuality =
        (GameConfig::MovieQuality)_movieQualityGroup->checkedId();
#endif
#endif
}

void MiniSettingsWidget::reloadData() {
#if !defined(GAME_STEINSGATEVSO)
    _resolutionComboBox->setCurrentIndex(
        _resolutionComboBox->findData((int)rbApp->gameConfig()->resolution));
    _fullscreenCb->setChecked(rbApp->gameConfig()->displayMode ==
                              GameConfig::DisplayMode::Fullscreen);
#if !defined(GAME_CHAOSHEADNOAH) && !defined(GAME_STEINSGATEELITE) &&        \
    !defined(GAME_STEINSGATEVSO) && !defined(GAME_ROBOTICSNOTESELITE) &&     \
    !defined(GAME_ROBOTICSNOTESDASH) && !defined(GAME_ANONYMOUSCODE)
    _movieQualityGroup->button((int)rbApp->gameConfig()->movieQuality)
        ->setChecked(true);
#endif
#if defined(GAME_CHAOSHEADNOAH)
    _languageGroup->button((int)rbApp->gameConfig()->language)
        ->setChecked(true);
#endif
#endif
}