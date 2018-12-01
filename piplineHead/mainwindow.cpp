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
    reader->SetFileName("./head.vtk");

    //用移动立方体方法提取等值面,即用立方体划分复杂体
    vtkSmartPointer<vtkMarchingCubes> marchingCubes = vtkSmartPointer<vtkMarchingCubes>::New();
    marchingCubes->SetInputConnection(reader->GetOutputPort());
    marchingCubes->SetValue(0, 500);

    //mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(marchingCubes->GetOutputPort());

    //actor
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    //render
    vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
    render->AddActor(actor);
    render->SetBackground(1.0, 1.0, 1.0);

    //render window
    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(render);
    renWin->SetSize(640, 480);
    renWin->Render();
    renWin->SetWindowName("vtk pipline head");

    //render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renWin);

    interactor->Initialize();
    interactor->Start();
}
