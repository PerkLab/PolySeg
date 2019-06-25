cmake_minimum_required(VERSION 3.10)
project(HelloPolySeg)

find_package(VTK REQUIRED
  COMPONENTS
    vtkIOExport
    vtkFiltersGeometry
  )
include(${VTK_USE_FILE})

set(io_components ITKImageIO)
if(EMSCRIPTEN)
  set(io_components BridgeJavaScript)
endif()

find_package(ITK REQUIRED
  COMPONENTS
    ITKIOMeshBase
    ITKVtkGlue
    ${io_components}
  )
include(${ITK_USE_FILE})

set(hello_SRCS helloPolySeg.cxx)
if(EMSCRIPTEN)
  include(ITKBridgeJavaScript)
  web_add_executable(hello ${hello_SRCS})
  web_target_link_libraries(hello ${VTK_LIBRARIES} ${ITK_LIBRARIES} /install-prefix/lib/PolySeg/libPolySeg.a)
else()
  add_executable(hello ${hello_SRCS})
  target_link_libraries(hello ${VTK_LIBRARIES} ${ITK_LIBRARIES} /install-prefix/lib/PolySeg/libPolySeg.a)
endif()
