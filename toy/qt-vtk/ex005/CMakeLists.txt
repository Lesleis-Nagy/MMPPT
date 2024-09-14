set(EXE_NAME "qt-vtk-ex005")

qt_add_executable(${EXE_NAME}
        main_window.ui
        main_window.cpp
        preferences_dialog.ui
        preferences_dialog.cpp
        pixel_widget.cpp
        model.cpp
        main.cpp
        fraction.cpp
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
