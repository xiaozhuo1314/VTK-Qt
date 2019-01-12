#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    marchingCubes();
}

MainWindow::~MainWindow()
{

}

void MainWindow::marchingCubes()  //等值面提取
{
    //vtkImageMarchingCubes:主要处理三维图像数据
    //vtkMarchingCubes:主要针对规则体数据生成等值面
    //vtkMarchingSquares:则是针对二维规则网格数据生成等值线
    //vtkMarchingContourFilter:可以接受任何类型的数据，其内部根据数据不同生成不同的算法对象实现等值面/线的提取，具有较高的效率
    //vtkContourFilter:则是一个更加通用的等值面提取类，其可以接受任意的数据类型生成等值线或等值面。
    //vtkDiscreteMarchingCubes继承自vtkMarchingCubes，主要针对Label图像，比如利用图像分割算法对医学图像进行分割后得到含有不同Label值得数据，每个Label对应一个组织，吐过想要得到其中一个或者几个组织的洛括模型，则可以考虑使用该类。
    vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/marchingCubes/HeadMRVolume.mhd");
    reader->Update();

    vtkSmartPointer<vtkMarchingCubes> surface = vtkSmartPointer<vtkMarchingCubes>::New();
    surface->SetInputData(reader->GetOutput());
    surface->ComputeNormalsOn();  //法向量开启，有助于渲染结果
    surface->SetValue(0, 100);  //第0个等值面  值为：200,这里的0只是为了多个等值面的时候便于区分而设置的索引号，后面的200，指的是等值面的方程参数，也即是F(x,y,z)=200
    surface->Update();

    vtkSmartPointer<vtkPolyDataMapper> surfMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    surfMapper->SetInputConnection(surface->GetOutputPort());
    vtkSmartPointer<vtkActor> surfActor = vtkSmartPointer<vtkActor>::New();
    surfActor->SetMapper(surfMapper);
    surfActor->GetProperty()->SetColor(1, 0, 0);

    vtkSmartPointer<vtkRenderer> surfRender = vtkSmartPointer<vtkRenderer>::New();
    surfRender->AddActor(surfActor);
    surfRender->SetBackground(0, 0, 0);

    vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(surfRender);
    rw->SetSize(640, 480);
    rw->SetWindowName("PolyData MarchingCubes");
    rw->Render();

    vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    rwi->SetInteractorStyle(style);
    rwi->SetRenderWindow(rw);
    rwi->Initialize();
    rwi->Start();
}
