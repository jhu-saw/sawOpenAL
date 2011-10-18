#
# $Id$
#
# (C) Copyright 2005-2011 Johns Hopkins University (JHU), All Rights
# Reserved.
#
# --- begin cisst license - do not edit ---
#
# This software is provided "as is" under an open source license, with
# no warranty.  The complete license can be found in license.txt and
# http://www.cisst.org/cisst/license.txt.
#
# --- end cisst license ---

cmake_minimum_required (VERSION 2.8)

find_package (cisst REQUIRED NO_MODULE)

IF(cisst_FOUND)

    # Modify CMake configuration to use cisst
    include (${CISST_USE_FILE})

    ADD_EXECUTABLE(${PROJECT_NAME}Test
               # code files to compile
               main.cpp
               mtsOpenALTestComponent.h
               mtsOpenALTestComponent.cpp
	       )
		   # this program
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}Test
			${cisstAudio_INCLUDE_LIBS}
			)

    CISST_REQUIRES("${PROJECT_NAME}Test" "cisstCommon;cisstVector;cisstOSAbstraction;cisstMultiTask")


    SET (QT_USE_QTOPENGL 1)

    find_package(Qt4 REQUIRED)
    include(${QT_USE_FILE})


    qt4_wrap_cpp(QT_WRAPPED_CPP
                 mtsOpenALRecordQtComponent.h
                 mtsOpenALPlayQtComponent.h
                 )

    qt4_wrap_ui(QT_WRAPPED_UI
                mtsOpenALPlayQtWidget.ui
                mtsOpenALRecordQtWidget.ui
                )

    INCLUDE(../../cmake/FindQwt5.cmake)

    INCLUDE_DIRECTORIES( ${Qwt5_INCLUDE_DIR} )

    ADD_EXECUTABLE(${PROJECT_NAME}GUI
               # code files to compile
               mainGUI.cpp
               mtsOpenALRecordQtComponent.h
               mtsOpenALRecordQtComponent.cpp
               mtsOpenALPlayQtComponent.h
               mtsOpenALPlayQtComponent.cpp
               ${QT_WRAPPED_CPP}
               ${QT_WRAPPED_UI}

           )
           # this program
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}GUI
            ${cisstAudio_INCLUDE_LIBS}
            ${QT_LIBRARIES}
            ${Qwt5_Qt4_TENTATIVE_LIBRARY}
            )

    CISST_REQUIRES("${PROJECT_NAME}GUI" "cisstCommon;cisstVector;cisstVectorQt;cisstOSAbstraction;cisstMultiTask")



    ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)

ENDIF(cisst_FOUND)
