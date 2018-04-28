#pragma once

#include "dinputcontroller.h"

#include <QMap>

class ControllerManager : public QObject {
    Q_OBJECT

   public:
    explicit ControllerManager(QObject* parent = 0);
    ~ControllerManager();

    const QMap<QString, DinputController*>& controllers() const {
        return _controllers;
    }

    DinputController* activeController() const { return _activeController; }
    void setActiveController(const QString& guid);

    void addController(DinputController* controller);

    LPDIRECTINPUT8 pDI() { return _pDI; }

   signals:
    void activeControllerChanged(DinputController* oldController,
                                 DinputController* newController);

   private:
    QMap<QString, DinputController*> _controllers;
    LPDIRECTINPUT8 _pDI = nullptr;
    DinputController* _activeController = nullptr;

    void setActiveController(DinputController* controller);
};