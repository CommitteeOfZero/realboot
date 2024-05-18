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
    QCheckBox* _outlineCb;
    QCheckBox* _rineBlackNamesCb;
    QCheckBox* _consistencyCb;
    QCheckBox* _honorificsCb;
    QButtonGroup* _movieQualityGroup;
    QComboBox* _songSubsComboBox;
    QCheckBox* _hqAudioCb;
    QCheckBox* _cosplayPatch;
};