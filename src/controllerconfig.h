#pragma once

#include <QObject>

class ControllerConfig : public QObject {
    Q_OBJECT

   public:
    enum class Preset : uint32_t { Default = 0, Custom = 1, Num };
    Q_ENUM(Preset)
    enum class Button : uint8_t {
        A = 0,
        B = 1,
        X = 2,
        Y = 3,
        LB = 4,
        RB = 5,
        BACK = 6,
        START = 7,
        LS = 8,
        RS = 9,
        LT = 10,
        RT = 11,
        Num
    };
    Q_ENUM(Button)
    enum class Bind : uint32_t {
        Enter = 0,
        Cancel = 1,
        AutoMode = 2,
        Skip = 3,
        SystemMenu = 4,
        Tips = 5,
        ForceSkip = 6,
        Backlog = 7,
        QuickSave = 8,
        Custom1 = 9,
        Custom2 = 10,
        Num
    };
    Q_ENUM(Bind)

    explicit ControllerConfig(const QString& guid, QObject* parent = 0);
    ~ControllerConfig() {}

    void loadDefaults();
    void save();

    Preset preset;
    Button binds[(size_t)Bind::Num];

   private:
    QString _path;
};