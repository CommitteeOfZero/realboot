#include "controllermanager.h"

#include <QMessageBox>
#include <QApplication>

#include <stdexcept>

void InitError() {
    QMessageBox::critical(0, "Launcher error",
                          "Failed to initialize DirectInput 8");
    QApplication::quit();
}

BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE *pdidInstance,
                                  void *pContext) {
    ControllerManager *manager = (ControllerManager *)pContext;
    try {
        DinputController *controller =
            new DinputController(pdidInstance, manager);
        manager->addController(controller);
    } catch (const std::runtime_error &ex) {
    }
    return DIENUM_CONTINUE;
}

ControllerManager::ControllerManager(QObject *parent) {
    if (FAILED(DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION,
                                  IID_IDirectInput8, (void **)&_pDI,
                                  nullptr)) ||
        FAILED(_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback,
                                 (void *)this, DIEDFL_ATTACHEDONLY))) {
        InitError();
        return;
    }

    if (!_controllers.empty()) {
        setActiveController(_controllers.first());
    }
}

ControllerManager::~ControllerManager() { SAFE_RELEASE(_pDI); }

void ControllerManager::addController(DinputController *controller) {
    _controllers[controller->guid()] = controller;
}

void ControllerManager::setActiveController(const QString &guid) {
    if (_controllers.contains(guid)) {
        setActiveController(_controllers[guid]);
    }
}

void ControllerManager::setActiveController(DinputController *controller) {
    DinputController *oldController = _activeController;
    _activeController = controller;
    emit activeControllerChanged(oldController, controller);
}