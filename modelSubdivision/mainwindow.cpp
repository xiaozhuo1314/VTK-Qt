#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    modelSubdivision();
}

MainWindow::~MainWindow()
{

}

void MainWindow::modelSubdivision()
{
    //vtkLinearSubdivisionFilter:该类实现了一种线性细分算法，每次细分将每个三角形面片生成四个新的面片，该算法比较简单，速度快，但是细分后不能产生光滑的模型。
    //vtkLoopSubdivisionFilter:该类实现的是Loop细分算法，每次细分会将一个三角面片生成4个三角面片，具体算法思想可以参考文献[1]；该方法可以生成光滑的连续曲面，应用比较广泛。
    //vtkButterflySubdivisionFilter:该类实现了蝶形细分算法
    vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/modelSubdivision/fran_cut.vtk");
    reader->Update();

    vtkSmartPointer<vtkPolyData> orig = reader->GetOutput();
    qDebug()<< "original" << "-----------------------";
    qDebug()<< "模型点数为： " << orig->GetNumberOfPoints();
    qDebug()<< "模型面数为： " << orig->GetNumberOfPolys();

    //线性网格细分滤波器
    vtkSmartPointer<vtkLinearSubdivisionFilter> linear = vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
    linear->SetInputData(orig);
    linear->SetNumberOfSubdivisions(4);  //细化次数4
    linear->Update();

    vtkSmartPointer<vtkPolyData> linearInfo = linear->GetOutput();
    qDebug()<< "linear" << "-----------------------";
    qDebug()<< "模型点数为： " << linearInfo->GetNumberOfPoints();
    qDebug()<< "模型面数为： " << linearInfo->GetNumberOfPolys();

    //Loop网格细分滤波器
    vtkSmartPointer<vtkLoopSubdivisionFilter> loop = vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
    loop->SetInputData(orig);
    loop->SetNumberOfSubdivisions(4); //细化次数4
    loop->Update();

    vtkSmartPointer<vtkPolyData> loopInfo = loop->GetOutput();
    qDebug()<< "loop" << "-----------------------";
    qDebug()<< "模型点数为： " << loopInfo->GetNumberOfPoints();
    qDebug()<< "模型面数为： " << loopInfo->GetNumberOfPolys();

    //butterfly网格细分滤波器
    vtkSmartPointer<vtkButterflySubdivisionFilter> butterfly = vtkSmartPointer<vtkButterflySubdivisionFilter>::New();
    butterfly->SetInputData(orig);
    butterfly->SetNumberOfSubdivisions(4); //细化次数4
    butterfly->Update();

    vtkSmartPointer<vtkPolyData> butterflyInfo = butterfly->GetOutput();
    qDebug()<< "butterfly" << "-----------------------";
    qDebug()<< "模型点数为： " << butterflyInfo->GetNumberOfPoints();
    qDebug()<< "模型面数为： " << butterflyInfo->GetNumberOfPolys();

    vtkSmartPointer<vtkPolyDataMapper> origMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    origMapper->SetInputData(orig);
    vtkSmartPointer<vtkActor> origActor = vtkSmartPointer<vtkActor>::New();
    origActor->SetMapper(origMapper);

    vtkSmartPointer<vtkPolyDataMapper> linearMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    linearMapper->SetInputData(linear->GetOutput());
    vtkSmartPointer<vtkActor> linearActor = vtkSmartPointer<vtkActor>::New();
    linearActor->SetMapper(linearMapper);

    vtkSmartPointer<vtkPolyDataMapper> loopMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    loopMapper->SetInputData(loop->GetOutput());
    vtkSmartPointer<vtkActor> loopActor = vtkSmartPointer<vtkActor>::New();
    loopActor->SetMapper(loopMapper);

    vtkSmartPointer<vtkPolyDataMapper> butterflyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    butterflyMapper->SetInputData(butterfly->GetOutput());
    vtkSmartPointer<vtkActor> butterflyActor = vtkSmartPointer<vtkActor>::New();
    butterflyActor->SetMapper(butterflyMapper);

    double ltView[4] = { 0, 0, 0.5, 0.5 };  //左上角
    double rtView[4] = { 0.5, 0, 1, 0.5 };
    double lbView[4] = { 0, 0.5, 0.5, 1 };
    double rbView[4] = { 0.5, 0.5, 1, 1 };

    vtkSmartPointer<vtkRenderer> origRender = vtkSmartPointer<vtkRenderer>::New();
    origRender->SetViewport(ltView); //左下角
    origRender->AddActor(origActor);
    origRender->SetBackground(1, 0, 0);

    vtkSmartPointer<vtkRenderer> linearRender = vtkSmartPointer<vtkRenderer>::New();
    linearRender->SetViewport(rtView);
    linearRender->AddActor(linearActor);
    linearRender->SetBackground(0, 1, 0);  //右下角

    vtkSmartPointer<vtkRenderer> loopRender = vtkSmartPointer<vtkRenderer>::New();
    loopRender->SetViewport(lbView);
    loopRender->AddActor(loopActor);
    loopRender->SetBackground(0, 0, 1);  //左上角

    vtkSmartPointer<vtkRenderer> butterflyRender = vtkSmartPointer<vtkRenderer>::New();
    butterflyRender->SetViewport(rbView);  //右上角
    butterflyRender->AddActor(butterflyActor);
    butterflyRender->SetBackground(0, 0, 0);

    vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(origRender);
    rw->AddRenderer(linearRender);
    rw->AddRenderer(loopRender);
    rw->AddRenderer(butterflyRender);
    rw->SetSize(640, 640);
    rw->SetWindowName("PolyData Subdivision");

    origRender->GetActiveCamera()->SetPosition(0, -1, 0);
    origRender->GetActiveCamera()->SetFocalPoint(0, 0, 0);
    origRender->GetActiveCamera()->SetViewUp(0, 0, 1);
    origRender->GetActiveCamera()->Azimuth(30);
    origRender->GetActiveCamera()->Elevation(30);
    origRender->ResetCamera();//刷新照相机
    linearRender->SetActiveCamera(origRender->GetActiveCamera());
    loopRender->SetActiveCamera(origRender->GetActiveCamera());
    butterflyRender->SetActiveCamera(origRender->GetActiveCamera());

    vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(rw);
    rwi->Start();
    rw->Render();
}
