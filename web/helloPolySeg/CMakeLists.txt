cmake_minimum_required(VERSION 3.10)
project(HelloPolySeg)

if(EMSCRIPTEN)
  find_package(VTK REQUIRED
    COMPONENTS
      vtkIOExport
      vtkCommonCore
      vtkCommonDataModel
      vtkImagingCore
      vtkImagingSources
      vtkFiltersGeometry
      vtkImagingStatistics
      vtkImagingStencil
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

set(hello_SRCS helloPolySeg.cxx)

if(EMSCRIPTEN)
  include(ITKBridgeJavaScript)
  web_add_executable(hello ${hello_SRCS})
  web_target_link_libraries(hello ${ITK_LIBRARIES} /install-prefix/lib/PolySeg/libPolySeg.so)
else()
  add_executable(hello ${hello_SRCS})
  target_link_libraries(hello ${ITK_LIBRARIES} /install-prefix/lib/PolySeg/libPolySeg.so)
endif()
