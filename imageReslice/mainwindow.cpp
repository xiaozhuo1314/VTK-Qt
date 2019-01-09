#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    imageReslice();
}

MainWindow::~MainWindow()
{

}

void MainWindow::imageReslice()
{
    //读取数据
    vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/imageReslice/brain.mhd");
    reader->Update();

    int extent[6]; //维数
    double spacing[3]; //像素间隔
    double origin[3]; //原点坐标
    reader->GetOutput()->GetExtent(extent);
    reader->GetOutput()->GetSpacing(spacing);
    reader->GetOutput()->GetOrigin(origin);

    double center[3]; //变化后中心点
    //计算中心点位置
    center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
    center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
    center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);

    //变换矩阵  前三列为xyz轴的方向矢量，第四列为切面坐标系原点，这里坐标用的是齐次坐标
    static double axialElements[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    vtkSmartPointer<vtkMatrix4x4> resliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();
    resliceAxes->DeepCopy(axialElements);
    //设置当前切片坐标系中心位置坐标
    resliceAxes->SetElement(0, 3, center[0]);  //第一个数字代表行，第2个数字代表列,行和列是从0开始计数
    resliceAxes->SetElement(1, 3, center[1]);
    resliceAxes->SetElement(2, 3, center[2]);

    vtkSmartPointer<vtkImageReslice> reslice = vtkSmartPointer<vtkImageReslice>::New();
    reslice->SetInputConnection(reader->GetOutputPort());
    reslice->SetOutputDimensionality(2);   //二维图像
    reslice->SetResliceAxes(resliceAxes);   //添加变换矩阵
    reslice->SetInterpolationModeToLinear();  //线性插值

    vtkSmartPointer<vtkLookupTable> colorTable = vtkSmartPointer<vtkLookupTable>::New();   //颜色查找表
    colorTable->SetRange(0, 1000);
    colorTable->SetValueRange(0.0, 1.0);
    colorTable->SetSaturationRange(0.0, 0.0);
    colorTable->SetRampToLinear();
    colorTable->Build();

    vtkSmartPointer<vtkImageMapToColors> colorMap = vtkSmartPointer<vtkImageMapToColors>::New();  //图像彩色映射
    colorMap->SetLookupTable(colorTable);
    colorMap->SetInputConnection(reslice->GetOutputPort());
    //colorMap->Update();

    vtkSmartPointer<vtkImageActor> imgActor = vtkSmartPointer<vtkImageActor>::New();
    imgActor->GetMapper()->SetInputConnection(colorMap->GetOutputPort());

    //render
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(imgActor);
    renderer->SetBackground(0.4, 0.5, 0.6);

    //render window
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(640, 480);
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("Extract3Dslice");

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> imagestyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
    renderWindowInteractor->SetInteractorStyle(imagestyle);
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindowInteractor->Initialize();

    vtkSmartPointer<vtkImageInteractionCallback> callback = vtkSmartPointer<vtkImageInteractionCallback>::New();
    callback->SetImageReslice(reslice);
    callback->SetInteractor(renderWindowInteractor);
    imagestyle->AddObserver(vtkCommand::MouseMoveEvent, callback);
    imagestyle->AddObserver(vtkCommand::LeftButtonPressEvent, callback);
    imagestyle->AddObserver(vtkCommand::LeftButtonReleaseEvent, callback);

    renderWindowInteractor->Start();
}
