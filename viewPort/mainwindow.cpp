#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    viewport();
}

MainWindow::~MainWindow()
{

}

void MainWindow::viewport()
{
    //source
    vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New(); //圆锥
    vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New(); //立方体
    vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New(); //圆柱体
    vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New(); //球

    //mapper
    vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper->SetInputConnection(cone->GetOutputPort());
    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputConnection(cube->GetOutputPort());
    vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());
    vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(sphere->GetOutputPort());

    //actor
    vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
    coneActor->SetMapper(coneMapper);
    vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
    cubeActor->SetMapper(cubeMapper);
    vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
    cylinderActor->SetMapper(cylinderMapper);
    vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);

    //render
    vtkSmartPointer<vtkRenderer> coneRender = vtkSmartPointer<vtkRenderer>::New();
    coneRender->AddActor(coneActor);
    vtkSmartPointer<vtkRenderer> cubeRender = vtkSmartPointer<vtkRenderer>::New();
    cubeRender->AddActor(cubeActor);
    vtkSmartPointer<vtkRenderer> cylinderRender = vtkSmartPointer<vtkRenderer>::New();
    cylinderRender->AddActor(cylinderActor);
    vtkSmartPointer<vtkRenderer> sphereRender = vtkSmartPointer<vtkRenderer>::New();
    sphereRender->AddActor(sphereActor);

    //background color
    coneRender->SetBackground(1.0, 0.0, 0.0); //红
    cubeRender->SetBackground(0.0, 1.0, 0.0); //黄
    cylinderRender->SetBackground(0.0, 0.0, 1.0); //蓝
    sphereRender->SetBackground(1.0, 1.0, 0.0); //绿

    //set location
    coneRender->SetViewport(0.0, 0.0, 0.5, 0.5);
    cubeRender->SetViewport(0.5, 0.0, 1.0, 0.5);
    cylinderRender->SetViewport(0.0, 0.5, 0.5, 1.0);
    sphereRender->SetViewport(0.5, 0.5, 1.0, 1.0);

    //render window
    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(coneRender);
    renWin->AddRenderer(cubeRender);
    renWin->AddRenderer(cylinderRender);
    renWin->AddRenderer(sphereRender);
    renWin->SetSize(640, 480);
    renWin->Render();
    renWin->SetWindowName("ViewPort");

    //Render Window Interactor
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renWin);

    //camera
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle(style);

    interactor->Initialize();
    interactor->Start();
}
