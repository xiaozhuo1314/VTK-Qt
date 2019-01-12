#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
//    delaunay2D();
    delaunay2DConstrained();
}

MainWindow::~MainWindow()
{

}

void MainWindow::delaunay2D()
{
    unsigned int gridSize = 10;
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for (unsigned int x = 0; x < gridSize; x++)
    {
        for (unsigned int y = 0; y < gridSize; y++)
        {
            points->InsertNextPoint(x, y, vtkMath::Random(0.0, 3.0)); //插入点
        }
    }

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);

    vtkSmartPointer<vtkDelaunay2D> delaunay = vtkSmartPointer<vtkDelaunay2D>::New(); //三角剖分
    delaunay->SetInputData(polydata);  //polydata是vtkDataObject的字类
    delaunay->Update();

    //图形显示点集合
    vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter =vtkSmartPointer<vtkVertexGlyphFilter>::New();
    glyphFilter->SetInputData(polydata);
    glyphFilter->Update();

    vtkSmartPointer<vtkPolyDataMapper> pointsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    pointsMapper->SetInputData(glyphFilter->GetOutput());

    vtkSmartPointer<vtkActor> pointsActor = vtkSmartPointer<vtkActor>::New();
    pointsActor->SetMapper(pointsMapper);
    pointsActor->GetProperty()->SetPointSize(3);
    pointsActor->GetProperty()->SetColor(1, 0, 0);

    //显示三角剖分
    vtkSmartPointer<vtkPolyDataMapper> triangulatedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    triangulatedMapper->SetInputData(delaunay->GetOutput());
    vtkSmartPointer<vtkActor> triangulatedActor = vtkSmartPointer<vtkActor>::New();
    triangulatedActor->SetMapper(triangulatedMapper);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(pointsActor);
    renderer->AddActor(triangulatedActor);
    renderer->SetBackground(0, 0, 0);

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    renderWindow->SetSize(640, 640);
    renderWindow->Render();
    renderWindow->SetWindowName("PolyData Delaunay2D");

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindowInteractor->Start();
}

void MainWindow::delaunay2DConstrained()  //代边界约束
{
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    unsigned int gridSize = 10;
    for (unsigned int x = 0; x < gridSize; x++)
    {
        for (unsigned int y = 0; y < gridSize; y++)
        {
            points->InsertNextPoint(x, y, vtkMath::Random(0.0, 3.0));
        }
    }

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);

    //多边形
    vtkSmartPointer<vtkPolygon> poly = vtkSmartPointer<vtkPolygon>::New();
//    poly->GetPointIds()->InsertNextId(32);
//    poly->GetPointIds()->InsertNextId(42);
//    poly->GetPointIds()->InsertNextId(43);
//    poly->GetPointIds()->InsertNextId(44);
//    poly->GetPointIds()->InsertNextId(45);
//    poly->GetPointIds()->InsertNextId(35);
//    poly->GetPointIds()->InsertNextId(25);
//    poly->GetPointIds()->InsertNextId(24);
//    poly->GetPointIds()->InsertNextId(23);
//    poly->GetPointIds()->InsertNextId(22);
    //反方向,就成空洞了
    poly->GetPointIds()->InsertNextId(22);
    poly->GetPointIds()->InsertNextId(23);
    poly->GetPointIds()->InsertNextId(24);
    poly->GetPointIds()->InsertNextId(25);
    poly->GetPointIds()->InsertNextId(35);
    poly->GetPointIds()->InsertNextId(45);
    poly->GetPointIds()->InsertNextId(44);
    poly->GetPointIds()->InsertNextId(43);
    poly->GetPointIds()->InsertNextId(42);
    poly->GetPointIds()->InsertNextId(32);

    vtkSmartPointer<vtkCellArray> cell = vtkSmartPointer<vtkCellArray>::New();
    cell->InsertNextCell(poly); //设计拓扑结构
    //边界约束
    vtkSmartPointer<vtkPolyData> boundary = vtkSmartPointer<vtkPolyData>::New();
    boundary->SetPoints(points);
    boundary->SetPolys(cell); //只显示具有拓扑结构部分

    vtkSmartPointer<vtkDelaunay2D> delaunay = vtkSmartPointer<vtkDelaunay2D>::New();
    delaunay->SetInputData(polydata);
    delaunay->SetSourceData(boundary); //设置剖分约束源
    delaunay->Update();

    vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    glyphFilter->SetInputData(polydata);
    glyphFilter->Update();

    vtkSmartPointer<vtkPolyDataMapper> pointsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    pointsMapper->SetInputData(glyphFilter->GetOutput());
    vtkSmartPointer<vtkActor> pointsActor = vtkSmartPointer<vtkActor>::New();
    pointsActor->SetMapper(pointsMapper);
    pointsActor->GetProperty()->SetPointSize(8);
    pointsActor->GetProperty()->SetColor(1, 0, 0);

    vtkSmartPointer<vtkPolyDataMapper> triangulatedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    triangulatedMapper->SetInputData(delaunay->GetOutput());
    vtkSmartPointer<vtkActor> triangulatedActor = vtkSmartPointer<vtkActor>::New();
    triangulatedActor->SetMapper(triangulatedMapper);

    //一个视图，需要一个render即可
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(pointsActor);
    renderer->AddActor(triangulatedActor);
    renderer->SetBackground(0, 0, 0);

    vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(renderer);
    rw->SetSize(640, 480);
    rw->SetWindowName("PolyData By ConstrainedDelaunay2D");

    vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(rw);
    rwi->Start();
}
