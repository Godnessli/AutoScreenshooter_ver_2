QT       += core gui widgets webenginewidgets webenginecore sql

#INCLUDEPATH = A:/Qt/xlnt_x64-windows/include
#LIBS += A:/Qt/xlnt_x64-windows/debug/lib/xlntd.lib

INCLUDEPATH = C:/Users/kznts/vcpkg/packages/xlnt_x64-windows/include
LIBS += C:/Users/kznts/vcpkg/packages/xlnt_x64-windows/debug/lib/xlntd.lib
#LIBS += C:/Users/kznts/vcpkg/packages/xlnt_x64-windows/lib/xlnt.lib

CONFIG += c++17

SOURCES += \
    classes/automate.cpp \
    classes/requestinterceptor.cpp \
    classes/table.cpp \
    forms/mainwindow.cpp \
    main.cpp \

HEADERS += \
    classes/automate.h \
    classes/delegate.h \
    classes/requestinterceptor.h \
    classes/table.h \
    forms/mainwindow.h

FORMS += \
    forms/mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
