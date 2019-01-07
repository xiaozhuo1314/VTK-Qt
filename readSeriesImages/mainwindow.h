#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>
#include <QString>
#include <vtkJPEGReader.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageAppend.h>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void readSeriesImages1();
    void readSeriesImages2();
    void readSeriesImages3();
};

#endif // MAINWINDOW_H
