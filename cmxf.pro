#-------------------------------------------------
#
# Project created by QtCreator 2016-03-08T21:04:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Transfer_Tool
TEMPLATE = app

RC_FILE = cmxf.rc
RESOURCES = cmxf.qrc

# The application version
VERSION = 1.1.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32 {
    DEFINES += BUILDTIME=\\\"$$system('echo %time%')\\\"
    DEFINES += BUILDDATE=\\\"$$system('echo %date%')\\\"
} else {
    DEFINES += BUILDTIME=\\\"$$system(date '+%H:%M')\\\"
    DEFINES += BUILDDATE=\\\"$$system(date '+%d/%m/%y')\\\"
}

SOURCES += main.cpp\
        mainwindow.cpp \
    database/database.cpp \
    database/index.cpp \
    settings.cpp \
    models/index_model.cpp \
    database/city.cpp \
    data_types/city_weather.cpp \
    data_types/string.cpp \
    database/club.cpp \
    database/club_comp.cpp \
    database/club_comp_history.cpp \
    database/colour.cpp \
    database/continent.cpp \
    database/name.cpp \
    database/nation.cpp \
    database/stadium.cpp \
    database/staff_comp.cpp \
    database/staff_comp_history.cpp \
    database/staff_history.cpp \
    database/official.cpp \
    data_types/date.cpp \
    database/non_player.cpp \
    database/player.cpp \
    database/staff.cpp \
    database/staff_preferences.cpp \
    importer/transfer_importer.cpp \
    data_types/data_pointer.cpp \
    data_types/ptr_club.cpp \
    data_types/ptr_club_comp.cpp \
    data_types/ptr_name.cpp \
    data_types/ptr_staff.cpp \
    data_types/staff_classification.cpp \
    data_types/cm_char.cpp \
    spreadsheet/spreadsheet.cpp \
    progress_window.cpp \
    data_types/year.cpp \
    importer/vlookup_hash.cpp \
    ui/summary_item.cpp \
    ui/style_sheet.cpp \
    data_types/ptr_nation.cpp \
    data_types/job.cpp \
    data_types/attribute.cpp \
    data_types/ptr_player.cpp \
    data_types/ptr_non_player.cpp \
    data_types/cm_int.cpp \
    data_types/cm_uchar.cpp \
    exporter/exporter.cpp \
    exporter/exporter_item.cpp \
    exporter/exporter_model.cpp \
    data_types/cm_short.cpp \
    data_types/reputation.cpp \
    data_types/wage.cpp \
    data_types/ptr_preferences.cpp \
    data_types/ability.cpp \
    data_types/squad.cpp \
    data_types/player_squad_number.cpp

HEADERS  += mainwindow.h \
    database/database.h \
    database/index.h \
    data_types/data_types.h \
    settings.h \
    models/index_model.h \
    database/city.h \
    data_types/city_weather.h \
    data_types/weather.h \
    data_types/string.h \
    database/club.h \
    database/club_comp.h \
    database/club_comp_history.h \
    database/colour.h \
    database/continent.h \
    database/name.h \
    database/nation.h \
    database/stadium.h \
    database/staff_comp.h \
    database/staff_comp_history.h \
    database/staff_history.h \
    database/official.h \
    data_types/date.h \
    database/non_player.h \
    database/player.h \
    database/staff.h \
    database/staff_preferences.h \
    importer/transfer_importer.h \
    data_types/data_pointer.h \
    data_types/ptr_club.h \
    data_types/ptr_club_comp.h \
    data_types/ptr_name.h \
    data_types/ptr_staff.h \
    data_types/staff_classification.h \
    data_types/cm_char.h \
    spreadsheet/spreadsheet.h \
    progress_window.h \
    data_types/year.h \
    importer/vlookup_hash.h \
    ui/summary_item.h \
    ui/style_sheet.h \
    data_types/ptr_nation.h \
    data_types/job.h \
    data_types/attribute.h \
    data_types/ptr_player.h \
    data_types/ptr_non_player.h \
    data_types/cm_int.h \
    data_types/cm_uchar.h \
    exporter/exporter.h \
    exporter/exporter_item.h \
    exporter/exporter_model.h \
    data_types/cm_short.h \
    data_types/reputation.h \
    data_types/wage.h \
    data_types/ptr_preferences.h \
    data_types/ability.h \
    data_types/squad.h \
    data_types/player_squad_number.h

FORMS    += mainwindow.ui
