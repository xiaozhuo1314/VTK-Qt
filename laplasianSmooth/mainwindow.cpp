#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    laplasianSmooth();
}

MainWindow::~MainWindow()
{

}

void MainWindow::laplasianSmooth()
{
    //读取数据
    vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/laplasianSmooth/fran_cut.vtk");
    reader->Update();

    vtkSmartPointer<vtkSmoothPolyDataFilter> smoothFilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
    smoothFilter->SetInputConnection(reader->GetOutputPort());
    smoothFilter->SetNumberOfIterations(10);  //控制平滑次数，次数越多平滑的越厉害,就越失真
    smoothFilter->Update();

    vtkSmartPointer<vtkWindowedSincPolyDataFilter> wndSincSmoothFilter = vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
    wndSincSmoothFilter->SetInputConnection(reader->GetOutputPort());
    wndSincSmoothFilter->SetNumberOfIterations(10);  //比上面的更光滑，这个方法是好的,上个方法要想达到相同效果，平滑次数要很多
    wndSincSmoothFilter->Update();

    //mapper
    vtkSmartPointer<vtkPolyDataMapper> inputMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    inputMapper->SetInputConnection(reader->GetOutputPort());

    vtkSmartPointer<vtkPolyDataMapper> smoothedMapper =vtkSmartPointer<vtkPolyDataMapper>::New();
    smoothedMapper->SetInputConnection(smoothFilter->GetOutputPort());

    vtkSmartPointer<vtkPolyDataMapper> wndSincSmoothMapper =vtkSmartPointer<vtkPolyDataMapper>::New();
    wndSincSmoothMapper->SetInputConnection(wndSincSmoothFilter->GetOutputPort());

    //actor
    vtkSmartPointer<vtkActor> inputActor = vtkSmartPointer<vtkActor>::New();
    inputActor->SetMapper(inputMapper);

    vtkSmartPointer<vtkActor> smoothedActor = vtkSmartPointer<vtkActor>::New();
    smoothedActor->SetMapper(smoothedMapper);

    vtkSmartPointer<vtkActor> wndSincSmoothActor = vtkSmartPointer<vtkActor>::New();
    wndSincSmoothActor->SetMapper(wndSincSmoothMapper);

    double leftViewport[4] = { 0.0, 0.0, 0.33, 1.0 };
    double midViewport[4] = { 0.33, 0.0, 0.66, 1.0 };
    double rightViewport[4] = { 0.66, 0.0, 1.0, 1.0 };

    vtkSmartPointer<vtkRenderer> leftRenderer = vtkSmartPointer<vtkRenderer>::New();
    leftRenderer->SetViewport(leftViewport);
    leftRenderer->AddActor(inputActor);
    leftRenderer->SetBackground(1, 0, 0);
    leftRenderer->ResetCamera();

    vtkSmartPointer<vtkRenderer> midRenderer = vtkSmartPointer<vtkRenderer>::New();
    midRenderer->SetViewport(midViewport);
    midRenderer->AddActor(smoothedActor);
    midRenderer->SetBackground(1, 0, 0);
    midRenderer->ResetCamera();

    vtkSmartPointer<vtkRenderer> rightRenderer = vtkSmartPointer<vtkRenderer>::New();
    rightRenderer->SetViewport(rightViewport);
    rightRenderer->AddActor(wndSincSmoothActor);
    rightRenderer->SetBackground(0, 0, 0);
    rightRenderer->ResetCamera();

    vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(leftRenderer);
    rw->AddRenderer(midRenderer);
    rw->AddRenderer(rightRenderer);
    rw->SetSize(640, 320);
    rw->SetWindowName("PolyData Grid Smooth By LapLasian");

    vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(rw);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    rwi->SetInteractorStyle(style);

    rwi->Initialize();
    rwi->Start();
}
