QT       += core gui widgets webenginewidgets webenginecore sql

INCLUDEPATH = A:/Qt/xlnt_x64-windows/include
LIBS += A:/Qt/xlnt_x64-windows/debug/lib/xlntd.lib

#INCLUDEPATH = C:/Users/kznts/vcpkg/packages/xlnt_x64-windows/include
#LIBS += C:/Users/kznts/vcpkg/packages/xlnt_x64-windows/debug/lib/xlntd.lib

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    classes/table.cpp \
    forms/mainwindow.cpp \
    main.cpp \

HEADERS += \
    classes/delegate.h \
    classes/table.h \
    forms/mainwindow.h

FORMS += \
    forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
