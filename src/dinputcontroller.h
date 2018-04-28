#pragma once

#include "controllerconfig.h"

#include <Windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <QObject>
#include <QTimer>

class ControllerManager;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)     \
    {                       \
        if (p) {            \
            (p)->Release(); \
            (p) = nullptr;  \
        }                   \
    }
#endif

class DinputController : public QObject {
    Q_OBJECT

   public:
    explicit DinputController(const DIDEVICEINSTANCE* pdidInstance,
                              ControllerManager* parent);
    ~DinputController();

    QString guid() const { return _guid; }
    bool isXinput() const { return _isXinput; }

    void startTracking(HWND topLevelWindow);
    void stopTracking();

   signals:
    void buttonPressed(ControllerConfig::Button button);

   private:
    bool _isXinput;
    QString _guid;
    LPDIRECTINPUTDEVICE8 _joystick;
    DIJOYSTATE _lastState = {0};
    QTimer _timer;

   private slots:
    void timerTick();
};