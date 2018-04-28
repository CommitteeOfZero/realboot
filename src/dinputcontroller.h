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
    ControllerConfig* config() { return _config; }
    QString deviceName() const { return _deviceName; }

    void startTracking(HWND topLevelWindow);
    void stopTracking();

    /* diagnostics */
    const DIJOYSTATE* lastState() { return &_lastState; }
    bool twoAxes() { return _twoAxes; }

   signals:
    void buttonPressed(ControllerConfig::Button button);
    /* diagnostics */
    void ticked();

   private:
    bool _isXinput;
    QString _guid;
    ControllerConfig* _config = nullptr;
    QString _deviceName;
    LPDIRECTINPUTDEVICE8 _joystick = nullptr;
    bool _twoAxes;
    DIJOYSTATE _lastState = {0};
    QTimer _timer;

   private slots:
    void timerTick();
};