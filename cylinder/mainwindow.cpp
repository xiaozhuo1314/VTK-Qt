#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    cylinder();
}

MainWindow::~MainWindow()
{

}

void MainWindow::cylinder()
{
    //source
    vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
    cylinder->SetHeight(3.0);
    cylinder->SetRadius(1.0);
    cylinder->SetResolution(20);   //圆周是由里面数字个边形组成，如本例是20边形组成的近似圆周

    //映射
    vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    //actor
    vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
    cylinderActor->SetMapper(cylinderMapper);
    //cylinderActor->GetProperty()->SetColor(1.0, 0.0, 0.0); //设圆柱为红色

    //render
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(cylinderActor);
    renderer->SetBackground(0.0, 0.0, 0.0); //背景为黑色

    //renderwindow
    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(renderer);
    renWin->SetSize(640, 480);
    renWin->Render();  //这句话注释掉会有其它表现
    renWin->SetWindowName("Cylinder");

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renWin);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle(style);  //控制相机对物体旋转、放大、缩小等操作

    //增加不同灯光使圆柱体展现不同颜色
    vtkSmartPointer<vtkLight> light1 = vtkSmartPointer<vtkLight>::New();
    light1->SetColor(0,1,0);
    light1->SetPosition(0, 0, 1);
    light1->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());
    renderer->AddLight(light1);

    vtkSmartPointer<vtkLight> light2 = vtkSmartPointer<vtkLight>::New();
    light2->SetColor(0,0,1);
    light2->SetPosition(0, 0, -1);
    light2->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());
    renderer->AddLight(light2);

    vtkSmartPointer<vtkLight> light3 = vtkSmartPointer<vtkLight>::New();
    light3->SetColor(1,0,0);
    light3->SetPosition(0, 1, 0);
    light3->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());
    renderer->AddLight(light3);

    vtkSmartPointer<vtkLight> light4 = vtkSmartPointer<vtkLight>::New();
    light4->SetColor(1,0,0);
    light4->SetPosition(0, -1, 0);
    light4->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());
    renderer->AddLight(light4);

    interactor->Initialize();
    interactor->Start();
}
