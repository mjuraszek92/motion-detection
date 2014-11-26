//Copyright (c) 2014, ZPI_PT_10_45
//All rights reserved.

//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//1. Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//3. All advertising materials mentioning features or use of this software
//   must display the following acknowledgement:
//   This product includes software developed by the ZPI_PT_10_45.
//4. Neither the name of the ZPI_PT_10_45 nor the
//   names of its contributors may be used to endorse or promote products
//   derived from this software without specific prior written permission.

//THIS SOFTWARE IS PROVIDED BY ZPI_PT_10_45 ''AS IS'' AND ANY
//EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL ZPI_PT_10_45 BE LIABLE FOR ANY
//DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


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
#include <opencv2\opencv.hpp>
#include <vector>
#include <deque>
#include <map>
#include <string>
#define NOMINMAX
#include <Windows.h>
#include <fstream>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstringt.h>
#include "ggthread.h"
#include "ffthread.h"

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

    int watkiz;

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

    bool twatki[16];

    QMessageBox ErrorBox;

    QString fileName;
    QString dir;
    GGthread* gthread;
    GGthread* gthredy[16];

    FFthread* fthread;
    FFthread* fthredy[16];


    QString videoPath;
    QFileInfo video;

    std::string path;
    //cv::VideoCapture capture;

    cv::Mat frame;
    cv::Mat res;
    QTimer* timer;

    //MANUAL

    struct movieFragment {
        int begin;
        int end;
        std::string name;
    };

    std::deque<std::vector<uchar>> allFrames;
    std::deque<int> motion;
    cv::VideoCapture movie;
    int width;
    int height;
    int data_storage;
    int counter;


    std::vector<movieFragment> fragmentList;
    int movies_count;
    int break_flag;
    unsigned int index;
    std::string video_name;
    cv::VideoWriter videocv;
    int frameNo;

    double fps;
    int maxFrame;

    char k;
    int mSec;
    int curFragment;
    int curFrame;
    int begin;
    int end;
    bool stop;
    bool esc;

    double momentFilmu;
    //~MANUAL



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

    void slider();

    void sprawdz();

    void isdone1();
    void isdone2();
    void isdone3();
    void isdone4();



    //void callAuto1();
    //void callAuto2();
    //void callMan();





    void on_actionAutorzy_triggered();

    void on_actionO_programie_triggered();

private:
    Ui::MainWindow *ui;

};



#endif // MAINWINDOW_H
