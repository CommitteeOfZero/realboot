#pragma once

#include <QWidget>
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>

class GeneralTab : public QWidget {
    Q_OBJECT

   public:
    explicit GeneralTab(QWidget* parent = 0);
    ~GeneralTab() {}

    void setConfig();
    void reloadData();

   private:
    QComboBox* _resolutionComboBox;
    QCheckBox* _fullscreenCb;
#if defined(GAME_CHAOSHEADNOAH)
    QButtonGroup* _languageGroup;
#endif
    QCheckBox* _outlineCb;
    QCheckBox* _rineBlackNamesCb;
    QCheckBox* _consistencyCb;
    QCheckBox* _honorificsCb;
    QButtonGroup* _movieQualityGroup;
    QComboBox* _songSubsComboBox;
    QCheckBox* _hqAudioCb;
    QCheckBox* _cosplayPatch;
#if defined(GAME_ROBOTICSNOTESELITE) || defined(GAME_ROBOTICSNOTESDASH)
    QCheckBox* _rneMouseControls;
    QCheckBox* _scrollDownToAdvanceText;
    QCheckBox* _disableScrollDownToCloseBacklog;
#endif
#if defined(GAME_ROBOTICSNOTESDASH)
    QCheckBox* _swimsuitPatch;
#endif
#if defined(GAME_ANONYMOUSCODE)
    QCheckBox* _voiceSubsCb;
#endif
};