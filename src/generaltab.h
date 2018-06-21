#pragma once

#include <QWidget>
#include <QButtonGroup>
#include <QCheckBox>

class GeneralTab : public QWidget {
    Q_OBJECT

   public:
    explicit GeneralTab(QWidget* parent = 0);
    ~GeneralTab() {}

    void setConfig();
    void reloadData();

   private:
    QButtonGroup* _resolutionGroup;
    QCheckBox* _fullscreenCb;
    QCheckBox* _outlineCb;
    QCheckBox* _consistencyCb;
    QButtonGroup* _movieQualityGroup;
    QCheckBox* _karaokeCb;
    QCheckBox* _hqAudioCb;
};