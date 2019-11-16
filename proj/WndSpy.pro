#-------------------------------------------------
#
# Project created by QtCreator 2011-05-27T06:07:17
#
#-------------------------------------------------

QT       += core gui widgets winextras
LIBS += -lgdi32
LIBS += -lpsapi
LIBS += -ldwmapi

TARGET = WndSpy
TEMPLATE = app

#---------------------------------------------------------------------------------------------------
# Versión
#---------------------------------------------------------------------------------------------------

DEFINES += MAJOR_VERSION=1
DEFINES += MINOR_VERSION=0

#---------------------------------------------------------------------------------------------------
# Directorio de destino
#---------------------------------------------------------------------------------------------------
DESTDIR = ..\bin

#---------------------------------------------------------------------------------------------------
# Directorios de inclusión
#---------------------------------------------------------------------------------------------------
INCLUDEPATH +=	res \
				src \
				src/3rd \
				src/models \
				src/dialogs \
				src/formsinfo1 \
				src/formsinfo2


#---------------------------------------------------------------------------------------------------
# Archivos de código
#---------------------------------------------------------------------------------------------------

HEADERS += \
    src/utils.h \
    src/utils2.h \
    src/dialogs/DlgFindWindow.h \
    src/3rd/InjectThread.h \
    src/3rd/GetRemoteWindowInfo.h \
    src/dialogs/DlgAbout.h \
    src/dialogs/DlgEditSize.h \
    src/dialogs/DlgEditStyle.h \
    src/formsinfo2/FormInfo2Windows.h \
    src/formsinfo2/FormInfo2Process.h \
    src/formsinfo2/list/ProcessList.h \
    src/formsinfo2/list/WindowProcessList.h \
    src/formsinfo1/FormInfoWindows.h \
    src/formsinfo1/FormInfoStyles.h \
    src/formsinfo1/FormInfoScroll.h \
    src/formsinfo1/FormInfoProperties.h \
    src/formsinfo1/FormInfoProcess.h \
    src/formsinfo1/FormInfoClass.h \
    src/formsinfo1/FormInfoGeneral.h \
    src/formsinfo1/FormInfoRect.h \
    src/MainWindow.h \
    src/MenuWindow.h \
    src/Languages.h \
    src/selector/WindowSelector.h \
    src/selector/WindowSelectorRectWidget.h \
    src/WindowInfo.h \
    src/LinkLabel.h \
    src/models/WindowsItemModel.h \
    src/models/StylesItemModel.h \
    src/models/PropertiesItemModel.h \
    src/3rd/WindowFromPointEx.h

SOURCES += \
    src/utils.cpp \
    src/main.cpp \
    src/data.cpp \
    src/dialogs/DlgFindWindow.cpp \
    src/3rd/InjectThread.c \
    src/3rd/GetRemoteWindowInfo.c \
    src/3rd/GetProcessNameByPid.c \
    src/3rd/FlashHWnd.c \
    src/3rd/CaptureHWnd.c \
    src/3rd/GetIconSize.c \
    src/3rd/CopyBrush.c \
    src/dialogs/DlgAbout.cpp \
    src/dialogs/DlgEditSize.cpp \
    src/dialogs/DlgEditStyle.cpp \
    src/formsinfo2/FormInfo2Windows.cpp \
    src/formsinfo2/FormInfo2Process.cpp \
    src/formsinfo2/list/ProcessList.cpp \
    src/formsinfo2/list/WindowProcessList.cpp \
    src/formsinfo1/FormInfoWindows.cpp \
    src/formsinfo1/FormInfoStyles.cpp \
    src/formsinfo1/FormInfoScroll.cpp \
    src/formsinfo1/FormInfoProperties.cpp \
    src/formsinfo1/FormInfoProcess.cpp \
    src/formsinfo1/FormInfoClass.cpp \
    src/formsinfo1/FormInfoGeneral.cpp \
    src/formsinfo1/FormInfoRect.cpp \
    src/MainWindow.cpp \
    src/MenuWindow.cpp \
    src/Languages.cpp \
    src/selector/WindowSelector.cpp \
    src/selector/WindowSelectorRectWidget.cpp \
    src/WindowInfo.cpp \
    src/LinkLabel.cpp \
    src/models/WindowsItemModel.cpp \
    src/models/StylesItemModel.cpp \
    src/models/PropertiesItemModel.cpp \
    src/3rd/WindowFromPointEx.c

FORMS += \
    src/dialogs/DlgFindWindow.ui \
    src/dialogs/DlgAbout.ui \
    src/dialogs/DlgEditSize.ui \
    src/dialogs/DlgEditStyle.ui \
    src/formsinfo2/FormInfo2Windows.ui \
    src/formsinfo2/FormInfo2Process.ui \
    src/formsinfo1/FormInfoWindows.ui \
    src/formsinfo1/FormInfoStyles.ui \
    src/formsinfo1/FormInfoScroll.ui \
    src/formsinfo1/FormInfoProperties.ui \
    src/formsinfo1/FormInfoProcess.ui \
    src/formsinfo1/FormInfoClass.ui \
    src/formsinfo1/FormInfoGeneral.ui \
    src/formsinfo1/FormInfoRect.ui \
    src/MainWindow.ui

#---------------------------------------------------------------------------------------------------
# Archivos de recursos
#---------------------------------------------------------------------------------------------------

RESOURCES += res/resource.qrc

win32:RC_FILE = res/resource_win32.rc

#---------------------------------------------------------------------------------------------------
# Archivos de traducción
#---------------------------------------------------------------------------------------------------
TRANSLATIONS =	translate/wndspy_es.ts \
                translate/wndspy_en.ts \
                translate/wndspy_it.ts
