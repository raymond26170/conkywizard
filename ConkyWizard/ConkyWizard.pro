TARGET = ConkyWizard
TRANSLATIONS = translations/ConkyWizard_es.ts
TEMPLATE = app
DESTDIR = ../Application
SOURCES += src/main.cpp \
	src/Wizard.cpp \
	src/ImageBackground.cpp \
	src/WidgetConky.cpp \
	src/DockConky.cpp \
	src/WidgetTitle.cpp \
	src/WidgetNormal.cpp \
	src/WidgetJump.cpp \
	src/WidgetSpace.cpp \
	src/DialogNewEdit.cpp \
	src/WidgetModel.cpp \
    src/DialogLoadFile.cpp
HEADERS += src/Wizard.h \
	src/ImageBackground.h \
	src/WidgetConky.h \
	src/DockConky.h \
	src/WidgetTitle.h \
	src/WidgetNormal.h \
	src/WidgetJump.h \
	src/WidgetSpace.h \
	src/DialogNewEdit.h \
	src/WidgetModel.h \
    src/DialogLoadFile.h
FORMS += src/Wizard.ui \
	src/DialogNewEdit.ui \
    src/DialogLoadFile.ui
RESOURCES += resources/ConkyWizard.qrc
OTHER_FILES +=
