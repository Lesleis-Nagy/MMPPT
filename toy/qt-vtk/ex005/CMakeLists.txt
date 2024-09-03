set(EXE_NAME "qt-vtk-ex005")

qt_add_executable(${EXE_NAME}
        main_window.ui
        main_window.cpp
        vector_field.cpp
        vector_field_set.cpp
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
