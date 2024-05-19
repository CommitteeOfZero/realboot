#pragma once

#include <QWidget>
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>

class MiniSettingsWidget : public QWidget {
    Q_OBJECT

   public:
    explicit MiniSettingsWidget(QWidget* parent = 0);
    ~MiniSettingsWidget() {}

    void setConfig();
    void reloadData();

   private:
    QComboBox* _resolutionComboBox;
    QCheckBox* _fullscreenCb;
#if defined(GAME_CHAOSHEADNOAH)
    QButtonGroup* _languageGroup;
#endif
#if !defined(GAME_CHAOSHEADNOAH) && !defined(GAME_ROBOTICSNOTESELITE) && \
    !defined(GAME_ROBOTICSNOTESDASH) && !defined(GAME_ANONYMOUSCODE)
    QButtonGroup* _movieQualityGroup;
#endif
};