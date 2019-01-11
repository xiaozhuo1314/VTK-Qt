#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    connectedCompExtract();
}

MainWindow::~MainWindow()
{

}

void MainWindow::connectedCompExtract()
{
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(10);
    sphereSource->SetThetaResolution(10);   //设置经线方向点数
    sphereSource->SetPhiResolution(10);    //设置纬线方向点数
    sphereSource->Update();

    vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New(); //圆锥
    coneSource->SetRadius(5);
    coneSource->SetHeight(10);
    coneSource->SetCenter(25, 0, 0);
    coneSource->Update();

    //合并数据
    vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
    appendFilter->AddInputData(sphereSource->GetOutput());
    appendFilter->AddInputData(coneSource->GetOutput());
    appendFilter->Update();

    //连通区域分析
    //SetExtractionModeToLargestRegion()：用于提取具有最多点数的连通区域；
    //SetExtractionModeToAllRegions()：该模式主要用于连通区域标记，配合函数ColorRegionsOn()使用，在连通区域像是的同时，生成一个名为RegionId的点属性数据。
    //SetExtractionModeToSpecifiedRegions()：该模式用于提取一个或多个连通区域，在该模式下，需要通过AddSpecifiedRegion()来添加需要提取的区域号，区域号从零开始。
    //SetExtractionModeToClosestPointRegion()：该模式需要使用SetClosestPoint()函数设置一个空间点坐标，执行结果为离该点最近的连通区域。
    //SetExtractionModeToPointSeededRegions()：该模式下需要使用AddSeed()函数添加种子点，提取种子点所在的区域。
    //SetExtractionModeToCellSeededRegions()：该模式下需要使用AddSeed()函数添加种子单元，提取种子单元所在的区域。
    vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
    connectivityFilter->SetInputData(appendFilter->GetOutput());
    connectivityFilter->SetExtractionModeToCellSeededRegions();
    connectivityFilter->AddSeed(100);
    connectivityFilter->Update();

    vtkSmartPointer<vtkPolyDataMapper> originalMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    originalMapper->SetInputConnection(appendFilter->GetOutputPort());
    originalMapper->Update();
    vtkSmartPointer<vtkActor> originalActor = vtkSmartPointer<vtkActor>::New();
    originalActor->SetMapper(originalMapper);

    vtkSmartPointer<vtkPolyDataMapper> extractedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    extractedMapper->SetInputConnection(connectivityFilter->GetOutputPort());
    extractedMapper->Update();
    vtkSmartPointer<vtkActor> extractedActor = vtkSmartPointer<vtkActor>::New();
    extractedActor->SetMapper(extractedMapper);

    double leftViewport[4] = { 0.0, 0.0, 0.5, 1.0 };
    double rightViewport[4] = { 0.5, 0.0, 1.0, 1.0 };

    vtkSmartPointer<vtkRenderer> leftRenderer = vtkSmartPointer<vtkRenderer>::New();
    leftRenderer->SetViewport(leftViewport);
    leftRenderer->AddActor(originalActor);
    leftRenderer->SetBackground(1, 0, 0);

    vtkSmartPointer<vtkRenderer> rightRenderer = vtkSmartPointer<vtkRenderer>::New();
    rightRenderer->SetViewport(rightViewport);
    rightRenderer->AddActor(extractedActor);
    rightRenderer->SetBackground(0, 0, 0);

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(leftRenderer);
    renderWindow->AddRenderer(rightRenderer);
    renderWindow->SetSize(640, 320);
    renderWindow->Render();
    renderWindow->SetWindowName("PolyDataConnectedCompExtract");

    leftRenderer->ResetCamera();
    rightRenderer->SetActiveCamera(leftRenderer->GetActiveCamera());

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);
    interactor->Initialize();
    interactor->Start();
}
