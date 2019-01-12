#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    modelExtract();  //模型提取
}

MainWindow::~MainWindow()
{

}

void MainWindow::modelExtract()
{
    //三个提取类vtkDecimatePro、vtkQuadricDecimation、vtkQuadricClustering
    vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/modelExtract/fran_cut.vtk");
    reader->Update();

    vtkSmartPointer<vtkPolyData> original = reader->GetOutput();
    qDebug()<< "抽取前"<< "-----------------------";
    qDebug()<< "模型点数为： " << original->GetNumberOfPoints();
    qDebug()<< "模型面数为： " << original->GetNumberOfPolys();

    vtkSmartPointer<vtkDecimatePro> decimation = vtkSmartPointer<vtkDecimatePro>::New();
    decimation->SetInputData(reader->GetOutput());
    decimation->SetTargetReduction(0.6);  //减少60%的点
    decimation->Update();

    vtkSmartPointer<vtkPolyData> decimated = decimation->GetOutput();
    qDebug()<< "抽取后"<< "-----------------------";
    qDebug()<<"模型点数为："<< decimated->GetNumberOfPoints();
    qDebug()<< "模型面数为：" << decimated->GetNumberOfPolys();

    vtkSmartPointer<vtkPolyDataMapper> origMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    origMapper->SetInputData(reader->GetOutput());
    vtkSmartPointer<vtkActor> origActor = vtkSmartPointer<vtkActor>::New();
    origActor->SetMapper(origMapper);

    vtkSmartPointer<vtkPolyDataMapper> deciMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    deciMapper->SetInputData(decimation->GetOutput());
    vtkSmartPointer<vtkActor> deciActor = vtkSmartPointer<vtkActor>::New();
    deciActor->SetMapper(deciMapper);

    double leftViewport[4] = { 0.0, 0.0, 0.5, 1.0 };
    double rightViewport[4] = { 0.5, 0.0, 1.0, 1.0 };

    vtkSmartPointer<vtkRenderer> leftRenderer = vtkSmartPointer<vtkRenderer>::New();
    leftRenderer->SetViewport(leftViewport);
    leftRenderer->AddActor(origActor);
    leftRenderer->SetBackground(1.0, 0, 0);

    vtkSmartPointer<vtkRenderer> rightRenderer = vtkSmartPointer<vtkRenderer>::New();
    rightRenderer->SetViewport(rightViewport);
    rightRenderer->AddActor(deciActor);
    rightRenderer->SetBackground(0, 0, 0);

    leftRenderer->GetActiveCamera()->SetPosition(0, -1, 0);
    leftRenderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
    leftRenderer->GetActiveCamera()->SetViewUp(0, 0, 1);
    leftRenderer->GetActiveCamera()->Azimuth(30);
    leftRenderer->GetActiveCamera()->Elevation(30);
    leftRenderer->ResetCamera();//刷新照相机
    rightRenderer->SetActiveCamera(leftRenderer->GetActiveCamera());//同步显示

    vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(leftRenderer);
    rw->AddRenderer(rightRenderer);
    rw->SetSize(640, 320);
    rw->SetWindowName("PolyData Decimation");

    vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(rw);
    rwi->Start();
}
