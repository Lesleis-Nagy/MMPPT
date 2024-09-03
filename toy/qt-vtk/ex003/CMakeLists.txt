set(EXE_NAME "qt-vtk-ex003")

qt_add_executable(${EXE_NAME}
        MainWindow.ui
        MainWindow.cpp
        main.cpp
)

set(MOC_GENERATED_INCLUDE_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}_autogen/include")

target_link_libraries(${EXE_NAME}
        PUBLIC Qt6::Core
               qcustomplot
               ${VTK_LIBRARIES}
)

target_include_directories(${EXE_NAME}
        PUBLIC ${MOC_GENERATED_INCLUDE_DIR}
               ${QCUSTOMPLOT_INCLUDE_DIR}
               ${VTK_INCLUDE_DIRS}
)
