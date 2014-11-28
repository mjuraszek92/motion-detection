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


#include "mainwindow.h"
#include "ui_mainwindow.h"







extern void motion_processing(std::deque<int> &motion, int offset, int beforeMotion, int pastMotion, int onesSize, int zerosSize);
extern int find_char(std::string path, char c);
extern std::string find_filename(std::string path);

//struct movieFragment {
//    int begin;
//    int end;
//    std::string name;
//};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    setDefaults();
    setDefPara();
    connectActions();
}






MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);

}









void MainWindow::connectActions()
{
     //Menu główne
     connect(ui->pushButton_3, SIGNAL(clicked()), qApp, SLOT(quit()));
     connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(clickTrybAuto1()));
     connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(clickTrybAuto2()));
     connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(clickTrybManu()));

     //Tryb Automatyczny 2
     connect(ui->auto2_back, SIGNAL(clicked()), this, SLOT(clickAuto2back()));
     connect(ui->auto2_start, SIGNAL(clicked()), this, SLOT(clickAuto2start()));
     connect(ui->auto2_select1, SIGNAL(clicked()), this, SLOT(clickAuto2sel1()));
     connect(ui->auto2_select2, SIGNAL(clicked()), this, SLOT(clickAuto2sel2()));
     connect(ui->auto2_def, SIGNAL(clicked()), this, SLOT(clickAuto2def()));

     //Tryb Manualny
     connect(ui->man_back, SIGNAL(clicked()), this, SLOT(clickManback()));
     connect(ui->man_next, SIGNAL(clicked()), this, SLOT(clickMannext()));
     connect(ui->man_save, SIGNAL(clicked()), this, SLOT(clickMansave()));

     connect(ui->auto1_back, SIGNAL(clicked()), this, SLOT(clickAuto1back()));
     connect(ui->auto1_add, SIGNAL(clicked()), this, SLOT(clickAuto1add()));

     connect(ui->man_5plus, SIGNAL(clicked()), this, SLOT(clickMan5plus()));
     connect(ui->man_1plus, SIGNAL(clicked()), this, SLOT(clickMan1plus()));
     connect(ui->man_5minus, SIGNAL(clicked()), this, SLOT(clickMan5minus()));
     connect(ui->man_1minus, SIGNAL(clicked()), this, SLOT(clickMan1minus()));
     connect(ui->man_play, SIGNAL(clicked()), this, SLOT(clickManplay()));

     connect(ui->auto2_met1, SIGNAL(clicked()), this, SLOT(clickMetoda1()));
     connect(ui->auto2_met2, SIGNAL(clicked()), this, SLOT(clickMetoda2()));

     connect(ui->horizontalSlider, SIGNAL(sliderMoved()), this, SLOT(slider()));
     //connect(ui->horizontalSlider, SIGNAL(sliderPressed()), this, SLOT(clickManplay()));


     connect(ui->go_end, SIGNAL(clicked()), this, SLOT(clickGo_end()));
     connect(ui->go_start, SIGNAL(clicked()), this, SLOT(clickGo_start()));
     connect(ui->set_end, SIGNAL(clicked()), this, SLOT(clickSet_end()));
     connect(ui->set_start, SIGNAL(clicked()), this, SLOT(clickSet_start()));

     timer = new QTimer(this);
     connect(timer,SIGNAL(timeout()),this,SLOT(updateImg()));










}

void MainWindow::setDefaults(){

    runing = false;
    ui->setupUi(this);
    ui->mainToolBar->hide();
    ui->Auto2->setAutoFillBackground(true);
    ui->Auto2->hide();
    ui->Auto1->setAutoFillBackground(true);
    ui->Auto1->hide();
    ui->label_3->hide();
    ui->Threads->hide();
    ui->Man->setAutoFillBackground(true);
    ui->Man->hide();
    nr = 0;
    ui->auto2_sel_text1->setDisabled(true);
    //ui->auto2_sel_text1->setText("F:/film2.avi");
    ui->auto2_sel_text2->setDisabled(true);
    ui->Lp->setDisabled(true);
    ui->Name->setDisabled(true);
    ui->Status->setDisabled(true);
    ui->label_6->setText("Tryb Auto Solo");
    ui->label_10->setText("Tryb Auto Multi");
    ui->auto2_sel_text2->hide();
    ui->auto2_select2->hide();
    ui->label_2->hide();

    QPalette sel = ui->auto2_sel_text1->palette();

    sel.setColor(QPalette::Disabled, QPalette::Text, sel.color(QPalette::Active, QPalette::Text));
    sel.setColor(QPalette::Disabled, QPalette::Base, sel.color(QPalette::Active, QPalette::Base));
    ui->auto2_sel_text1->setPalette(sel);
    ui->auto2_sel_text2->setPalette(sel);
    ui->Lp->setPalette(sel);
    ui->Name->setPalette(sel);
    ui->Status->setPalette(sel);

    ErrorBox.setFixedSize(500,200);

    ui->Lp->setText("\n\n\n");
    ui->Name->setText("\n\n\n");
    ui->Status->setText("\n\n\n");

    ui->zapis->hide();






}

void MainWindow::setDefPara(){

    FramesSkip = 1;
    GapSize = 90;
    MotionLen = 60;
    BeforeMove = 30;
    AfterMove = 30;
    Area = 0.0005;
    History = 200;
    Mixtures = 3;
    Threads = 1;
    Metod = 1;
    Save = 0;

    watkiz = 0;
    res.create(240*3/2,320*3/2,CV_8UC1);

    ui->FrameSkip->setText(QString().number(FramesSkip));
    ui->GapSize->setText(QString().number(GapSize));
    ui->MotionLen->setText(QString().number(MotionLen));
    ui->BeforeMove->setText(QString().number(BeforeMove));
    ui->AfterMove->setText(QString().number(AfterMove));
    ui->Area->setText(QString().number(Area));
    ui->History->setText(QString().number(History));
    ui->Mixtures->setText(QString().number(Mixtures));
    ui->Threads->setText(QString().number(Threads));

    if (!ui->zapis1->isChecked()){
        ui->zapis1->toggle();
    }
    if (!ui->auto2_met2->isChecked()){
        ui->auto2_met2->toggle();
        clickMetoda2();
    }


        for(int i = 0;i<17;i++){
            isDone[i]=false;
            isOn[i]=false;
        }




}




// Funkcje do manuala







void MainWindow::clickTrybAuto1()
{
    tryb = 1;
    ui->Auto1->show();
    ui->label_7->hide();
}

void MainWindow::clickTrybAuto2()
{
    tryb = 2;
    ui->label_6->setText("Tryb Auto Solo");
    ui->Auto2->show();
    ui->label_3->show();
    ui->label_7->show();
    ui->Threads->show();
}


void MainWindow::clickTrybManu()
{
    ui->label_6->setText("Tryb Manualny");
    ui->Auto2->show();
    ui->zapis->show();
    ui->label_7->hide();
    tryb = 0;
}

//Tryb Automatyczny 2
void MainWindow::clickAuto2back()
{

    if (tryb==1){

         ui->Auto2->hide();
         ui->Auto1->show();

    } else{

        ui->Auto2->hide();
        ui->label_3->hide();
        ui->Threads->hide();
        setDefPara();
    }

    ui->zapis->hide();

}

void MainWindow::clickAuto2start()
{

    FramesSkip = QString(ui->FrameSkip->text()).toDouble(&okFramesSkip);
    GapSize = QString(ui->GapSize->text()).toDouble(&okGapSize);
    MotionLen = QString(ui->MotionLen->text()).toDouble(&okMotionLen);
    BeforeMove = QString(ui->BeforeMove->text()).toDouble(&okBeforeMove);
    AfterMove = QString(ui->AfterMove->text()).toDouble(&okAfterMove);
    Area = QString(ui->Area->text()).toDouble(&okArea);
    History = QString(ui->History->text()).toDouble(&okHistory);
    Mixtures = QString(ui->Mixtures->text()).toDouble(&okMixtures);
    Threads = QString(ui->Threads->text()).toDouble(&okThreads);
    path = ui->auto2_sel_text1->text().toStdString();


    if(ui->auto2_met1->isChecked()){
        Metod = 0;
    } else {
        Metod = 1;
    }




    if (okFramesSkip && FramesSkip>=0 && FramesSkip<=5){
        if(okGapSize && GapSize>=1){
            if(okMotionLen &&  MotionLen>=1){

                    if(okBeforeMove && BeforeMove>=0){
                        if(okAfterMove && AfterMove>=0){
                            if(okArea && Area>=0 && Area<=0.5) {
                                if(okHistory && History>=10 && History<=500){
                                    if(okMixtures && Mixtures>=1 && Mixtures<=10){
                                        if(okThreads && Threads>=1){
                                            //path = fileName.toStdString();

                                            if(path.compare("")!=0){



                                            if(tryb==2){
                                                if (!runing){


                                                    std::map<std::string,double> parametry;
                                                        parametry["frame_skip"] = FramesSkip;
                                                        parametry["zeros_size"] = GapSize;
                                                        parametry["ones_size"] = MotionLen;
                                                        parametry["befo_motion"] = BeforeMove;
                                                        parametry["past_motion"] = AfterMove;
                                                        parametry["area"] = Area;
                                                        parametry["history"] = History;
                                                        parametry["nmixtures"] = Mixtures;
                                                        parametry["method"] = Metod;
                                                        parametry["thread_no"] = 1;
                                                        parametry["threads"] = Threads;
                                                        //std::string path = "F://film2.avi";
                                                        //path = fileName.toStdString();



                                                        videoPath = QString::fromStdString(path);
                                                        video.setFile(videoPath);
                                                        if(video.exists()){
                                                        path = videoPath.toUtf8().constData();
                                                        movie.open(path);


                                                        qDebug("DONE!");

                                                        if(!movie.isOpened()){
                                                             ErrorBox.critical(0, "Błędna ścieżka","Podana ścieżka do pliku jest niepoprawna");
                                                                   qDebug() << "Error, video not loaded";
                                                        }
                                                         else {
                                                            ui->auto2_start->setText("Przerwij");
                                                            runing = true;
                                                            ui->auto2_back->setDisabled(true);


                                                        //thread = new GGthread(path,parametry);
                                                        //thread->start();


                                                        for (int i=0;i<parametry["threads"];i++){
                                                            Sleep(100);
                                                           gthredy[i] = new GGthread(path,parametry);
                                                           connect(gthredy[i], SIGNAL(finished()), this, SLOT(sprawdz()));
                                                           gthredy[i]->start();
                                                            parametry["thread_no"] = parametry["thread_no"] + 1;
                                                        }
                                                }
                                                        } else {
                                                            qDebug() << "Error, video not loaded";
                                                        }






                                                } else{
                                                    ui->auto2_start->setText("Start");
                                                    runing = false;
                                                    ui->auto2_back->setDisabled(false);
                                                    for (int i=0;i<Threads;i++){

                                                       gthredy[i]->stop();
                                                        gthredy[i]->terminate();
                                                       qDebug("kill");
                                                    }

                                                }
                                            } else if(tryb==1){
                                                    ui->Auto2->hide();
                                                    ui->Auto1->show();
                                                    ui->Lp->setText(ui->Lp->toPlainText()+QString().setNum(nr)+"\n");
                                                    ui->Name->setText(ui->Name->toPlainText()+fileName+"\n");
                                                    ui->Status->setText(ui->Status->toPlainText()+"Trwa"+"\n");

                                                    std::map<std::string,double> parametry;
                                                        parametry["frame_skip"] = FramesSkip;
                                                        parametry["zeros_size"] = GapSize;
                                                        parametry["ones_size"] = MotionLen;
                                                        parametry["befo_motion"] = BeforeMove;
                                                        parametry["past_motion"] = AfterMove;
                                                        parametry["area"] = Area;
                                                        parametry["history"] = History;
                                                        parametry["nmixtures"] = Mixtures;
                                                        parametry["method"] = Metod;
                                                        parametry["thread_no"] = 1;
                                                        parametry["threads"] = Threads;
                                                        //std::string path = "F://film2.avi";

                                                        videoPath = QString::fromStdString(path);
                                                        video.setFile(videoPath);
                                                        if(video.exists()){
                                                        path = videoPath.toUtf8().constData();
                                                        movie.open(path);


                                                        qDebug("DONE!");

                                                        if(!movie.isOpened()){
                                                             ErrorBox.critical(0, "Błędna ścieżka","Podana ścieżka do pliku jest niepoprawna");
                                                                   qDebug() << "Error, video not loaded";
                                                        }
                                                         else {

                                                    fthredy[nr-1] = new FFthread(path,parametry);
                                                    fthredy[nr-1]->start();

                                                        twatki[nr-1]=true;
                                                        isOn[nr]=true;


                                                        if(nr==1){
                                                            connect(fthredy[nr-1], SIGNAL(finished()), this, SLOT(isdone1()));
                                                        }
                                                        if(nr==2){
                                                            connect(fthredy[nr-1], SIGNAL(finished()), this, SLOT(isdone2()));
                                                        }
                                                        if(nr==3){
                                                            connect(fthredy[nr-1], SIGNAL(finished()), this, SLOT(isdone3()));
                                                        }
                                                        if(nr==4){
                                                            connect(fthredy[nr-1], SIGNAL(finished()), this, SLOT(isdone4()));
                                                        }
                                                        if(nr==5){
                                                            connect(fthredy[nr-1], SIGNAL(finished()), this, SLOT(isdone1()));
                                                        }
                                                        if(nr==6){
                                                            connect(fthredy[nr-1], SIGNAL(finished()), this, SLOT(isdone2()));
                                                        }
                                                        if(nr==7){
                                                            connect(fthredy[nr-1], SIGNAL(finished()), this, SLOT(isdone3()));
                                                        }
                                                        if(nr==8){
                                                            connect(fthredy[nr-1], SIGNAL(finished()), this, SLOT(isdone4()));
                                                        }

                                                        runing=true;
                                                        //ui->auto1_back->setText("Przerwij");




                                                        }
                                                        } else {
                                                            ErrorBox.critical(0, "Błędna ścieżka","Podana ścieżka do pliku jest niepoprawna");
                                                            qDebug() << "Error, video not loaded";
                                                        }





                                                } else if(tryb==0){
                                                    ui->Auto2->hide();
                                                    ui->Man->show();


                                                    std::map<std::string,double> parametry;
                                                        parametry["frame_skip"] = FramesSkip;
                                                        parametry["zeros_size"] = GapSize;
                                                        parametry["ones_size"] = MotionLen;
                                                        parametry["befo_motion"] = BeforeMove;
                                                        parametry["past_motion"] = AfterMove;
                                                        parametry["area"] = Area;
                                                        parametry["history"] = History;
                                                        parametry["nmixtures"] = Mixtures;
                                                        parametry["method"] = Metod;
                                                        parametry["thread_no"] = 1;
                                                        parametry["threads"] = Threads;
                                                        //path = fileName.toStdString();


                                                    videoPath = QString::fromStdString(path);
                                                    video.setFile(videoPath);
                                                    if(video.exists()){
                                                    path = videoPath.toUtf8().constData();
                                                    //movie.open(path);



                                                      counter = 0;


                                                      mthread = new mmthread(path,parametry, &allFrames, &motion, &movie, &width, &height, &counter);
                                                      connect(mthread, SIGNAL(finished()), this, SLOT(manualCheck()));
                                                      mthread->start();

                                                      ui->horizontalSlider->setDisabled(true);
                                                      ui->go_end->setDisabled(true);
                                                      ui->go_start->setDisabled(true);
                                                      ui->set_end->setDisabled(true);
                                                      ui->set_start->setDisabled(true);
                                                      ui->man_1minus->setDisabled(true);
                                                      ui->man_5minus->setDisabled(true);
                                                      ui->man_1plus->setDisabled(true);
                                                      ui->man_5plus->setDisabled(true);
                                                      ui->man_back->setText("Przetwij");
                                                      ui->man_play->setDisabled(true);
                                                      ui->man_save->setDisabled(true);
                                                      ui->man_next->setDisabled(true);

                                                      runing = true;


                                                    //motion_detection(path,parametry, allFrames, motion, movie, width, height, counter);











                                                    }













                                            }



                                            } else {
                                                ErrorBox.critical(0, "Brak pliku","Wybierz plik");
                                            }

                                        } else {
                                            ErrorBox.critical(0, "Błedny parametr","Błędny parametr Threads");
                                        }
                                    } else {
                                        ErrorBox.critical(0, "Błedny parametr","Błędny parametr Mixtures");
                                    }
                                } else {
                                    ErrorBox.critical(0, "Błedny parametr","Błędny parametr History");
                                }
                            } else {
                                ErrorBox.critical(0, "Błedny parametr","Błędny parametr Area");
                            }
                        } else {
                            ErrorBox.critical(0, "Błedny parametr","Błędny parametr After Move");
                        }
                    } else {
                        ErrorBox.critical(0, "Błedny parametr","Błędny parametr Before Move");
                    }

            } else {
                ErrorBox.critical(0, "Błedny parametr","Błędny parametr Motion Len");
            }
        } else {
            ErrorBox.critical(0, "Błedny parametr","Błędny parametr Gap Size");
        }

    }else {
        ErrorBox.critical(0, "Błedny parametr","Błędny parametr Frame Skip");

    }




}

void MainWindow::clickAuto2sel1()
{
     fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "C:/",
                                                    tr("filmy (*.avi)"));




     //string tmp = fileName.toStdString();

     //auto it = std::find(tmp.begin(), tmp.end(), '/');
     //while (it != tmp.end()) {
     //    auto it2 = tmp.insert(it, '/');

         // skip over the slashes we just inserted
     //    it = std::find(it2+1, tmp.end(), '/');
    // }

    //fileName = QString::fromStdString(tmp);

    ui->auto2_sel_text1->setText(fileName);
}

void MainWindow::clickAuto2sel2()
{
     dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "C:/",
                                                      QFileDialog::DontResolveSymlinks);





    ui->auto2_sel_text2->setText(dir);
}



//Tryb Manualny
void MainWindow::clickManback()
{
    if(runing==false){
        ui->Man->hide();
        ui->zapis->hide();
    } else {
        mthread->stop();
        mthread->terminate();
        runing = false;
        manualCheck();
    }

}

void MainWindow::clickMannext()
{
    ui->Man->hide();
    ui->Auto2->show();



}


void MainWindow::updateImg(){



    if(curFrame>=motion.size()){
        return;
        ui->man_play->setText("Pause");
    }

//   movie.read(frame);

//   if (frame.empty()==true){
//       qDebug("EMPTY");
//       timer->stop();
//       return;
//   }
//    cv::resize(frame,frame,res.size());
//   //qDebug() << frame.rows;


//    cv::cvtColor(frame, frame, CV_BGR2RGB);
   // frame = playVideo(motion,allFrames,stop,esc,curFrame, momentFilmu, k, mSec, data_storage);

    if (data_storage == 1)
    {
        std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
        frame = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
    }
    else
    {
        frame = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
    }

//		cv::imshow("Motion",picToShow);
    cv::resize(frame,frame,res.size());
    curFrame=curFrame + 1;

     ui->horizontalSlider->setValue(((int)(100*curFrame/motion.size())));

    QImage qim((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    qDebug("wat");

   ui->label_22->setPixmap((QPixmap::fromImage(qim)));



}

void MainWindow::clickMansave()
{

    clickManplay();

    if (data_storage == 1)
            chdir("..");

        videocv.open((fragmentList[curFragment].name),CV_FOURCC('M','P','E','G'),fps, cv::Size(width,height),true);
        for (int j=begin; j<=end; j++)
        {
            if (data_storage == 1)
            {
                chdir("images");
                std::string pic_name = std::string("img") + std::to_string((long double)j) + std::string(".jpg");
                frame = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
                chdir("..");
            }
            else
            {
                frame = cv::imdecode(cv::Mat(allFrames[j]),CV_LOAD_IMAGE_COLOR);
            }
            videocv.write(frame);
        }
        videocv.release();

        // Przeskoczenie na następny fragment (jeśli jest) i ustawienie aktualnych informacji dotyczących fragmentu
        if (curFragment + 1 != fragmentList.size())
        {
            curFragment = curFragment + 1;
            begin = fragmentList[curFragment].begin;
            end = fragmentList[curFragment].end;
            curFrame = begin;
        }
        std::cout << fragmentList[curFragment].name << std::endl;
        if (data_storage == 1)
            chdir("images");

        QMessageBox msgBox;
        msgBox.setWindowTitle("Koniec");
        msgBox.setText("Zapisano!");
        msgBox.exec();


}


void MainWindow::clickAuto1back()
{


    runing=false;
    ui->Auto1->hide();
    setDefPara();
    nr = 0;
    timer->stop();

}

void MainWindow::clickAuto1add()
{

    nr = nr + 1;
    QString s;
    s.setNum(nr,10);
    ui->label_6->setText(" Film numer " + s);
    ui->Auto1->hide();
    ui->Auto2->show();



}

void MainWindow::clickManplay(){

    stop = false;


//    videoPath = QString::fromStdString("F://film2.avi");
//    video.setFile(fileName);
//    if(video.exists()){
//        path = videoPath.toUtf8().constData();
//        movie.open(path);


//        if(!movie.isOpened()){
//            qDebug() << "Error, video not loaded";
//        }

//        cv::namedWindow("window",1);
//        while(true)
//        {
//            bool success = movie.read(frame);
//                        if(success == false){
//                            break;
//                        }

//            cv::cvtColor(frame, frame, CV_BGR2RGB);
//            QImage qim((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

//            ui->label_22->setPixmap((QPixmap::fromImage(qim)));
//             cv::waitKey(20);
//        }
//        cv::waitKey(0);
//    }
//    else{
//        qDebug() << "Error, File doesn't exist";
//    }

    if(timer->isActive()==true){
        timer->stop();
        ui->man_play->
                setText("Play");
        stop = true;
    } else{
        timer->start(20);
         ui->man_play->setText("Pause");
    }



}

void MainWindow::clickMan5plus(){

    if (curFrame + 5 < motion.size())
        curFrame = curFrame + 5;
    else
        curFrame = motion.size();

    if(!timer->isActive()){

        if (data_storage == 1)
        {
            std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
            frame = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
        }
        else
        {
            frame = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
        }

        cv::resize(frame,frame,res.size());


         ui->horizontalSlider->setValue(((int)(100*curFrame/motion.size())));

        QImage qim((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        qDebug("wat");

       ui->label_22->setPixmap((QPixmap::fromImage(qim)));
    }

}

void MainWindow::clickMan1plus(){

    if (curFrame + 1 < motion.size())
        curFrame = curFrame + 1;
    else
        curFrame = motion.size();

    if(!timer->isActive()){

        if (data_storage == 1)
        {
            std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
            frame = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
        }
        else
        {
            frame = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
        }

        cv::resize(frame,frame,res.size());


         ui->horizontalSlider->setValue(((int)(100*curFrame/motion.size())));

        QImage qim((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        qDebug("wat");

       ui->label_22->setPixmap((QPixmap::fromImage(qim)));
    }

}

void MainWindow::clickMan5minus(){

    if (curFrame - 5 > 0)
        curFrame = curFrame - 5;
    else
        curFrame = 0;

    if(!timer->isActive()){

        if (data_storage == 1)
        {
            std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
            frame = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
        }
        else
        {
            frame = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
        }

        cv::resize(frame,frame,res.size());


         ui->horizontalSlider->setValue(((int)(100*curFrame/motion.size())));

        QImage qim((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        qDebug("wat");

       ui->label_22->setPixmap((QPixmap::fromImage(qim)));
    }

}

void MainWindow::clickMan1minus(){


        if (curFrame - 1 > 0)
            curFrame = curFrame - 1;
        else
            curFrame = 0;

        if(!timer->isActive()){

            if (data_storage == 1)
            {
                std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
                frame = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
            }
            else
            {
                frame = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
            }

            cv::resize(frame,frame,res.size());


             ui->horizontalSlider->setValue(((int)(100*curFrame/motion.size())));

            QImage qim((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            qDebug("wat");

           ui->label_22->setPixmap((QPixmap::fromImage(qim)));
        }



}

void MainWindow::clickAuto2def(){
    setDefPara();
    ui->Auto2->repaint();

}

void MainWindow::clickMetoda1(){

    ui->History->hide();
    ui->Mixtures->hide();

    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_26->hide();
    ui->label_27->hide();
}

void MainWindow::clickMetoda2(){

    ui->History->show();
    ui->Mixtures->show();

    ui->label_17->show();
    ui->label_18->show();
    ui->label_26->show();
    ui->label_27->show();

}

void MainWindow::slider(){

    clickManplay();
    //curFrame = (int)(momentFilmu*motion.size());
    curFrame = (ui->horizontalSlider->value()/100)*motion.size();
    //ui->man_save->setText(QString::number(ui->horizontalSlider->value(),10));
    clickManplay();



}

void MainWindow::sprawdz(){

    watkiz++;
    if(watkiz==Threads){

        if(runing==true){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Koniec");
        msgBox.setText("Zakończono sukcesem!");
        msgBox.exec();
        ui->auto2_start->setText("Start");
        runing = false;
        ui->auto2_back->setDisabled(false);
        qDebug("DONE!");
        }
        watkiz=0;
    }

}

void MainWindow::clickSet_end(){

    end = curFrame;

}

void MainWindow::clickSet_start(){

    begin = curFrame;

}

void MainWindow::clickGo_end(){

    curFrame = end;
    if (data_storage == 1)
    {
        std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
        frame = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
    }
    else
    {
        frame = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
    }

    if(!timer->isActive()){

        if (data_storage == 1)
        {
            std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
            frame = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
        }
        else
        {
            frame = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
        }

        cv::resize(frame,frame,res.size());


         ui->horizontalSlider->setValue(((int)(100*curFrame/motion.size())));

        QImage qim((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        qDebug("wat");

       ui->label_22->setPixmap((QPixmap::fromImage(qim)));
    }

}

void MainWindow::clickGo_start(){

    curFrame = begin;
    if (data_storage == 1)
    {
        std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
        frame = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
    }
    else
    {
        frame = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
    }

    if(!timer->isActive()){

        if (data_storage == 1)
        {
            std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
            frame = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
        }
        else
        {
            frame = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
        }

        cv::resize(frame,frame,res.size());


         ui->horizontalSlider->setValue(((int)(100*curFrame/motion.size())));

        QImage qim((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        qDebug("wat");

       ui->label_22->setPixmap((QPixmap::fromImage(qim)));
    }

}

void MainWindow::manualCheck(){

//    QMessageBox msgBox;
//    msgBox.setWindowTitle("Koniec");
//    msgBox.setText("Zapisano!");
//    msgBox.exec();

    ui->horizontalSlider->setDisabled(false);
    ui->go_end->setDisabled(false);
    ui->go_start->setDisabled(false);
    ui->set_end->setDisabled(false);
    ui->set_start->setDisabled(false);
    ui->man_1minus->setDisabled(false);
    ui->man_5minus->setDisabled(false);
    ui->man_1plus->setDisabled(false);
    ui->man_5plus->setDisabled(false);
    ui->man_back->setText("Wstecz");
    ui->man_play->setDisabled(false);
    ui->man_save->setDisabled(false);
    ui->man_next->setDisabled(false);


    if(runing==true){
        movies_count = 0;
        break_flag = 0;
        index = 0;

    qDebug("1");
    while( true )
        {
        qDebug("2");
            movieFragment movieData;
            ++movies_count;
            while( motion[index] == 0 )
            {
                qDebug("3");
                ++index;
                if( index == motion.size() )
                {
                    break_flag = 1;
                    break;
                }
            }
            if( break_flag )
            {
                break;
            }

            qDebug("4");
            movieData.begin = index;
            video_name = std::string("video_") + std::to_string((long double)movies_count) + std::string(".avi");
            movieData.name = video_name;

            while( motion[index] == 1 )
            {
                ++index;
                if( index == motion.size() )
                {
                    break_flag = 1;
                    break;
                }
            }
            movieData.end = index - 1;
            if( break_flag)
            {
                break;
            }
            fragmentList.push_back(movieData);

        }



        fps = movie.get(CV_CAP_PROP_FPS);
        maxFrame;
        if (allFrames.size()>counter)
            maxFrame = allFrames.size();
        else
            maxFrame = counter;

        if((movie.get(CV_CAP_PROP_FRAME_COUNT)/maxFrame) > 1.2 )
        {
            fps = fps/2;
        }

        k;
        mSec = (int)(1000/fps);
        curFragment = 0;
        curFrame = fragmentList[0].begin;
        begin = fragmentList[0].begin;
        end = fragmentList[0].end;
        stop = false;
        esc = false;

    qDebug("DONE!");
    }

    runing = false;




}


void MainWindow::isdone1(){

    isDone[1] = true;

    ui->Status->setText("\n\n");

    for(int i=0;i<17;i++){
        if(isOn[i]==true){
            if(isDone[i]==true){
                ui->Status->setText(ui->Status->toPlainText() + "\nZakończono");
            } else {
                ui->Status->setText(ui->Status->toPlainText() + "\nTrwa");
            }
        }
    }



}

void MainWindow::isdone2(){

    isDone[2] = true;

    ui->Status->setText("\n\n");

    for(int i=0;i<17;i++){
        if(isOn[i]==true){
            if(isDone[i]==true){
                ui->Status->setText(ui->Status->toPlainText() + "\nZakończono");
            } else {
                ui->Status->setText(ui->Status->toPlainText() + "\nTrwa");
            }
        }
    }


}

void MainWindow::isdone3(){

    isDone[3] = true;

    ui->Status->setText("\n");

    for(int i=0;i<17;i++){
        if(isOn[i]==true){
            if(isDone[i]==true){
                ui->Status->setText(ui->Status->toPlainText() + "\nZakończono");
            } else {
                ui->Status->setText(ui->Status->toPlainText() + "\nTrwa");
            }
        }
    }




}

void MainWindow::isdone4(){

    isDone[4] = true;

    ui->Status->setText("\n\n");

    for(int i=0;i<17;i++){
        if(isOn[i]==true){
            if(isDone[i]==true){
                ui->Status->setText(ui->Status->toPlainText() + "\nZakończono");
            } else {
                ui->Status->setText(ui->Status->toPlainText() + "\nTrwa");
            }
        }
    }


}

void MainWindow::isdone5(){

    isDone[5] = true;

    ui->Status->setText("\n\n");

    for(int i=0;i<17;i++){
        if(isOn[i]==true){
            if(isDone[i]==true){
                ui->Status->setText(ui->Status->toPlainText() + "\nZakończono");
            } else {
                ui->Status->setText(ui->Status->toPlainText() + "\nTrwa");
            }
        }
    }


}

void MainWindow::isdone6(){

    isDone[6] = true;

    ui->Status->setText("\n\n");

    for(int i=0;i<17;i++){
        if(isOn[i]==true){
            if(isDone[i]==true){
                ui->Status->setText(ui->Status->toPlainText() + "\nZakończono");
            } else {
                ui->Status->setText(ui->Status->toPlainText() + "\nTrwa");
            }
        }
    }


}

void MainWindow::isdone7(){

    isDone[7] = true;

    ui->Status->setText("\n\n");

    for(int i=0;i<17;i++){
        if(isOn[i]==true){
            if(isDone[i]==true){
                ui->Status->setText(ui->Status->toPlainText() + "\nZakończono");
            } else {
                ui->Status->setText(ui->Status->toPlainText() + "\nTrwa");
            }
        }
    }


}

void MainWindow::isdone8(){

    isDone[8] = true;

    ui->Status->setText("\n\n");

    for(int i=0;i<17;i++){
        if(isOn[i]==true){
            if(isDone[i]==true){
                ui->Status->setText(ui->Status->toPlainText() + "\nZakończono");
            } else {
                ui->Status->setText(ui->Status->toPlainText() + "\nTrwa");
            }
        }
    }


}

void MainWindow::on_actionAutorzy_triggered()
{
    QMessageBox::about(this,"Autorzy","Paweł Brańka\nKamil Jamróz\nMateusz Juraszek\nMirosław Małek\nRafał Płonka\nMateusz Stanaszek\nDominik Wyroba");
}

void MainWindow::on_actionO_programie_triggered()
{
    QMessageBox::about(this,"O programie","Bardzo dobry program służący do detekcji ruchu w filmach");
}




