cmake_minimum_required(VERSION 3.16)

project(KnowIt VERSION 0.1.08 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts Network LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts Network LinguistTools)

set(PROJECT_SOURCES
        src/main/main.cpp
        src/main/mainwindow.cpp
        src/main/mainwindow.h
        src/main/mainwindow.ui
        src/main/About/about.h
        src/main/About/about.cpp
        src/main/About/about.ui
        src/main/SplashScreen/splashscreen.h
        src/main/SplashScreen/splashscreen.cpp
        src/main/SplashScreen/splashscreen.ui
        src/ApplicationContainer/FormMineIt/formmineit.h
        src/ApplicationContainer/FormMineIt/formmineit.cpp
        src/ApplicationContainer/FormMineIt/formmineit.ui
        src/ApplicationContainer/FormMineIt/FormProperty/formproperty.h
        src/ApplicationContainer/FormMineIt/FormProperty/formproperty.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/formproperty.ui
        src/ApplicationContainer/FormMineIt/FormProperty/DialogAdd/dialogadd.h
        src/ApplicationContainer/FormMineIt/FormProperty/DialogAdd/dialogadd.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/DialogAdd/dialogadd.ui
        src/ApplicationContainer/FormMineIt/FormProperty/DialogEdit/dialogedit.h
        src/ApplicationContainer/FormMineIt/FormProperty/DialogEdit/dialogedit.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/DialogEdit/dialogedit.ui
        src/ApplicationContainer/FormMineIt/FormProperty/EditID/editid.h
        src/ApplicationContainer/FormMineIt/FormProperty/EditID/editid.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/EditID/editid.ui
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/editinfo.h
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/editinfo.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/editinfo.ui
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/EditContainer/editcontainer.h
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/EditContainer/editcontainer.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/EditContainer/editcontainer.ui
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/EditContainer/PeakInfo/peakinfo.h
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/EditContainer/PeakInfo/peakinfo.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/EditContainer/PeakInfo/peakinfo.ui
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/EditContainer/SingleLineInfo/singlelineinfo.h
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/EditContainer/SingleLineInfo/singlelineinfo.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/EditContainer/SingleLineInfo/singlelineinfo.ui
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/AnnotationsAndTags/annotationsandtags.h
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/AnnotationsAndTags/annotationsandtags.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/AnnotationsAndTags/annotationsandtags.ui
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/BasicInformation/basicinformation.h
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/BasicInformation/basicinformation.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/BasicInformation/basicinformation.ui
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/Originate/originate.h
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/Originate/originate.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/Originate/originate.ui
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/SampleInformation/sampleinformation.h
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/SampleInformation/sampleinformation.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/SampleInformation/sampleinformation.ui
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/SpectralData/spectraldata.h
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/SpectralData/spectraldata.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/SpectralData/spectraldata.ui
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/SpectralAcquisitionConditions/spectralacquisitionconditions.h
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/SpectralAcquisitionConditions/spectralacquisitionconditions.cpp
        src/ApplicationContainer/FormMineIt/FormProperty/EditInfo/SpectralAcquisitionConditions/spectralacquisitionconditions.ui
        src/ApplicationContainer/FormMineIt/FormDataset/formdataset.h
        src/ApplicationContainer/FormMineIt/FormDataset/formdataset.cpp
        src/ApplicationContainer/FormMineIt/FormDataset/formdataset.ui
        src/ApplicationContainer/FormMineIt/FormPlot/formplot.h
        src/ApplicationContainer/FormMineIt/FormPlot/formplot.cpp
        src/ApplicationContainer/FormMineIt/FormPlot/formplot.ui
        src/IndependentWindow/FormSetting/formsetting.h
        src/IndependentWindow/FormSetting/formsetting.cpp
        src/IndependentWindow/FormSetting/formsetting.ui
        src/IndependentWindow/FormSetting/ServerSetting/serversetting.h
        src/IndependentWindow/FormSetting/ServerSetting/serversetting.cpp
        src/IndependentWindow/FormSetting/ServerSetting/serversetting.ui
        src/IndependentWindow/FormSetting/SpectralSetting/spectralsetting.h
        src/IndependentWindow/FormSetting/SpectralSetting/spectralsetting.cpp
        src/IndependentWindow/FormSetting/SpectralSetting/spectralsetting.ui
        src/IndependentWindow/HoverPopup/hoverpopup.h
        src/IndependentWindow/HoverPopup/hoverpopup.cpp
        src/IndependentWindow/HoverPopup/hoverpopup.ui
        src/IndependentWindow/AutoUpdate/autoupdate.h src/IndependentWindow/AutoUpdate/autoupdate.cpp
        src/IndependentWindow/AutoUpdate/autoupdate.ui
        src/IndependentWindow/FormSetting/UpdateSetting/updatesetting.h
        src/IndependentWindow/FormSetting/UpdateSetting/updatesetting.cpp
        src/IndependentWindow/FormSetting/UpdateSetting/updatesetting.ui
        src/ApplicationMenu/formapplicationmenu.h
        src/ApplicationMenu/formapplicationmenu.cpp
        src/ApplicationMenu/formapplicationmenu.ui
        src/components/MyHttp/myhttp.h
        src/components/MyHttp/myhttp.cpp
        src/components/MyChartView/mychartview.h
        src/components/MyChartView/mychartview.cpp
        src/components/MyLog/mylog.h
        src/components/MyLog/mylog.cpp
        src/components/MySetting/mysetting.h
        src/components/MySetting/mysetting.cpp
        src/components/MyGlobal/myglobal.h
        src/components/MyGlobal/myglobal.cpp
        src/global/g_define.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        res.qrc
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET KnowIt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/IndependentWindow"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/ApplicationContainer/FormMineIt"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/ApplicationMenu"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/global"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/components/MyChartView"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/components/MyGlobal"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/components/MyHttp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/components/MyLog"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/components/MySetting"

)

target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Charts
  Qt${QT_VERSION_MAJOR}::Network
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.soymilk.KnowIt)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

add_subdirectory(vendor/spdlog-1.15.0/)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.h
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(TRANSLATIONS
 res/i18n/en.ts
 res/i18n/zh_simple.ts
 res/i18n/zh_traditional.ts
)
qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES)
qt_add_translations(${PROJECT_NAME} TS_FILES ${TRANSLATIONS})

if (WIN32)
    set(APP_ICON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res/icons/KnowIt.ico")

    set(APP_ICON_RC "${CMAKE_CURRENT_BINARY_DIR}/app_icon.rc")
    file(WRITE ${APP_ICON_RC} "IDI_ICON1 ICON \"${APP_ICON_PATH}\"\n")

    target_sources(${PROJECT_NAME} PRIVATE ${APP_ICON_RC})
endif()
