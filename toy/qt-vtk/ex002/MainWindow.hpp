//
// Created by Lesleis Nagy on 23/07/2024.
//

#ifndef VCOMPARE_TOY_QT_VTK_MAINWINDOWS_HPP_
#define VCOMPARE_TOY_QT_VTK_MAINWINDOWS_HPP_

#include <QMainWindow>

#include "vtkCamera.h"
#include "vtkCylinderSource.h"
#include "vtkPlaneSource.h"
#include "vtkProperty.h"
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCubeSource.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>

#include "ui_MainWindow.h"

namespace {
    // Handle mouse events.
    class MouseInteractorStyle5 : public vtkInteractorStyleTrackballActor
    {
    public:
        static MouseInteractorStyle5* New();
        vtkTypeMacro(MouseInteractorStyle5, vtkInteractorStyleTrackballActor);

        void OnLeftButtonDown() override {
            vtkInteractorStyleTrackballActor::OnLeftButtonDown();

            if (this->InteractionProp == this->cylinder) {
                std::cout << "Picked cylinder." << std::endl;
            }
            else if (this->InteractionProp == this->plane) {
                std::cout << "Picked plane." << std::endl;
            }
        }

        vtkActor* cylinder;
        vtkActor* plane;
    };
    vtkStandardNewMacro(MouseInteractorStyle5);
}

class MainWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

public:

    using PtrCylinderSource = vtkSmartPointer<vtkCylinderSource>;
    using PtrPlaneSource = vtkSmartPointer<vtkPlaneSource>;
    using PtrPolyDataMapper = vtkSmartPointer<vtkPolyDataMapper>;
    using PtrActor = vtkSmartPointer<vtkActor>;
    using PtrRenderer = vtkSmartPointer<vtkRenderer>;
    using PtrRenderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>;
    using PtrMouseInteractorStyle5 = vtkSmartPointer<MouseInteractorStyle5>;
    using PtrTrackballCameraStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>;

    MainWindow();

    ~MainWindow() override = default;

public slots:

    void slot_btn_toggle_clicked();

private:

    enum CameraMode {
        OBJECT_MODE,
        WORLD_MODE
    };

    double _bg_red;
    double _bg_green;
    double _bg_blue;

    double _actor_red;
    double _actor_green;
    double _actor_blue;

    CameraMode _camera_mode = WORLD_MODE;

    PtrCylinderSource _cylinder_source;
    PtrPolyDataMapper _cylinder_poly_data_mapper;
    PtrPlaneSource _plane_source;
    PtrPolyDataMapper _plane_poly_data_mapper;

    PtrRenderer _renderer;

    PtrActor _cylinder_actor;
    PtrActor _plane_actor;

    PtrRenderWindowInteractor _render_window_interactor;

    PtrMouseInteractorStyle5 _object_interaction_style;
    PtrTrackballCameraStyle _world_interaction_style;

    void
    display_camera_mode();
};


#endif // VCOMPARE_TOY_QT_VTK_MAINWINDOWS_HPP_
