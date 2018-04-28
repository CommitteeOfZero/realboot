#include "dinputcontroller.h"
#include "controllermanager.h"

#include <wbemidl.h>
#include <oleauto.h>

#include <stdexcept>

BOOL IsXInputDevice(const GUID* pGuidProductFromDirectInput);

DinputController::DinputController(const DIDEVICEINSTANCE* pdidInstance,
                                   ControllerManager* parent) {
    _isXinput = IsXInputDevice(&pdidInstance->guidProduct);

    if (FAILED(parent->pDI()->CreateDevice(pdidInstance->guidInstance,
                                           &_joystick, nullptr)) ||
        FAILED(_joystick->SetDataFormat(&c_dfDIJoystick))) {
        throw std::runtime_error("Couldn't create device for joystick");
    }

    wchar_t* wGuid;
    StringFromIID(pdidInstance->guidInstance, &wGuid);
    _guid = QString::fromWCharArray(wGuid);
    CoTaskMemFree(wGuid);

    _deviceName = QString::fromWCharArray(pdidInstance->tszInstanceName) +
                  (_isXinput ? " (XInput)" : " (DirectInput)");

    _config = new ControllerConfig(_guid, this);

    _timer.setSingleShot(true);
    _timer.setInterval((int)(1000.f / 30.f));
    connect(&_timer, &QTimer::timeout, this, &DinputController::timerTick);
}

DinputController::~DinputController() {
    if (_joystick) _joystick->Unacquire();
    SAFE_RELEASE(_joystick);
}

void DinputController::startTracking(HWND topLevelWindow) {
    if (!_joystick) return;
    _timer.stop();
    _joystick->SetCooperativeLevel(topLevelWindow,
                                   DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    _timer.start();
}

void DinputController::stopTracking() { _timer.stop(); }

void DinputController::timerTick() {
    if (!_joystick) return;
    if (FAILED(_joystick->Poll())) {
        // MS seems to think this is a good idea, anyway...
        // https://github.com/walbourn/directx-sdk-samples/blob/master/DirectInput/Joystick/joystick.cpp
        HRESULT hr = _joystick->Acquire();
        while (hr == DIERR_INPUTLOST) {
            hr = _joystick->Acquire();
        }
        _timer.start();
        return;
    }
    DIJOYSTATE newState;
    if (FAILED(_joystick->GetDeviceState(sizeof(DIJOYSTATE), &newState))) {
        _timer.start();
        return;
    }

    for (int i = 0; i < (int)ControllerConfig::Button::Num; i++) {
        if (newState.rgbButtons[i] & 0x80 &&
            !(_lastState.rgbButtons[i] & 0x80)) {
            emit buttonPressed((ControllerConfig::Button)i);
        }
    }

    _lastState = newState;

    _timer.start();
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/ee417014(v=vs.85).aspx
//-----------------------------------------------------------------------------
// Enum each PNP device using WMI and check each device ID to see if it contains
// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it's an XInput device
// Unfortunately this information can not be found by just using DirectInput
//-----------------------------------------------------------------------------
BOOL IsXInputDevice(const GUID* pGuidProductFromDirectInput) {
    IWbemLocator* pIWbemLocator = NULL;
    IEnumWbemClassObject* pEnumDevices = NULL;
    IWbemClassObject* pDevices[20] = {0};
    IWbemServices* pIWbemServices = NULL;
    BSTR bstrNamespace = NULL;
    BSTR bstrDeviceID = NULL;
    BSTR bstrClassName = NULL;
    DWORD uReturned = 0;
    bool bIsXinputDevice = false;
    UINT iDevice = 0;
    VARIANT var;
    HRESULT hr;

    // CoInit if needed
    hr = CoInitialize(NULL);
    bool bCleanupCOM = SUCCEEDED(hr);

    // Create WMI
    hr = CoCreateInstance(__uuidof(WbemLocator), NULL, CLSCTX_INPROC_SERVER,
                          __uuidof(IWbemLocator), (LPVOID*)&pIWbemLocator);
    if (FAILED(hr) || pIWbemLocator == NULL) goto LCleanup;

    bstrNamespace = SysAllocString(L"\\\\.\\root\\cimv2");
    if (bstrNamespace == NULL) goto LCleanup;
    bstrClassName = SysAllocString(L"Win32_PNPEntity");
    if (bstrClassName == NULL) goto LCleanup;
    bstrDeviceID = SysAllocString(L"DeviceID");
    if (bstrDeviceID == NULL) goto LCleanup;

    // Connect to WMI
    hr = pIWbemLocator->ConnectServer(bstrNamespace, NULL, NULL, 0L, 0L, NULL,
                                      NULL, &pIWbemServices);
    if (FAILED(hr) || pIWbemServices == NULL) goto LCleanup;

    // Switch security level to IMPERSONATE.
    CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
                      RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL,
                      EOAC_NONE);

    hr = pIWbemServices->CreateInstanceEnum(bstrClassName, 0, NULL,
                                            &pEnumDevices);
    if (FAILED(hr) || pEnumDevices == NULL) goto LCleanup;

    // Loop over all devices
    for (;;) {
        // Get 20 at a time
        hr = pEnumDevices->Next(10000, 20, pDevices, &uReturned);
        if (FAILED(hr)) goto LCleanup;
        if (uReturned == 0) break;

        for (iDevice = 0; iDevice < uReturned; iDevice++) {
            // For each device, get its device ID
            hr = pDevices[iDevice]->Get(bstrDeviceID, 0L, &var, NULL, NULL);
            if (SUCCEEDED(hr) && var.vt == VT_BSTR && var.bstrVal != NULL) {
                // Check if the device ID contains "IG_".  If it does, then it's an XInput device
                // This information can not be found from DirectInput
                if (wcsstr(var.bstrVal, L"IG_")) {
                    // If it does, then get the VID/PID from var.bstrVal
                    DWORD dwPid = 0, dwVid = 0;
                    WCHAR* strVid = wcsstr(var.bstrVal, L"VID_");
                    if (strVid && swscanf(strVid, L"VID_%4X", &dwVid) != 1)
                        dwVid = 0;
                    WCHAR* strPid = wcsstr(var.bstrVal, L"PID_");
                    if (strPid && swscanf(strPid, L"PID_%4X", &dwPid) != 1)
                        dwPid = 0;

                    // Compare the VID/PID to the DInput device
                    DWORD dwVidPid = MAKELONG(dwVid, dwPid);
                    if (dwVidPid == pGuidProductFromDirectInput->Data1) {
                        bIsXinputDevice = true;
                        goto LCleanup;
                    }
                }
            }
            SAFE_RELEASE(pDevices[iDevice]);
        }
    }

LCleanup:
    if (bstrNamespace) SysFreeString(bstrNamespace);
    if (bstrDeviceID) SysFreeString(bstrDeviceID);
    if (bstrClassName) SysFreeString(bstrClassName);
    for (iDevice = 0; iDevice < 20; iDevice++) SAFE_RELEASE(pDevices[iDevice]);
    SAFE_RELEASE(pEnumDevices);
    SAFE_RELEASE(pIWbemLocator);
    SAFE_RELEASE(pIWbemServices);

    if (bCleanupCOM) CoUninitialize();

    return bIsXinputDevice;
}