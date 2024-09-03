set(EXE_NAME "qt-vtk-ex004")

qt_add_executable(${EXE_NAME}
        MainWindow.ui
        MainWindow.cpp
        pixel_widget.cpp
        main.cpp
)

set(MOC_GENERATED_INCLUDE_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}_autogen/include")

target_link_libraries(${EXE_NAME}
        PUBLIC Qt6::Core
               ${VTK_LIBRARIES}
)

target_include_directories(${EXE_NAME}
        PUBLIC ${MOC_GENERATED_INCLUDE_DIR}
               ${CMAKE_CURRENT_SOURCE_DIR}
               ${VTK_INCLUDE_DIRS}
)
