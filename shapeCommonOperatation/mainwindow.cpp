#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //triangleSurfaceAndVolume();  //计算表面积和体积
    //distanceOnSurface();   //计算测地距离
    //boundingBox();  //包围盒
    //normalVector();  //加入法向量后图形会更加平滑
    curvatures();  //显示曲率
}

MainWindow::~MainWindow()
{

}

void MainWindow::triangleSurfaceAndVolume()
{
    vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New(); //vtkPolyData类型数据
    cubeSource->SetXLength(10);
    cubeSource->SetYLength(10);
    cubeSource->SetZLength(10);
    cubeSource->Update();

    //其他网格类型转换成三角网格类型
    vtkSmartPointer<vtkTriangleFilter> triFilter = vtkSmartPointer<vtkTriangleFilter>::New();
    triFilter->SetInputData(cubeSource->GetOutput());
    triFilter->Update();
    //计算三角网格的基本属性 面积。体积等
    vtkSmartPointer<vtkMassProperties> massProp = vtkSmartPointer<vtkMassProperties>::New();
    massProp->SetInputData(triFilter->GetOutput());
    double Volume = massProp->GetVolume();  //获得体积
    double SurfaceArea = massProp->GetSurfaceArea();  //获得表面积
    double maxArea = massProp->GetMaxCellArea();  //最大三角形网格面积
    double minArea = massProp->GetMinCellArea();  //最小三角形网格面积
    qDebug()<<"the Volume   : " << Volume;
    qDebug()<<"the SurfaceArea   : " << SurfaceArea;
    qDebug()<<"the MaxAreaofCell   : " << maxArea;
    qDebug()<<"the MinAreaofCell   : " << minArea;

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(triFilter->GetOutput());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0, 0, 1);
    actor->GetProperty()->SetEdgeColor(1, 0, 0);
    actor->GetProperty()->SetEdgeVisibility(1);

    vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
    render->AddActor(actor);
    render->SetBackground(0, 0, 0);

    vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(render);
    rw->SetSize(480, 420);
    rw->SetWindowName("Calculating Area and Volume of Triangle grid");

    vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(rw);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    rwi->SetInteractorStyle(style);  //控制相机对物体旋转、放大、缩小等操作

    rwi->Initialize();
    rwi->Start();
}

void MainWindow::distanceOnSurface()
{
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetCenter(0.0, 0.0, 0.0);
    sphereSource->SetRadius(5.0);
    sphereSource->Update();

    //计算测地距离
    vtkSmartPointer<vtkDijkstraGraphGeodesicPath> dijstra = vtkSmartPointer<vtkDijkstraGraphGeodesicPath>::New();
    dijstra->SetInputData(sphereSource->GetOutput());
    dijstra->SetStartVertex(0);  //开始点的索引号
    dijstra->SetEndVertex(10);  //结束点的索引号
    dijstra->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(sphereSource->GetOutput());
    vtkSmartPointer<vtkPolyDataMapper> pathMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    pathMapper->SetInputData(dijstra->GetOutput());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    vtkSmartPointer<vtkActor> pathActor = vtkSmartPointer<vtkActor>::New();
    pathActor->SetMapper(pathMapper);
    pathActor->GetProperty()->SetColor(1, 0, 0);
    pathActor->GetProperty()->SetLineWidth(5);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->AddActor(pathActor);
    renderer->SetBackground(0, 0, 0);

    vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(renderer);
    rw->SetSize(640, 480);
    rw->SetWindowName("Calculating Geodesic Path");

    vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(rw);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    rwi->SetInteractorStyle(style);  //控制相机对物体旋转、放大、缩小等操作

    rwi->Initialize();
    rwi->Start();
}

void MainWindow::boundingBox()
{
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetCenter(0.0, 0.0, 0.0);
    sphereSource->SetRadius(5.0);
    sphereSource->Update();

    vtkSmartPointer<vtkPolyData> sphere = sphereSource->GetOutput();

    //生成包围盒
    vtkSmartPointer<vtkOutlineFilter> outline = vtkSmartPointer<vtkOutlineFilter>::New();
    outline->SetInputData(sphere);
    outline->Update();   //算法执行完毕，必须更新！！！

    vtkSmartPointer<vtkPolyDataMapper> mapper =vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(sphere);
    vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    outlineMapper->SetInputData(outline->GetOutput());

    vtkSmartPointer<vtkActor> actor =vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);  //一个actor一个模型，一个mapper
    vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
    outlineActor->SetMapper(outlineMapper);
    outlineActor->GetProperty()->SetColor(0, 1, 0);
    outlineActor->GetProperty()->SetLineWidth(3);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->AddActor(outlineActor);
    renderer->SetBackground(0, 0, 0);

    vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(renderer);
    rw->SetSize(640, 480);;
    rw->SetWindowName("PolyData Bounding Box");
    rw->Render();

    vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(rw);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    rwi->SetInteractorStyle(style);  //控制相机对物体旋转、放大、缩小等操作

    rwi->Initialize();
    rwi->Start();
}

void MainWindow::normalVector()
{
    //读取文件
    vtkSmartPointer<vtkPolyDataReader> plyReader = vtkSmartPointer<vtkPolyDataReader>::New();
    plyReader->SetFileName("/home/silence/Project/VTK-Qt/shapeCommonOperatation/fran_cut.vtk");
    plyReader->Update();

    //法向量
    vtkSmartPointer<vtkPolyDataNormals> normFilter = vtkSmartPointer<vtkPolyDataNormals>::New();
    normFilter->SetInputData(plyReader->GetOutput());
    normFilter->SetComputePointNormals(1);  //开启点法向量计算
    normFilter->SetComputeCellNormals(0);   //关闭单元法向量计算
    normFilter->SetConsistency(1);   //自动调整模型的单元法向量。
    normFilter->SetAutoOrientNormals(1);  //开启或关闭合适法向的自动检测,自动调整法线方向
    normFilter->SetSplitting(0);  //打开或关闭锐利边缘的拆分。如果检测到存在锐利边缘，则会将其分裂，则模型数据可能会变化
    normFilter->Update();

    //vtkMaskPoints采集数据，仅保留点数据和属性
    vtkSmartPointer<vtkMaskPoints> mask = vtkSmartPointer<vtkMaskPoints>::New();
    mask->SetInputData(normFilter->GetOutput());
    mask->SetMaximumNumberOfPoints(300); //为了减少计算量，随机采样300个点
    mask->RandomModeOn();  //随机采样
    mask->Update();

    //将圆柱体附加到圆锥体上形成箭头，就是箭头根部是圆柱体，前部是圆锥
    vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
    arrow->Update(); //一定要更新 否则数据没有添加进来，程序会报错

    //符号化技术，用图形将法向量图形化展示，所用的图形是上面生成的箭头
    vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
    glyph->SetInputData(mask->GetOutput());  //设置模型
    glyph->SetSourceData(arrow->GetOutput());  //设置图形化显示的图形源数据，用箭头代替
    glyph->SetVectorModeToUseNormal();//设置向量显示模式和法向量一致，指定要使用法向量数据来控制Glyph图形方向
    glyph->SetScaleFactor(0.01); //设置伸缩比例，控制Glyph图形的大小
    glyph->Update();

    //mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(plyReader->GetOutput());
    //法向量mapper
    vtkSmartPointer<vtkPolyDataMapper> normMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    normMapper->SetInputData(normFilter->GetOutput());
    //法向量可视化mapper
    vtkSmartPointer<vtkPolyDataMapper> glyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    glyphMapper->SetInputData(glyph->GetOutput());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkActor> normActor = vtkSmartPointer<vtkActor>::New();
    normActor->SetMapper(normMapper);

    vtkSmartPointer<vtkActor> glyphActor = vtkSmartPointer<vtkActor>::New();
    glyphActor->SetMapper(glyphMapper);
    glyphActor->GetProperty()->SetColor(1, 0, 0);

    double origView[4] = { 0, 0, 0.33, 1 };
    double normView[4] = { 0.33, 0, 0.66, 1 };
    double glyphView[4] = { 0.66, 0, 1, 1 };

    //render
    vtkSmartPointer<vtkRenderer> origRender = vtkSmartPointer<vtkRenderer>::New();
    origRender->SetViewport(origView);
    origRender->AddActor(actor);
    origRender->SetBackground(1, 0, 0);

    vtkSmartPointer<vtkRenderer> normRender = vtkSmartPointer<vtkRenderer>::New();
    normRender->SetViewport(normView);
    normRender->AddActor(normActor);
    normRender->SetBackground(0, 1, 0);

    vtkSmartPointer<vtkRenderer> glyphRender = vtkSmartPointer<vtkRenderer>::New();
    glyphRender->SetViewport(glyphView);
    glyphRender->AddActor(glyphActor);
    glyphRender->AddActor(normActor);
    glyphRender->SetBackground(0, 0, 1);

    //render window
    vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(origRender);
    rw->AddRenderer(normRender);
    rw->AddRenderer(glyphRender);
    rw->SetWindowName("Calculating Point Norm & Cell Norm");
    rw->SetSize(960, 320);
    rw->Render();

    //render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(rw);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    rwi->SetInteractorStyle(style);

    rwi->Initialize();
    rwi->Start();

//关于类vtkGlyph3D
//详细描述:为每个输入点拷贝相应方向和伸缩比例的轮廓几何体
//vtkGlyph3d是一个过滤器,当拷贝几何体到每个输入点时,它起到滤波作用.
//glyph从过滤输入源中以多边形数据形式定义,glyph根据输入的矢量和法向量来确定方向,根据伸缩数据和矢量大小来确定伸缩比例.
//当glyph较多时,可能通过对象源与其相应的定义信息来创建glyph表.
//glyph表可以通过伸缩值或矢量大小来索引相应的gpyph对象.
//要使用vtkGlyph3D对象,我们首先需要提供一个输入集和一个对象源来定义ghyph.
//然后决定是否对ghyph进行伸缩,以及怎样对其进行伸缩,接下来决定是否对glyph设置方向,以及如何根据矢量及法向量来设置它,最终决定我们是用glyph表还是仅仅是单一的ghyph.
//如果使用了glyph表,我们还需要考虑相应的索引值.
//vtkGlyph3D 实际上是一种符号化的算法工具，可以使用一个源（如球体）为输入数据集的每一个点生成一个符号，并且可以设置符号的方向以及缩放比例，简单点说就是对于你想关注的数据点添加符号标注，符号的样式由自己指定。
    //比如你有一个曲面数据，希望将曲面数据的每个点都用锥体标注出来并且锥体的方向表示该点的法向量方向，这个时候就可以使用vtkGlyph3D。
}

void MainWindow::curvatures()
{
    vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/shapeCommonOperatation/fran_cut.vtk");
    reader->Update();

    //计算曲率
    vtkSmartPointer<vtkCurvatures> curvaturesFilter = vtkSmartPointer<vtkCurvatures>::New();
    curvaturesFilter->SetInputConnection(reader->GetOutputPort());
    //curvaturesFilter->SetCurvatureTypeToMinimum(); //最小曲率
    curvaturesFilter->SetCurvatureTypeToMaximum();   //最大曲率
    //curvaturesFilter->SetCurvatureTypeToGaussian();//高斯曲率
    //curvaturesFilter->SetCurvatureTypeToMean();    //平均曲率
    curvaturesFilter->Update();
    //保存属性数据时，四种曲率数据分别对应属性名字为
    //Minimum_Curvature/Maximum_Curvature/Gauss_Curvature/Mean_Curvature,
    //因此可以通过属性名字获取相应的曲率数据。例如要获得高斯曲率数据，可调用：
    //vtkDoubleArray *gauss = static_cast<vtkDoubleArray*>(curvaturesFilter->GetOutput()->GetpointData()->GetArray("Gauss_Curvature"));

    double scalarRange[2];
    curvaturesFilter->GetOutput()->GetScalarRange(scalarRange);  //获得曲率标量数据的最大最小值

    //建立查找表 做颜色映射
    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetHueRange(0.0, 0.6);
    lut->SetAlphaRange(1.0, 1.0);
    lut->SetValueRange(1.0, 1.0);
    lut->SetSaturationRange(1.0, 1.0);
    lut->SetNumberOfTableValues(256);  //表中有256个值
    lut->SetRange(scalarRange);  //设置颜色表最大最小值
    lut->Build();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(curvaturesFilter->GetOutput());
    mapper->SetLookupTable(lut);
    mapper->SetScalarRange(scalarRange);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    //设置标签
    vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
    scalarBar->SetLookupTable(mapper->GetLookupTable());
    scalarBar->SetTitle(curvaturesFilter->GetOutput()->GetPointData()->GetScalars()->GetName());
    scalarBar->SetNumberOfLabels(5); //设置5个标签

    vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
    render->AddActor(actor);
    render->AddActor2D(scalarBar);
    render->SetBackground(0, 0, 0);

    vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(render);
    rw->SetSize(640, 480);
    rw->SetWindowName("Calculating PolyData Curvature");

    vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(rw);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    rwi->SetInteractorStyle(style);

    rwi->Initialize();
    rwi->Start();
}
