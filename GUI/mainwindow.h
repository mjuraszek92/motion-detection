#ifndef MAINWINDOW_H
#define MAINWINDOW_H




#include <QMainWindow>
#include <QtGui>
#include <QTextCodec>
#include <QMessageBox>
#include <QProgressBar>
#include <QString>
#include <QFileDialog>
#include <QThread>
#include <iostream>
#include <windows.h>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <cstringt.h>
#include "ggthread.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QAction *quitAction;
    boolean runing;
    int tryb;
    int nr;

    double FramesSkip;
    double GapSize;
    double MotionLen;
    double Offset;
    double BeforeMove;
    double AfterMove;
    double Area;
    double History;
    double Mixtures;
    double Threads;
    double Metod;
    double Save;

    bool okFramesSkip;
    bool okGapSize;
    bool okMotionLen;
    bool okOffset;
    bool okBeforeMove;
    bool okAfterMove;
    bool okArea;
    bool okHistory;
    bool okMixtures;
    bool okThreads;

    QMessageBox ErrorBox;

    QString fileName;
    QString dir;
    GGthread* gthread;
    GGthread* gthredy[16];

    //FFthread* fthread;
    //FFthread* fthredy[16];




    void connectActions();
    void setDefaults();
    void setDefPara();







private slots:
    void on_actionExit_triggered();
    void clickTrybAuto1();
    void clickTrybAuto2();
    void clickTrybManu();

    void clickAuto2back();
    void clickAuto2start();
    void clickAuto2sel1();
    void clickAuto2sel2();

    void clickManback();
    void clickMannext();
    void clickMansave();

    void clickAuto1back();
    void clickAuto1add();

    void clickAuto2def();

    void clickManplay();
    void clickMan5plus();
    void clickMan1plus();
    void clickMan5minus();
    void clickMan1minus();

    void clickMetoda1();
    void clickMetoda2();

    void updateImg();

    //void callAuto1();
    //void callAuto2();
    //void callMan();





    void on_actionAutorzy_triggered();

    void on_actionO_programie_triggered();

private:
    Ui::MainWindow *ui;

};



#endif // MAINWINDOW_H
