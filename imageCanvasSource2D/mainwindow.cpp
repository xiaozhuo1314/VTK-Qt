#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    imageCanvasSource2D();
}

MainWindow::~MainWindow()
{

}

void MainWindow::imageCanvasSource2D()
{
    vtkSmartPointer<vtkImageCanvasSource2D> canvas = vtkSmartPointer<vtkImageCanvasSource2D>::New();
    canvas->SetScalarTypeToUnsignedChar();   //设置画布的像素数据类型
    canvas->SetNumberOfScalarComponents(1);  //设置像素组分数目
    canvas->SetExtent(0, 100, 0, 100, 0, 0);  //设置画布大小，分别对应min x, max x,min y, max y, min z, max z
    canvas->SetDrawColor(0, 0, 0, 0);
    canvas->FillBox(0, 100, 0, 100);
    canvas->SetDrawColor(255, 0, 0, 0);
    canvas->FillBox(20, 40, 20, 40);
    //canvas->Update();

    //actor
    //因为SetInputData并不是管线，因此它执行的时候不会去跟canvas要数据，因此必须上一步要update
    //要是用管线的话，除了SetInputData改为SetInputConnection、GetOutput改为GetOutputPort，还要加mapper
    vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
    actor->SetInputData(canvas->GetOutput());

    //renderer
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->ResetCamera();
    renderer->SetBackground(1.0, 1.0, 1.0);

    //render window
    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(renderer);
    renWin->SetSize(640, 480);
    renWin->Render();
    renWin->SetWindowName("ImageCanvasSource2D");

    //render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renWin);
    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    interactor->SetInteractorStyle(style);
    interactor->Initialize();
    interactor->Start();
}
