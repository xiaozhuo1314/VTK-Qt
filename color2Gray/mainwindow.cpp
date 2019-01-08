#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    color2gray();
}

MainWindow::~MainWindow()
{

}

void MainWindow::color2gray()
{
    vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/color2Gray/lena.bmp");

    vtkSmartPointer<vtkImageLuminance> luminanceFilter = vtkSmartPointer<vtkImageLuminance>::New();
    luminanceFilter->SetInputConnection(reader->GetOutputPort());   //用SetInputData不行
    luminanceFilter->Update();

    vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
    originalActor->GetMapper()->SetInputConnection(reader->GetOutputPort());  //用SetInputData不行

    vtkSmartPointer<vtkImageActor> shiftscaleActor = vtkSmartPointer<vtkImageActor>::New();
    shiftscaleActor->GetMapper()->SetInputConnection(luminanceFilter->GetOutputPort());  //用SetInputData不行

    double originalViewport[4] = {0.0, 0.0, 0.5, 1.0};
    double shiftscaleViewport[4] = {0.5, 0.0, 1.0, 1.0};

    vtkSmartPointer<vtkRenderer> originalRenderer = vtkSmartPointer<vtkRenderer>::New();
    originalRenderer->SetViewport(originalViewport);
    originalRenderer->AddActor(originalActor);
    originalRenderer->SetBackground(1.0, 1.0, 1.0);
    originalRenderer->ResetCamera();

    vtkSmartPointer<vtkRenderer> shiftscaleRenderer = vtkSmartPointer<vtkRenderer>::New();
    shiftscaleRenderer->SetViewport(shiftscaleViewport);
    shiftscaleRenderer->AddActor(shiftscaleActor);
    shiftscaleRenderer->SetBackground(1.0, 1.0, 1.0);
    shiftscaleRenderer->ResetCamera();

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(originalRenderer);
    renderWindow->AddRenderer(shiftscaleRenderer);
    renderWindow->SetSize(640, 480);
    renderWindow->SetWindowName("Color2GrayImageExample");

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();

    renderWindowInteractor->SetInteractorStyle(style);
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindow->Render();
    renderWindowInteractor->Initialize();

    renderWindowInteractor->Start();
}
