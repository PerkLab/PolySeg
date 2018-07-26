project(PolySeg)

#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5)
#-----------------------------------------------------------------------------

# --------------------------------------------------------------------------
# Try to find VTK and include its settings (otherwise complain)
FIND_PACKAGE(VTK NO_MODULE REQUIRED PATHS ${VTK_DIR} NO_DEFAULT_PATH)

IF(VTK_FOUND)
  INCLUDE(${VTK_USE_FILE})
ELSE()
  MESSAGE(FATAL_ERROR "This application requires VTK. One of these components is missing. Please verify configuration")
ENDIF()

# --------------------------------------------------------------------------
# Configure headers
# --------------------------------------------------------------------------
set(configure_header_file PolySegConfigure.h)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/${configure_header_file}.in
  ${CMAKE_CURRENT_BINARY_DIR}/${configure_header_file}
  )

# --------------------------------------------------------------------------
# Install headers
# --------------------------------------------------------------------------
if(NOT DEFINED ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
  set(${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT ON)
endif()
if(NOT ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
  file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.(h|txx)")
  install(
    FILES ${headers} ${CMAKE_CURRENT_BINARY_DIR}/${configure_header_file}
    DESTINATION include/${PROJECT_NAME} COMPONENT Development)
endif()

# --------------------------------------------------------------------------
# Sources
# --------------------------------------------------------------------------

set(PolySeg_SRCS
  vtkOrientedImageData.cxx
  vtkOrientedImageData.h
  vtkOrientedImageDataResample.cxx
  vtkOrientedImageDataResample.h
  vtkSegment.cxx
  vtkSegment.h
  vtkSegmentation.cxx
  vtkSegmentation.h
  vtkSegmentationConverter.cxx
  vtkSegmentationConverter.h
  vtkSegmentationConverterFactory.cxx
  vtkSegmentationConverterFactory.h
  vtkSegmentationConverterRule.cxx
  vtkSegmentationConverterRule.h
  vtkSegmentationHistory.cxx
  vtkSegmentationHistory.h
  vtkTopologicalHierarchy.cxx
  vtkTopologicalHierarchy.h
  vtkBinaryLabelmapToClosedSurfaceConversionRule.cxx
  vtkBinaryLabelmapToClosedSurfaceConversionRule.h
  vtkClosedSurfaceToBinaryLabelmapConversionRule.cxx
  vtkClosedSurfaceToBinaryLabelmapConversionRule.h
  vtkCalculateOversamplingFactor.cxx
  vtkCalculateOversamplingFactor.h
  vtkClosedSurfaceToFractionalLabelmapConversionRule.h
  vtkClosedSurfaceToFractionalLabelmapConversionRule.cxx
  vtkFractionalLabelmapToClosedSurfaceConversionRule.h
  vtkFractionalLabelmapToClosedSurfaceConversionRule.cxx
  vtkPolyDataToFractionalLabelmapFilter.h
  vtkPolyDataToFractionalLabelmapFilter.cxx
  )

# Abstract/pure virtual classes

#set_source_files_properties(
#  ABSTRACT
#  )

set(PolySeg_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  CACHE INTERNAL "" FORCE
  )

# --------------------------------------------------------------------------
# Build the library
# --------------------------------------------------------------------------

set(PolySeg_LIBS
  ${VTK_LIBRARIES}
  )

include_directories( ${PolySeg_INCLUDE_DIRS} )
add_library(${PROJECT_NAME} ${PolySeg_SRCS})
target_link_libraries( ${PROJECT_NAME} ${PolySeg_LIBS} )

if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" AND NOT WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

# --------------------------------------------------------------------------
# Folder
# --------------------------------------------------------------------------
if(NOT DEFINED ${PROJECT_NAME}_FOLDER)
  set(${PROJECT_NAME}_FOLDER ${PROJECT_NAME})
endif()
if(NOT "${${PROJECT_NAME}_FOLDER}" STREQUAL "")
  set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${${PROJECT_NAME}_FOLDER})
endif()

# --------------------------------------------------------------------------
# Export target
# --------------------------------------------------------------------------
if(NOT DEFINED ${PROJECT_NAME}_EXPORT_FILE)
  set(${PROJECT_NAME}_EXPORT_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake)
endif()
export(TARGETS ${PROJECT_NAME} APPEND FILE ${${PROJECT_NAME}_EXPORT_FILE})

# --------------------------------------------------------------------------
# Install library
# --------------------------------------------------------------------------
if(NOT DEFINED ${PROJECT_NAME}_INSTALL_BIN_DIR)
  set(${PROJECT_NAME}_INSTALL_BIN_DIR bin)
endif()
if(NOT DEFINED ${PROJECT_NAME}_INSTALL_LIB_DIR)
  set(${PROJECT_NAME}_INSTALL_LIB_DIR lib/${PROJECT_NAME})
endif()

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION ${${PROJECT_NAME}_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
  LIBRARY DESTINATION ${${PROJECT_NAME}_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION ${${PROJECT_NAME}_INSTALL_LIB_DIR} COMPONENT Development
  )

# --------------------------------------------------------------------------
# Python wrapping
# --------------------------------------------------------------------------
if(VTK_WRAP_PYTHON AND BUILD_SHARED_LIBS)

  include(vtkMacroKitPythonWrap)

  vtkMacroKitPythonWrap(
    KIT_NAME ${PROJECT_NAME}
    KIT_SRCS ${PolySeg_SRCS}
    KIT_INSTALL_BIN_DIR ${${PROJECT_NAME}_INSTALL_BIN_DIR}
    KIT_INSTALL_LIB_DIR ${${PROJECT_NAME}_INSTALL_LIB_DIR}
    )

  # Export target
  export(TARGETS ${PROJECT_NAME}Python ${PROJECT_NAME}PythonD APPEND FILE ${${PROJECT_NAME}_EXPORT_FILE})
  # Folder
  if(NOT "${${PROJECT_NAME}_FOLDER}" STREQUAL "")
    set_target_properties(${PROJECT_NAME}Python PROPERTIES FOLDER ${${PROJECT_NAME}_FOLDER})
    set_target_properties(${PROJECT_NAME}PythonD PROPERTIES FOLDER ${${PROJECT_NAME}_FOLDER})
    if(TARGET ${PROJECT_NAME}Hierarchy)
      set_target_properties(${PROJECT_NAME}Hierarchy PROPERTIES FOLDER ${${PROJECT_NAME}_FOLDER})
    endif()
  endif()
endif()

# --------------------------------------------------------------------------
# Testing
# --------------------------------------------------------------------------
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()

# --------------------------------------------------------------------------
# Set INCLUDE_DIRS variable
# --------------------------------------------------------------------------
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
  CACHE INTERNAL "${PROJECT_NAME} include dirs" FORCE)
