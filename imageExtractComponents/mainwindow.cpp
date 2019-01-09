#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    imageExtractComponents();
    //imageAppend();
}

MainWindow::~MainWindow()
{

}

void MainWindow::imageExtractComponents()
{
    vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/imageExtractComponents/lena.bmp");
    //reader->Update();

    vtkSmartPointer<vtkImageExtractComponents> redComponent = vtkSmartPointer<vtkImageExtractComponents>::New();
    redComponent->SetInputConnection(reader->GetOutputPort());
    redComponent->SetComponents(0);

    vtkSmartPointer<vtkImageExtractComponents> blueComponent = vtkSmartPointer<vtkImageExtractComponents>::New();
    blueComponent->SetInputConnection(reader->GetOutputPort());
    blueComponent->SetComponents(2);

    vtkSmartPointer<vtkImageExtractComponents> greenComponent = vtkSmartPointer<vtkImageExtractComponents>::New();
    greenComponent->SetInputConnection(reader->GetOutputPort());
    greenComponent->SetComponents(1);

    //写入文件
    char red[128] = {"/home/silence/Project/VTK-Qt/imageExtractComponents/red.bmp"};
    char green[128] = {"/home/silence/Project/VTK-Qt/imageExtractComponents/green.bmp"};
    char blue[128] = {"/home/silence/Project/VTK-Qt/imageExtractComponents/blue.bmp"};

    saveImage(red, redComponent->GetOutputPort());
    saveImage(green, greenComponent->GetOutputPort());
    saveImage(blue, blueComponent->GetOutputPort());

    vtkSmartPointer<vtkImageActor> originActor = vtkSmartPointer<vtkImageActor>::New();
    originActor->GetMapper()->SetInputConnection(reader->GetOutputPort());

    vtkSmartPointer<vtkImageActor> redActor = vtkSmartPointer<vtkImageActor>::New();
    redActor->GetMapper()->SetInputConnection(redComponent->GetOutputPort());

    vtkSmartPointer<vtkImageActor> greenActor = vtkSmartPointer<vtkImageActor>::New();
    greenActor->GetMapper()->SetInputConnection(greenComponent->GetOutputPort());

    vtkSmartPointer<vtkImageActor> blueActor = vtkSmartPointer<vtkImageActor>::New();
    blueActor->GetMapper()->SetInputConnection(blueComponent->GetOutputPort());

    //定义窗口视图
    double originView[4] = { 0.0, 0.0, 0.25, 1.0 };
    double redView[4] = { 0.25, 0.0, 0.5, 1.0 };
    double greenView[4] = { 0.5, 0.0, 0.75, 1.0 };
    double blueView[4] = { 0.75, 0.0, 1.0, 1.0 };

    //render
    vtkSmartPointer<vtkRenderer> originRender = vtkSmartPointer<vtkRenderer>::New();
    originRender->SetViewport(originView);
    originRender->AddActor(originActor);
    originRender->ResetCamera();
    originRender->SetBackground(1.0, 1.0, 1.0);

    vtkSmartPointer<vtkRenderer> redRender = vtkSmartPointer<vtkRenderer>::New();
    redRender->SetViewport(redView);
    redRender->AddActor(redActor);
    redRender->ResetCamera();
    redRender->SetBackground(1.0, 1.0, 1.0);

    vtkSmartPointer<vtkRenderer> greenRender = vtkSmartPointer<vtkRenderer>::New();
    greenRender->SetViewport(greenView);
    greenRender->AddActor(greenActor);
    greenRender->ResetCamera();
    greenRender->SetBackground(1.0, 1.0, 1.0);

    vtkSmartPointer<vtkRenderer> blueRender = vtkSmartPointer<vtkRenderer>::New();
    blueRender->SetViewport(blueView);
    blueRender->AddActor(blueActor);
    blueRender->ResetCamera();
    blueRender->SetBackground(1.0, 1.0, 1.0);


    //render window
    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(originRender);
    renWin->AddRenderer(redRender);
    renWin->AddRenderer(greenRender);
    renWin->AddRenderer(blueRender);
    renWin->SetSize(960, 320);
    renWin->Render();
    renWin->SetWindowName("ExtraImageColorComponents");

    //render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    interactor->SetInteractorStyle(style);
    interactor->SetRenderWindow(renWin);
    interactor->Initialize();
    interactor->Start();
}

void MainWindow::saveImage(char *str, vtkAlgorithmOutput *data)
{
    vtkSmartPointer<vtkBMPWriter> writer = vtkSmartPointer<vtkBMPWriter>::New();
    writer->SetFileName(str);
    writer->SetInputConnection(data);
    writer->Write();
}

void MainWindow::imageAppend()   //有问题，我也不知道怎么回事
{
    vtkSmartPointer<vtkBMPReader> redReader = vtkSmartPointer<vtkBMPReader>::New();
    redReader->SetFileName("/home/silence/Project/VTK-Qt/imageExtractComponents/red.bmp");
    //redReader->Update();
    vtkSmartPointer<vtkBMPReader> greenReader = vtkSmartPointer<vtkBMPReader>::New();
    greenReader->SetFileName("/home/silence/Project/VTK-Qt/imageExtractComponents/green.bmp");
    vtkSmartPointer<vtkBMPReader> blueReader = vtkSmartPointer<vtkBMPReader>::New();
    blueReader->SetFileName("/home/silence/Project/VTK-Qt/imageExtractComponents/blue.bmp");

    vtkSmartPointer<vtkImageAppendComponents> appendFilter = vtkSmartPointer<vtkImageAppendComponents>::New();
    appendFilter->AddInputConnection(redReader->GetOutputPort());
    appendFilter->AddInputConnection(greenReader->GetOutputPort());
    appendFilter->AddInputConnection(blueReader->GetOutputPort());
    vtkSmartPointer<vtkImageActor> combinedActor = vtkSmartPointer<vtkImageActor>::New();
    combinedActor->GetMapper()->SetInputConnection(appendFilter->GetOutputPort());

    vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
    render->AddActor(combinedActor);
    render->ResetCamera();
    render->SetBackground(1.0, 1.0, 1.0);

    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(render);
    renWin->SetSize(640, 480);
    renWin->Render();
    renWin->SetWindowName("AppendImageComponents");

    //char combine[128] = {"/home/silence/Project/VTK-Qt/imageExtractComponents/aa.bmp"};
    //saveImage(combine, appendFilter->GetOutputPort());

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    interactor->SetInteractorStyle(style);
    interactor->SetRenderWindow(renWin);
    interactor->Initialize();
    interactor->Start();
}
