QT       += core gui widgets webenginewidgets webenginecore
QT       += sql

INCLUDEPATH = A:/Qt/xlnt_x64-windows/include

CONFIG(debug, debug|release) {
    LIBS += A:/Qt/xlnt_x64-windows/debug/lib/xlntd.lib
}else {
    LIBS += A:/Qt/xlnt_x64-windows/lib/xlnt.lib
}

#INCLUDEPATH = C:/Users/kznts/vcpkg/packages/xlnt_x64-windows/include
#LIBS += C:/Users/kznts/vcpkg/packages/xlnt_x64-windows/debug/lib/xlntd.lib
#LIBS += C:/Users/kznts/vcpkg/packages/xlnt_x64-windows/lib/xlnt.lib

CONFIG += c++17

SOURCES += \
    classes/automate.cpp \
    classes/table.cpp \
    classes/viewer.cpp \
    forms/dialog.cpp \
    forms/mainwindow.cpp \
    forms/setting.cpp \
    main.cpp \

HEADERS += \
    classes/automate.h \
    classes/delegate.h \
    classes/table.h \
    classes/viewer.h \
    forms/dialog.h \
    forms/mainwindow.h \
    forms/setting.h

FORMS += \
    forms/dialog.ui \
    forms/mainwindow.ui \
    forms/setting.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    forms/login.js
