#pragma once

#include <QWidget>
#include <QButtonGroup>
#include <QCheckBox>

class GraphicsTab : public QWidget {
    Q_OBJECT

   public:
    explicit GraphicsTab(QWidget* parent = 0);
    ~GraphicsTab() {}

    void setConfig();

   private:
    QButtonGroup* _resolutionGroup;
    QCheckBox* _fullscreenCb;
};