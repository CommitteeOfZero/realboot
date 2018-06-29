#include "minisettingswidget.h"
#include "launcherapplication.h"
#include "gameconfig.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QRadioButton>

MiniSettingsWidget::MiniSettingsWidget(QWidget *parent) : QWidget(parent) {
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

    reloadData();
}

void MiniSettingsWidget::setConfig() {
    rbApp->gameConfig()->resolution =
        (GameConfig::Resolution)_resolutionComboBox->currentData().toInt();
    rbApp->gameConfig()->displayMode = _fullscreenCb->isChecked()
                                           ? GameConfig::DisplayMode::Fullscreen
                                           : GameConfig::DisplayMode::Windowed;
    rbApp->gameConfig()->movieQuality =
        (GameConfig::MovieQuality)_movieQualityGroup->checkedId();
}

void MiniSettingsWidget::reloadData() {
    _resolutionComboBox->setCurrentIndex(
        _resolutionComboBox->findData((int)rbApp->gameConfig()->resolution));
    _fullscreenCb->setChecked(rbApp->gameConfig()->displayMode ==
                              GameConfig::DisplayMode::Fullscreen);
    _movieQualityGroup->button((int)rbApp->gameConfig()->movieQuality)
        ->setChecked(true);
}