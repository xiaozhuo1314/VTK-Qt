#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    surfaceRestruction();
}

MainWindow::~MainWindow()
{

}

void MainWindow::surfaceRestruction()
{
    vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/surfaceRestruction/fran_cut.vtk");
    reader->Update();

    vtkSmartPointer<vtkPolyData> points = vtkSmartPointer<vtkPolyData>::New();
    points->SetPoints(reader->GetOutput()->GetPoints()); //获得网格模型中的几何数据：点集

    //表面重建
    vtkSmartPointer<vtkSurfaceReconstructionFilter> surf = vtkSmartPointer<vtkSurfaceReconstructionFilter>::New();
    surf->SetInputData(points);
    surf->SetNeighborhoodSize(20);  //设置相邻区域点的个数
    surf->SetSampleSpacing(0.005);  //设置网格间距
    surf->Update();

    //重建后抽取轮廓
    vtkSmartPointer<vtkContourFilter> contour = vtkSmartPointer<vtkContourFilter>::New();
    contour->SetInputConnection(surf->GetOutputPort());
    contour->SetValue(0, 0.0);  //第一个参数是抽取值的索引号，表示第几个抽取值。索引号从0开始计数；第二个参数就是指定的抽取值。
    contour->Update();

    vtkSmartPointer <vtkVertexGlyphFilter> vertexGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexGlyphFilter->AddInputData(points);
    vertexGlyphFilter->Update();
    vtkSmartPointer<vtkPolyDataMapper> pointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    pointMapper->SetInputData(vertexGlyphFilter->GetOutput());
    pointMapper->ScalarVisibilityOff();

    vtkSmartPointer<vtkActor> pointActor = vtkSmartPointer<vtkActor>::New();
    pointActor->SetMapper(pointMapper);
    pointActor->GetProperty()->SetColor(1, 0, 0);
    pointActor->GetProperty()->SetPointSize(4);

    vtkSmartPointer<vtkPolyDataMapper> contourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    contourMapper->SetInputData(contour->GetOutput());
    vtkSmartPointer<vtkActor> contourActor = vtkSmartPointer<vtkActor>::New();
    contourActor->SetMapper(contourMapper);

    double pointView[4] = { 0, 0, 0.5, 1 };
    double contourView[4] = { 0.5, 0, 1, 1 };

    vtkSmartPointer<vtkRenderer> pointRender = vtkSmartPointer<vtkRenderer>::New();
    pointRender->AddActor(pointActor);
    pointRender->SetViewport(pointView);
    pointRender->SetBackground(1, 1, 1);

    vtkSmartPointer<vtkRenderer> contourRender = vtkSmartPointer<vtkRenderer>::New();
    contourRender->AddActor(contourActor);
    contourRender->SetViewport(contourView);
    contourRender->SetBackground(0, 1, 0);

    pointRender->GetActiveCamera()->SetPosition(0, -1, 0);
    pointRender->GetActiveCamera()->SetFocalPoint(0, 0, 0);
    pointRender->GetActiveCamera()->SetViewUp(0,0,1);
    pointRender->GetActiveCamera()->Azimuth(30);
    pointRender->GetActiveCamera()->Elevation(30);
    pointRender->ResetCamera();
    contourRender->SetActiveCamera(pointRender->GetActiveCamera());

    vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(pointRender);
    rw->AddRenderer(contourRender);
    rw->SetSize(640, 320);
    rw->SetWindowName("3D Surface Reconstruction ");
    rw->Render();

    vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(rw);
    rwi->Initialize();
    rwi->Start();
}
