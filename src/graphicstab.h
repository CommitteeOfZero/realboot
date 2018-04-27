#pragma once

#include <QWidget>

class GraphicsTab : public QWidget {
    Q_OBJECT

   public:
    explicit GraphicsTab(QWidget* parent = 0);
    ~GraphicsTab() {}

    void setConfig();
};