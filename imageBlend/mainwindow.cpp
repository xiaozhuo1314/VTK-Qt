#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    imageBlend();
}

MainWindow::~MainWindow()
{

}

void MainWindow::imageBlend()
{
    //读取图像
    vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/imageBlend/lena-gray.jpg");
    reader->Update();
    reader->GetOutput();

    //生成二值图像
    vtkSmartPointer<vtkImageCanvasSource2D> canvas = vtkSmartPointer<vtkImageCanvasSource2D>::New();
    canvas->SetScalarTypeToUnsignedChar();
    canvas->SetNumberOfScalarComponents(1);
    canvas->SetExtent(0, 512, 0, 512, 0, 0);
    canvas->SetDrawColor(0.0);
    canvas->FillBox(0, 512, 0, 512);
    canvas->SetDrawColor(255.0);
    canvas->FillBox(100, 400, 100, 400);
    canvas->Update();

    //融合
    vtkSmartPointer<vtkImageBlend> blend = vtkSmartPointer<vtkImageBlend>::New();
    blend->AddInputData(reader->GetOutput());
    blend->AddInputData(canvas->GetOutput());
    blend->SetOpacity(0, 0.4);
    blend->SetOpacity(1, 0.6);
    blend->Update();

    //actor
    vtkSmartPointer<vtkImageActor> actor1 = vtkSmartPointer<vtkImageActor>::New();
    actor1->SetInputData(reader->GetOutput());
    vtkSmartPointer<vtkImageActor> actor2 = vtkSmartPointer<vtkImageActor>::New();
    actor2->SetInputData(canvas->GetOutput());
    vtkSmartPointer<vtkImageActor> actor3 = vtkSmartPointer<vtkImageActor>::New();
    actor3->SetInputData(blend->GetOutput());

    //定义视图范围
    double leftView[4] = {0.0, 0.0, 0.33, 1.0};
    double midView[4] = {0.33, 0.0, 0.66, 1.0};
    double rightView[4] = {0.66, 0.0, 1.0, 1.0};

    //render
    vtkSmartPointer<vtkRenderer> render1 = vtkSmartPointer<vtkRenderer>::New();
    render1->AddActor(actor1);
    render1->SetViewport(leftView);
    render1->ResetCamera();
    render1->SetBackground(1.0, 1.0, 1.0);
    vtkSmartPointer<vtkRenderer> render2 = vtkSmartPointer<vtkRenderer>::New();
    render2->AddActor(actor2);
    render2->SetViewport(midView);
    render2->ResetCamera();
    render2->SetBackground(1.0, 1.0, 1.0);
    vtkSmartPointer<vtkRenderer> render3 = vtkSmartPointer<vtkRenderer>::New();
    render3->AddActor(actor3);
    render3->SetViewport(rightView);
    render3->ResetCamera();
    render3->SetBackground(1.0, 1.0, 1.0);

    //render window
    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->SetSize(640, 480);
    renWin->SetWindowName("ImageBlendExample");
    renWin->AddRenderer(render1);
    renWin->AddRenderer(render2);
    renWin->AddRenderer(render3);
    renWin->Render();

    //render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    interactor->SetInteractorStyle(style);
    interactor->SetRenderWindow(renWin);
    interactor->Initialize();
    interactor->Start();
}
