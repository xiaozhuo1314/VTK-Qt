#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //本例是标量数据
    //在进行颜色渲染时，使用的是点的颜色，而不是单元的颜色，且根据点的索引号依次设置的颜色！在单元内部为设置的单元颜色
    polyDataColor();
}

MainWindow::~MainWindow()
{

}

void MainWindow::polyDataColor()
{
    //几何数据 点集
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0.0, 0.0, 0.0);
    points->InsertNextPoint(1.0, 0.0, 0.0);
    points->InsertNextPoint(1.0, 1.0, 0.0);
    points->InsertNextPoint(0.0, 1.0, 0.0);
    points->InsertNextPoint(2.0, 0.0, 0.0);

    //拓扑数据
    //四边形
    vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
    polygon->GetPointIds()->SetNumberOfIds(4);
    polygon->GetPointIds()->SetId(0, 0);   //多边形看成一条折线段，第一个参数是线段的端点号，第二个参数是点的索引号
    polygon->GetPointIds()->SetId(1, 1);
    polygon->GetPointIds()->SetId(2, 2);
    polygon->GetPointIds()->SetId(3, 3);
    //三角形
    vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
    triangle->GetPointIds()->SetId(0, 1);
    triangle->GetPointIds()->SetId(1, 2);
    triangle->GetPointIds()->SetId(2, 4);

    //拓扑结构集合
    vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();
    cellArray->InsertNextCell(polygon);
    cellArray->InsertNextCell(triangle);

    //几何数据和拓扑数据合成
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);
    polyData->SetPolys(cellArray);

    //设置属性
    unsigned char red[3] = {255, 0, 0};
    unsigned char green[3] = {0, 255, 0};
    unsigned char blue[3] = {0, 0, 255};
    vtkSmartPointer<vtkUnsignedCharArray> ptColor = vtkSmartPointer<vtkUnsignedCharArray>::New();
    ptColor->SetNumberOfComponents(3);
    ptColor->InsertNextTypedTuple(red);   //有几个点就要设置几个,所以要设置5个,按点的索引号依次设置颜色
    ptColor->InsertNextTypedTuple(green);
    ptColor->InsertNextTypedTuple(blue);
    ptColor->InsertNextTypedTuple(red);
    ptColor->InsertNextTypedTuple(blue);
    polyData->GetPointData()->SetScalars(ptColor);
    vtkSmartPointer<vtkUnsignedCharArray> cellColor = vtkSmartPointer<vtkUnsignedCharArray>::New();
    cellColor->SetNumberOfComponents(3); //rgb
    cellColor->InsertNextTypedTuple(red);
    cellColor->InsertNextTypedTuple(green);
    polyData->GetCellData()->SetScalars(cellColor);

    //mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polyData);
    mapper->Update();

    //actor
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    //render
    vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
    render->AddActor(actor);
    render->SetBackground(0.0, 0.0, 0.0);

    //render window
    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(render);
    renWin->Render();
    renWin->SetSize(640, 480);
    renWin->SetWindowName("haha");

    //render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renWin);
    interactor->Initialize();
    interactor->Start();
}
