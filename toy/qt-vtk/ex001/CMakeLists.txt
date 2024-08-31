qt_add_executable(qt-vtk-ex001
        MainWindow.ui
        MainWindow.cpp
        main.cpp
)

set(MOC_GENERATED_INCLUDE_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/qt-vtk-ex001_autogen/include")


target_link_libraries(qt-vtk-ex001
        PRIVATE Qt6::Core
                ${VTK_LIBRARIES}
)

target_include_directories(qt-vtk-ex001
        PUBLIC ${MOC_GENERATED_INCLUDE_DIR}
               ${VTK_INCLUDE_DIRS}
)
