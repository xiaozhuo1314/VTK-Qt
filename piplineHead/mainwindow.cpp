#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    piplineHead();
}

MainWindow::~MainWindow()
{

}

void MainWindow::piplineHead()
{
    //读取文件
    vtkSmartPointer<vtkStructuredPointsReader> reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/piplineHead/head.vtk");

    //用移动立方体方法提取等值面,即用立方体划分复杂体
    vtkSmartPointer<vtkMarchingCubes> marchingCubes = vtkSmartPointer<vtkMarchingCubes>::New();
    marchingCubes->SetInputConnection(reader->GetOutputPort());
    marchingCubes->SetValue(0, 1000); //设置第i个等值面的值为value,等值面应该是关心的解剖结构,数字越大越细致

    //mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(marchingCubes->GetOutputPort());

    //actor
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    //render
    vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
    render->AddActor(actor);
    render->SetBackground(1.0, 0.0, 0.0);  //设置舞台背景

    //render window
    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(render);
    renWin->SetSize(640, 480);
    renWin->Render();
    renWin->SetWindowName("vtk pipline head");

    //render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renWin);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle(style);  //控制相机对物体旋转、放大、缩小等操作

    interactor->Initialize();
    interactor->Start();
}
