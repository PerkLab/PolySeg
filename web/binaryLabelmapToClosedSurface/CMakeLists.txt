cmake_minimum_required(VERSION 3.10)
project(BinaryLabelmapToClosedSurface)

if(EMSCRIPTEN)
  find_package(VTK REQUIRED
    COMPONENTS
      vtkCommonCore
      vtkCommonDataModel
      vtkImagingCore
      vtkImagingSources
      vtkFiltersGeometry
      vtkImagingStatistics
      vtkImagingStencil
      vtkIOExport
    )
else()
  find_package(VTK REQUIRED)
endif()

include(${VTK_USE_FILE})

set(io_components ITKImageIO)
if(EMSCRIPTEN)
  set(io_components BridgeJavaScript)
endif()

if(EMSCRIPTEN)
  find_package(ITK REQUIRED
    COMPONENTS
      ITKIOMeshBase
      ITKVtkGlue
      ${io_components}
    )
else()
  find_package(ITK REQUIRED
    COMPONENTS
      ${io_components}
    )
endif()

include(${ITK_USE_FILE})

set(hello_SRCS binaryLabelmapToClosedSurface.cxx)

if(EMSCRIPTEN)
  include(ITKBridgeJavaScript)
  web_add_executable(hello ${hello_SRCS})
  web_target_link_libraries(hello ${ITK_LIBRARIES} ${VTK_LIBRARIES} /install-prefix/lib/PolySeg/libPolySeg.a)
else()
  add_executable(hello ${hello_SRCS})
  target_link_libraries(hello ${ITK_LIBRARIES} ${VTK_LIBRARIES} /install-prefix/lib/PolySeg/libPolySeg.a)
endif()

enable_testing()
add_test(NAME binaryLabelmapToClosedSurface
  COMMAND binaryLabelmapToClosedSurface ${CMAKE_CURRENT_BINARY_DIR}/Segmentation-label.nii 
  ${CMAKE_CURRENT_BINARY_DIR}/Segmentation.vtk.json
  )