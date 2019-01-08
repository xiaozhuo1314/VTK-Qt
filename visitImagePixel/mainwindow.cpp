#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    visitImagePixel();
}

MainWindow::~MainWindow()
{

}

void MainWindow::visitImagePixel()  //将左下角弄成黑色
{
    vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/visitImagePixel/lena.bmp");
    reader->Update();

    int dims[3];
    reader->GetOutput()->GetDimensions(dims);
    int nbOfComp;
    nbOfComp = reader->GetOutput()->GetNumberOfScalarComponents();
    for(int k=0; k<dims[2]; k++)
    {
        for(int j=0; j<dims[1]; j++)
        {
            for(int i=0; i<dims[0]; i++)
            {
                if(i<100 && j<100)
                {
                    unsigned char * pixel = (unsigned char *) ( reader->GetOutput()->GetScalarPointer(i, j, k) );
                    *pixel = 0;
                    *(pixel+1) = 0;
                    *(pixel+2) = 0;
                }
            }
        }
    }
    vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
    imageViewer->SetInputConnection(reader->GetOutputPort());
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =  vtkSmartPointer<vtkRenderWindowInteractor>::New();
    imageViewer->SetupInteractor(renderWindowInteractor);
    imageViewer->Render();
    imageViewer->GetRenderer()->ResetCamera();
    imageViewer->Render();

    imageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
    imageViewer->SetSize(640, 480);
    imageViewer->GetRenderWindow()->SetWindowName("VisitImagePixelDirectlyExample");


    renderWindowInteractor->Start();
}
