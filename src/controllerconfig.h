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

    explicit ControllerConfig(const QString& guid, QObject* parent = 0);
    ~ControllerConfig() {}

    void save();

    Preset preset = Preset::Default;
    Button enter = Button::A;
    Button cancel = Button::B;
    Button autoMode = Button::X;
    Button skip = Button::RB;
    Button systemMenu = Button::START;
    Button tips = Button::BACK;
    Button forceSkip = Button::LB;
    Button backlog = Button::Y;
    Button quickSave = Button::RS;
    Button custom1 = Button::LT;
    Button custom2 = Button::RT;

   private:
    QString _path;
};