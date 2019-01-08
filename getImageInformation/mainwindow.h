#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkBMPReader.h>
#include <vtkImageData.h>
#include <QDebug>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void getImageInformation();
};

#endif // MAINWINDOW_H
