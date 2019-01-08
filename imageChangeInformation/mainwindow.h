#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <vtkSmartPointer.h>
#include <vtkBMPReader.h>
#include <vtkImageChangeInformation.h>
#include <vtkImageData.h>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void imageChangeInformation();
};

#endif // MAINWINDOW_H
