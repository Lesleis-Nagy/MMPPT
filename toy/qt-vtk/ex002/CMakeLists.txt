qt_add_executable(qt-vtk-ex002
        MainWindow.ui
        MainWindow.cpp
        main.cpp
)

target_link_libraries(qt-vtk-ex002
        PRIVATE Qt6::Core
                ${VTK_LIBRARIES}
)

target_include_directories(qt-vtk-ex002
        PUBLIC ${VTK_INCLUDE_DIRS}
)
