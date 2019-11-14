set(KIT PolySeg)

create_test_sourcelist(Tests ${KIT}CxxTests.cxx
  vtkSegmentationTest1.cxx
  vtkSegmentationTest2.cxx
  vtkSegmentationConverterTest1.cxx
  vtkClosedSurfaceToFractionalLabelMapConversionTest1.cxx
  )

add_executable(${KIT}CxxTests ${Tests})
target_link_libraries(${KIT}CxxTests ${PROJECT_NAME})
set_target_properties(${KIT}CxxTests PROPERTIES FOLDER ${${PROJECT_NAME}_FOLDER})

macro(simple_test TEST_NAME)
  add_test(${TEST_NAME}
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
    ${TEST_NAME}
    )
endmacro()

simple_test( vtkSegmentationTest1 )
simple_test( vtkSegmentationTest2 )
simple_test( vtkSegmentationConverterTest1 )
simple_test( vtkClosedSurfaceToFractionalLabelMapConversionTest1 )
