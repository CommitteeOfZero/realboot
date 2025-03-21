QT += core gui widgets concurrent

TARGET = realboot
TEMPLATE = app

CONFIG += force_debug_info

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# This crap lets us run files with the same name, in the same project, through moc, without conflicts.
# Good idea? Probably not.

CONFIG += object_parallel_to_source
CONFIG += no_batch # above doesn't properly work with nmake without this

defineReplace(fnPath) {
    variable = $$1
    variable = $$replace(variable, $$basename(variable), "")
    variable = $$replace(variable, "\\\\", "_")
    variable = $$replace(variable, "\.", "_")
    variable = $$replace(variable, "/", "_")
    return($$variable)
}

QMAKE_H_MOD_MOC = moc_${QMAKE_FUNC_fnPath}
QMAKE_CPP_MOD_MOC = moc_${QMAKE_FUNC_fnPath}
QMAKE_MOD_RCC = qrc${QMAKE_FUNC_fnPath}

# But I'm going with it anyway.

include(kofuna/kofuna.pri)

chaosheadnoah {
    DEFINES += GAME_CHAOSHEADNOAH
    RESOURCES += profiles/chaosheadnoah.qrc
}
steinsgate {
    DEFINES += GAME_STEINSGATE
    RESOURCES += profiles/steinsgate.qrc
}
steinsgateelite {
    DEFINES += GAME_STEINSGATEELITE
    RESOURCES += profiles/steinsgateelite.qrc
}
chaosheadlcc {
    DEFINES += GAME_CHAOSHEADLCC
    RESOURCES += profiles/chaosheadlcc.qrc
}
steinsgatemde {
    DEFINES += GAME_STEINSGATEMDE
    RESOURCES += profiles/steinsgatemde.qrc
}
roboticsnoteselite {
    DEFINES += GAME_ROBOTICSNOTESELITE
    RESOURCES += profiles/roboticsnoteselite.qrc
}
steinsgatelbp {
    DEFINES += GAME_STEINSGATELBP
    RESOURCES += profiles/steinsgatelbp.qrc
}
chaoschild {
    DEFINES += GAME_CHAOSCHILD
    RESOURCES += profiles/chaoschild.qrc
}
steinsgate0 {
    DEFINES += GAME_STEINSGATE0
    RESOURCES += profiles/steinsgate0.qrc
}
chaoschildlcc {
    DEFINES += GAME_CHAOSCHILDLCC
    RESOURCES += profiles/chaoschildlcc.qrc
}
roboticsnotesdash {
    DEFINES += GAME_ROBOTICSNOTESDASH
    RESOURCES += profiles/roboticsnotesdash.qrc
}
anonymouscode {
    DEFINES += GAME_ANONYMOUSCODE
    RESOURCES += profiles/anonymouscode.qrc
}
steam {
    DEFINES += GAME_STEAM
}

DEFINES += WIN32_LEAN_AND_MEAN
INCLUDEPATH += $$(DXSDK_DIR)/include vendor/curl/include
LIBS += $$quote(-L$$PWD/vendor/curl/lib) libcurl.lib $$quote(-L$$(DXSDK_DIR)/lib/x86) dinput8.lib dxguid.lib Shell32.lib Ole32.lib OleAut32.lib

INCLUDEPATH += $$PWD/vendor/lua/src
SOURCES += $$files($$PWD/vendor/lua/src/*.c, true)

# Exclude files that contain main()
SOURCES -= $$PWD/vendor/lua/src/lua.c
SOURCES -= $$PWD/vendor/lua/src/luac.c

INCLUDEPATH += src

SOURCES += $$files(src/*.cpp, true)
SOURCES += $$files(src/*.c, true)
HEADERS += $$files(src/*.h, true)
FORMS += $$files(src/*.ui, true)

RESOURCES += \
    main.qrc