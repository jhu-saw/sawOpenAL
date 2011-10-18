#
# $Id$
#
# (C) Copyright 2011 Johns Hopkins University (JHU), All Rights
# Reserved.
#
# --- begin cisst license - do not edit ---
#
# This software is provided "as is" under an open source license, with
# no warranty.  The complete license can be found in license.txt and
# http://www.cisst.org/cisst/license.txt.
#
# --- end cisst license ---

# Core library
set (REQUIRED_CISST_LIBRARIES cisstCommon
                              cisstVector
                              cisstOSAbstraction
                              cisstMultiTask
                              cisstParameterTypes)

find_package (cisst REQUIRED ${REQUIRED_CISST_LIBRARIES})

if (cisst_FOUND_AS_REQUIRED)

  # load cisst configuration
  include (${CISST_USE_FILE})

  add_library (sawOpenAL
               ../sawOpenAL.h
               ../sawOpenALExport.h
               mtsOpenALPlay.cpp
               ../mtsOpenALPlay.h
               mtsOpenALRecord.cpp
               ../mtsOpenALRecord.h)

  target_link_libraries (sawOpenAL "${OPENAL_LIBRARY}")
  cisst_target_link_libraries (sawOpenAL ${REQUIRED_CISST_LIBRARIES})

  # Qt widgets
  if (CISST_HAS_QT)

    # initial find_package doesn't have cisstQt
    cisst_set_directories (cisstQt)

    # for Qt generated header files
    include_directories (${CMAKE_CURRENT_BINARY_DIR})
    qt4_wrap_cpp (SAW_OPENAL_QT_COMPONENTS_GENERATED
                  ../mtsOpenALPlayQtComponent.h
                  ../mtsOpenALRecordQtComponent.h)
    qt4_wrap_ui (SAW_OPENAL_QT_WIDGETS_GENERATED
                 mtsOpenALPlayQtWidget.ui
                 mtsOpenALRecordQtWidget.ui)

    add_library (sawOpenALQt
                 ${SAW_OPENAL_QT_COMPONENTS_GENERATED}
                 ${SAW_OPENAL_QT_WIDGETS_GENERATED}
                 ../sawOpenALExportQt.h
                 mtsOpenALPlayQtComponent.cpp
                 ../mtsOpenALPlayQtComponent.h
                 mtsOpenALRecordQtComponent.cpp
                 ../mtsOpenALRecordQtComponent.h)

    target_link_libraries (sawOpenALQt ${QT_LIBRARIES})
    cisst_target_link_libraries (sawOpenALQt ${REQUIRED_CISST_LIBRARIES} cisstVectorQt cisstQt)
  endif (CISST_HAS_QT)


  # create/configure file for find_package (sawOpenAL)
  set (sawOpenAL_INCLUDE_DIR "${sawComponents_SOURCE_DIR}" "${sawComponents_BINARY_DIR}" "${OPENAL_INCLUDE_DIR}")
  set (sawOpenAL_LIBRARY_DIR "${LIBRARY_OUTPUT_PATH}")
  set (sawOpenAL_LIBRARIES sawOpenAL "${OPENAL_LIBRARY}")
  set (sawOpenALQt_LIBRARIES sawOpenALQt)

  configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/sawOpenALConfig.cmake.in"
                  "${sawComponents_BINARY_DIR}/sawOpenALConfig.cmake"
                   @ONLY@)

else (cisst_FOUND_AS_REQUIRED)
  message ("Information: code in ${CMAKE_CURRENT_SOURCE_DIR} will not be compiled, it requires ${REQUIRED_CISST_LIBRARIES}")
endif (cisst_FOUND_AS_REQUIRED)
