#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //landMark(); //基本点云配准
    landMarkICP();  //ICP算法点云配准
}

MainWindow::~MainWindow()
{

}

void MainWindow::landMark()
{

    //VTKLandmarkTransform类的使用比较简单，只需要设定源标记点和目标标记点。
    //SetModeToRigidBody()函数用于设置其配准变换类型为刚体变换，仅包括简单的平移和旋转（六个自由度）。
    //此外还有一个更为广泛应用的函数——SetModeToSimilarity，设置为相似变换，包括平移、旋转和放缩变换（七个自由度）。以及SetModeToAffine()函数设置仿射变换。默认情况，就采用相似变换进行配准。

    vtkSmartPointer<vtkPoints> sourcePoints = vtkSmartPointer<vtkPoints>::New(); //创建源点
    double sourcePoint1[3] = { 0.5, 0, 0 };
    sourcePoints->InsertNextPoint(sourcePoint1);
    double sourcePoint2[3] = { 0, 0.5, 0 };
    sourcePoints->InsertNextPoint(sourcePoint2);
    double sourcePoint3[3] = { 0, 0, 0.5 };
    sourcePoints->InsertNextPoint(sourcePoint3);

    vtkSmartPointer<vtkPoints> targetPoints = vtkSmartPointer<vtkPoints>::New(); //创建目标点
    double targetPoint1[3] = { 0.0, 0.0, 0.55 };
    targetPoints->InsertNextPoint(targetPoint1);
    double targetPoint2[3] = { 0.0, 0.55, 0.0 };
    targetPoints->InsertNextPoint(targetPoint2);
    double targetPoint3[3] = { -0.55, 0.0, 0.0 };
    targetPoints->InsertNextPoint(targetPoint3);

    //利用Landmark算法求变换矩阵
    //vtkLandmarkTransform输入数据为vtkPoints
    vtkSmartPointer<vtkLandmarkTransform> landmarkTransform = vtkSmartPointer<vtkLandmarkTransform>::New();
    landmarkTransform->SetSourceLandmarks(sourcePoints);
    landmarkTransform->SetTargetLandmarks(targetPoints);
    landmarkTransform->SetModeToRigidBody(); //执行刚体配准
    landmarkTransform->Update();   //秋的变换矩阵

    vtkSmartPointer<vtkPolyData> source = vtkSmartPointer<vtkPolyData>::New();
    source->SetPoints(sourcePoints);
    vtkSmartPointer<vtkPolyData> target = vtkSmartPointer<vtkPolyData>::New();
    target->SetPoints(targetPoints);

    //点集合可视化
    vtkSmartPointer<vtkVertexGlyphFilter> sourceGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    sourceGlyphFilter->SetInputData(source);
    sourceGlyphFilter->Update();

    vtkSmartPointer<vtkVertexGlyphFilter> targetGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    targetGlyphFilter->SetInputData(target);
    targetGlyphFilter->Update();

    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
        vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter->SetInputData(sourceGlyphFilter->GetOutput());
    transformFilter->SetTransform(landmarkTransform);
    transformFilter->Update();  //将源点与变换矩阵计算后显示,用来看看与target的区别

    vtkSmartPointer<vtkPolyDataMapper> sourceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sourceMapper->SetInputConnection(sourceGlyphFilter->GetOutputPort());

    vtkSmartPointer<vtkActor> sourceActor = vtkSmartPointer<vtkActor>::New();
    sourceActor->SetMapper(sourceMapper);
    sourceActor->GetProperty()->SetColor(1, 1, 0);
    sourceActor->GetProperty()->SetPointSize(10);

    vtkSmartPointer<vtkPolyDataMapper> targetMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    targetMapper->SetInputConnection(targetGlyphFilter->GetOutputPort());

    vtkSmartPointer<vtkActor> targetActor = vtkSmartPointer<vtkActor>::New();
    targetActor->SetMapper(targetMapper);
    targetActor->GetProperty()->SetColor(0, 1, 0);
    targetActor->GetProperty()->SetPointSize(10);

    //变换后的点
    vtkSmartPointer<vtkPolyDataMapper> solutionMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    solutionMapper->SetInputConnection(transformFilter->GetOutputPort());

    vtkSmartPointer<vtkActor> solutionActor = vtkSmartPointer<vtkActor>::New();
    solutionActor->SetMapper(solutionMapper);
    solutionActor->GetProperty()->SetColor(1, 0, 0);
    solutionActor->GetProperty()->SetPointSize(10);

    vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
    render->AddActor(sourceActor);
    render->AddActor(targetActor);
    render->AddActor(solutionActor);
    render->SetBackground(0, 0, 0);

    vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(render);
    rw->SetSize(480, 480);
    rw->SetWindowName("Regisration by Landmark");
    //设置坐标系显示功能
    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
    axes->SetScale(10);
    render->AddActor(axes);

    vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(rw);
    /************************************************************/
    vtkSmartPointer<vtkOrientationMarkerWidget> widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    widget->SetOutlineColor(0.9300, 0.5700, 0.1300);
    widget->SetOrientationMarker(axes);
    widget->SetInteractor(rwi); //加入鼠标交互
    widget->SetViewport(0.0, 0.0, 0.3, 0.3);  //设置显示位置
    widget->SetEnabled(1);
    widget->InteractiveOn();//开启鼠标交互
    /************************************************************/
    render->ResetCamera();
    rw->Render();
    rwi->Initialize();
    rwi->Start();
}

void MainWindow::landMarkICP()
{
    vtkSmartPointer <vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/landMark/fran_cut.vtk");
    reader->Update();

    //构造浮动数据点集,生成target点集合
    vtkSmartPointer<vtkPolyData> orig = reader->GetOutput();
    vtkSmartPointer<vtkTransform> trans = vtkSmartPointer<vtkTransform>::New();
    trans->Translate(0.2, 0.1, 0.1);
    trans->RotateX(10);

    //生成target
    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter1 = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter1->SetInputData(reader->GetOutput());
    transformFilter1->SetTransform(trans);
    transformFilter1->Update();
    /*********************************************************/
    //源数据 与 目标数据
    vtkSmartPointer<vtkPolyData> source = vtkSmartPointer<vtkPolyData>::New();
    source->SetPoints(orig->GetPoints());

    vtkSmartPointer<vtkPolyData> target = vtkSmartPointer<vtkPolyData>::New();
    target->SetPoints(transformFilter1->GetOutput()->GetPoints());

    vtkSmartPointer<vtkVertexGlyphFilter>  sourceGlyph = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    sourceGlyph->SetInputData(source);
    sourceGlyph->Update();

    vtkSmartPointer<vtkVertexGlyphFilter>  targetGlyph = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    targetGlyph->SetInputData(target);
    targetGlyph->Update();

    //进行ICP配准求变换矩阵
    //vtkIterativeClosestPointTransform输入点集合为vtkDatatSet
    vtkSmartPointer<vtkIterativeClosestPointTransform> icptrans = vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
    icptrans->SetSource(sourceGlyph->GetOutput());
    icptrans->SetTarget(targetGlyph->GetOutput());
    icptrans->GetLandmarkTransform()->SetModeToRigidBody();  //刚体变换
    icptrans->SetMaximumNumberOfIterations(50); //迭代此书
    icptrans->StartByMatchingCentroidsOn();  //配准之前先计算两个点集合重心，并平移源点集合使得两者重心重合
    icptrans->Modified();
    icptrans->Update();

    //配准矩阵调整源数据
    //进行配准
    vtkSmartPointer<vtkTransformPolyDataFilter> solution = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    solution->SetInputData(sourceGlyph->GetOutput());
    solution->SetTransform(icptrans);
    solution->Update();

    vtkSmartPointer<vtkPolyDataMapper> sourceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sourceMapper->SetInputConnection(sourceGlyph->GetOutputPort());
    vtkSmartPointer<vtkActor> sourceActor = vtkSmartPointer<vtkActor>::New();
    sourceActor->SetMapper(sourceMapper);
    sourceActor->GetProperty()->SetColor(1, 1, 0);
    sourceActor->GetProperty()->SetPointSize(2);

    vtkSmartPointer<vtkPolyDataMapper> targetMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    targetMapper->SetInputConnection(targetGlyph->GetOutputPort());
    vtkSmartPointer<vtkActor> targetActor = vtkSmartPointer<vtkActor>::New();
    targetActor->SetMapper(targetMapper);
    targetActor->GetProperty()->SetColor(0, 1, 0);
    targetActor->GetProperty()->SetPointSize(3);

    vtkSmartPointer<vtkPolyDataMapper> soluMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    soluMapper->SetInputConnection(solution->GetOutputPort());
    vtkSmartPointer<vtkActor> soluActor = vtkSmartPointer<vtkActor>::New();
    soluActor->SetMapper(soluMapper);
    soluActor->GetProperty()->SetColor(1, 0, 0);
    soluActor->GetProperty()->SetPointSize(2);
    //设置坐标系
    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();

    vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
    render->AddActor(sourceActor);
    render->AddActor(targetActor);
    render->AddActor(soluActor);
    render->SetBackground(0, 0, 0);

    vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(render);
    rw->SetSize(480, 320);
    rw->SetWindowName("Regisration by ICP");

    vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(rw);
    /****************************************************************/
    //谨记：顺序不可以颠倒！！！
    vtkSmartPointer<vtkOrientationMarkerWidget> widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    widget->SetOutlineColor(1, 1, 1);
    widget->SetOrientationMarker(axes);
    widget->SetInteractor(rwi); //加入鼠标交互
    widget->SetViewport(0.0, 0.0, 0.3, 0.3);  //设置显示位置
    widget->SetEnabled(1);
    widget->InteractiveOn();//开启鼠标交互
    /****************************************************************/
    render->ResetCamera();
    rw->Render();
    rwi->Initialize();
    rwi->Start();
}
