#
# $Id$
#
# (C) Copyright 2011-2012 Johns Hopkins University (JHU), All Rights
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

  find_package (OpenAL REQUIRED)

  if (OPENAL_FOUND)

    # create/configure file for find_package (sawOpenAL)
    set (sawOpenAL_INCLUDE_DIR "${sawOpenAL_SOURCE_DIR}/include"
                               "${CMAKE_CURRENT_BINARY_DIR}"
                               "${OPENAL_INCLUDE_DIR}")
    set (sawOpenAL_LIBRARY_DIR "${LIBRARY_OUTPUT_PATH}")
    set (sawOpenAL_LIBRARIES sawOpenAL ${OPENAL_LIBRARY})
    set (sawOpenALQt_LIBRARIES ${sawOpenAL_LIBRARIES} sawOpenALQt)

    include_directories (${sawOpenAL_INCLUDE_DIR})
    set (sawOpenAL_HEADER_DIR "${sawOpenAL_SOURCE_DIR}/include/sawOpenAL")
    add_library (sawOpenAL
                 ${sawOpenAL_HEADER_DIR}/osaOpenAL.h
                 ${sawOpenAL_HEADER_DIR}/sawOpenAL.h
                 ${sawOpenAL_HEADER_DIR}/sawOpenALExport.h
                 osaOpenAL.cpp
                 mtsOpenALPlay.cpp
                 ${sawOpenAL_HEADER_DIR}/mtsOpenALPlay.h
                 mtsOpenALRecord.cpp
                 ${sawOpenAL_HEADER_DIR}/mtsOpenALRecord.h)

    target_link_libraries (sawOpenAL "${OPENAL_LIBRARY}")
    cisst_target_link_libraries (sawOpenAL ${REQUIRED_CISST_LIBRARIES})

    # Qt widgets
    if (CISST_HAS_QT)

      # initial find_package doesn't have cisstQt
      cisst_set_directories (cisstQt)

      # for Qt generated header files
      include_directories (${CMAKE_CURRENT_BINARY_DIR})
      qt4_wrap_cpp (SAW_OPENAL_QT_COMPONENTS_GENERATED
                    ${sawOpenAL_HEADER_DIR}/mtsOpenALPlayQtComponent.h
                    ${sawOpenAL_HEADER_DIR}/mtsOpenALRecordQtComponent.h
                    OPTIONS )
      qt4_wrap_ui (SAW_OPENAL_QT_WIDGETS_GENERATED
                   mtsOpenALPlayQtWidget.ui
                   mtsOpenALRecordQtWidget.ui)

      add_library (sawOpenALQt
                   ${SAW_OPENAL_QT_COMPONENTS_GENERATED}
                   ${SAW_OPENAL_QT_WIDGETS_GENERATED}
                   ${sawOpenAL_HEADER_DIR}/sawOpenALExportQt.h
                   mtsOpenALPlayQtComponent.cpp
                   ${sawOpenAL_HEADER_DIR}/mtsOpenALPlayQtComponent.h
                   mtsOpenALRecordQtComponent.cpp
                   ${sawOpenAL_HEADER_DIR}/mtsOpenALRecordQtComponent.h)

      target_link_libraries (sawOpenALQt ${QT_LIBRARIES})
      cisst_target_link_libraries (sawOpenALQt ${REQUIRED_CISST_LIBRARIES} cisstVectorQt cisstQt)

      install (TARGETS sawOpenALQt
               RUNTIME DESTINATION bin
               LIBRARY DESTINATION lib
               ARCHIVE DESTINATION lib)

    endif (CISST_HAS_QT)

    configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/sawOpenALConfig.cmake.in"
                    "${sawOpenAL_BINARY_DIR}/sawOpenALConfig.cmake"
                     @ONLY@)

    # Install target for headers and library
    install (DIRECTORY
             "${sawOpenAL_SOURCE_DIR}/include/sawOpenAL"
             DESTINATION include
             PATTERN .svn EXCLUDE)

    install (TARGETS sawOpenAL
             RUNTIME DESTINATION bin
             LIBRARY DESTINATION lib
             ARCHIVE DESTINATION lib)

    install (FILES "${sawOpenAL_BINARY_DIR}/sawOpenALConfig.cmake"
             DESTINATION "${CISST_CMAKE_INSTALL_SUFFIX}/saw")

  else (OPENAL_FOUND)
    message (SEND_ERROR "code in ${CMAKE_CURRENT_SOURCE_DIR} requires OpenAL.")
  endif (OPENAL_FOUND)

else (cisst_FOUND_AS_REQUIRED)
  message ("Information: code in ${CMAKE_CURRENT_SOURCE_DIR} will not be compiled, it requires ${REQUIRED_CISST_LIBRARIES}")
endif (cisst_FOUND_AS_REQUIRED)
