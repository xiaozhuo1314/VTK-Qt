#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    imageAppendComponents();
}

MainWindow::~MainWindow()
{

}

void MainWindow::imageAppendComponents()
{
    vtkSmartPointer<vtkImageCanvasSource2D> red = vtkSmartPointer<vtkImageCanvasSource2D>::New();
    red->SetScalarTypeToUnsignedChar();
    red->SetNumberOfScalarComponents(1);
    red->SetExtent(0, 100, 0, 100, 0, 0);
    red->SetDrawColor(0, 0, 0, 0);
    red->FillBox(0,100,0,100);
    red->SetDrawColor(255, 0, 0, 0);
    red->FillBox(20,40,20,40);
    red->Update();

    vtkSmartPointer<vtkImageCanvasSource2D> green = vtkSmartPointer<vtkImageCanvasSource2D>::New();
    green->SetScalarTypeToUnsignedChar();
    green->SetNumberOfScalarComponents(1);
    green->SetExtent(0, 100, 0, 100, 0, 0);
    green->SetDrawColor(0, 0, 0, 0);
    green->FillBox(0,100,0,100);
    green->SetDrawColor(255, 0, 0, 0);
    green->FillBox(30,50,30,50);
    green->Update();

    vtkSmartPointer<vtkImageCanvasSource2D> blue = vtkSmartPointer<vtkImageCanvasSource2D>::New();
    blue->SetScalarTypeToUnsignedChar();
    blue->SetNumberOfScalarComponents(1);
    blue->SetExtent(0, 100, 0, 100, 0, 0);
    blue->SetDrawColor(0, 0, 0, 0);
    blue->FillBox(0,100,0,100);
    blue->SetDrawColor(255, 0, 0, 0);  //这四个参数什么意思？为什么rgb参数值设置一样?
    blue->FillBox(40,60,40,60);
    blue->Update();

    vtkSmartPointer<vtkImageAppendComponents> appendFilter = vtkSmartPointer<vtkImageAppendComponents>::New();
    appendFilter->SetInputConnection(0, red->GetOutputPort());
    appendFilter->AddInputConnection(0, green->GetOutputPort());
    appendFilter->AddInputConnection(0, blue->GetOutputPort());   //是否是直接填入RGB内存地址
    appendFilter->Update();

    vtkSmartPointer<vtkImageActor> redActor = vtkSmartPointer<vtkImageActor>::New();
    redActor->SetInputData(red->GetOutput());

    vtkSmartPointer<vtkImageActor> greenActor = vtkSmartPointer<vtkImageActor>::New();
    greenActor->SetInputData(green->GetOutput());

    vtkSmartPointer<vtkImageActor> blueActor = vtkSmartPointer<vtkImageActor>::New();
    blueActor->SetInputData(blue->GetOutput());

    vtkSmartPointer<vtkImageActor> combinedActor = vtkSmartPointer<vtkImageActor>::New();
    combinedActor->SetInputData(appendFilter->GetOutput());

    // Define viewport ranges
    // (xmin, ymin, xmax, ymax)
    double redViewport[4] = {0.0, 0.0, 0.25, 1.0};
    double greenViewport[4] = {0.25, 0.0, 0.5, 1.0};
    double blueViewport[4] = {0.5, 0.0, 0.75, 1.0};
    double combinedViewport[4] = {0.75, 0.0, 1.0, 1.0};

    // Setup renderers
    vtkSmartPointer<vtkRenderer> redRenderer = vtkSmartPointer<vtkRenderer>::New();
    redRenderer->SetViewport(redViewport);
    redRenderer->AddActor(redActor);
    redRenderer->ResetCamera();
    redRenderer->SetBackground(1.0, 1.0, 1.0);

    vtkSmartPointer<vtkRenderer> greenRenderer = vtkSmartPointer<vtkRenderer>::New();
    greenRenderer->SetViewport(greenViewport);
    greenRenderer->AddActor(greenActor);
    greenRenderer->ResetCamera();
    greenRenderer->SetBackground(1.0, 1.0, 1.0);

    vtkSmartPointer<vtkRenderer> blueRenderer = vtkSmartPointer<vtkRenderer>::New();
    blueRenderer->SetViewport(blueViewport);
    blueRenderer->AddActor(blueActor);
    blueRenderer->ResetCamera();
    blueRenderer->SetBackground(1.0, 1.0, 1.0);

    vtkSmartPointer<vtkRenderer> combinedRenderer = vtkSmartPointer<vtkRenderer>::New();
    combinedRenderer->SetViewport(combinedViewport);
    combinedRenderer->AddActor(combinedActor);
    combinedRenderer->ResetCamera();
    combinedRenderer->SetBackground(1.0, 1.0, 1.0);

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(redRenderer);
    renderWindow->AddRenderer(greenRenderer);
    renderWindow->AddRenderer(blueRenderer);
    renderWindow->AddRenderer(combinedRenderer);
    renderWindow->SetSize(1200, 300);
    renderWindow->Render();
    renderWindow->SetWindowName("ImageAppendComponentsExample");

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();

    renderWindowInteractor->SetInteractorStyle(style);
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindowInteractor->Initialize();
    renderWindowInteractor->Start();
}
