QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
           core/pet_core.cpp \
           core/emotion_manager.cpp \
           core/action_controller.cpp \
           ui/pet_widget.cpp \
           ui/chat_dialog.cpp \
           interaction/mouse_tracker.cpp \
           interaction/reminder_manager.cpp \

           ai/ai_manager.cpp \


HEADERS += core/pet_core.h \
           core/emotion_manager.h \
           core/action_controller.h \
           ui/pet_widget.h \
           ui/chat_dialog.h \
           interaction/mouse_tracker.h \
           interaction/reminder_manager.h \

           ai/ai_manager.h \


FORMS += \
    widget.ui
RESOURCES += res/qrc_resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L"D:\SDK\vosk-win64-0.3.45" -lvosk
