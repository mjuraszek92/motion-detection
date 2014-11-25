#include "mainwindow.h"
#include "ui_mainwindow.h"



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



     //connect(thread, SIGNAL(started()), thread, SLOT(callAuto2()));






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
    ui->auto2_sel_text2->setDisabled(true);
    ui->Lp->setDisabled(true);
    ui->Name->setDisabled(true);
    ui->Status->setDisabled(true);

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
    GapSize = 60;
    MotionLen = 30;
    BeforeMove = 30;
    AfterMove = 30;
    Area = 0.001;
    History = 100;
    Mixtures = 3;
    Threads = 1;
    Metod = 1;
    Save = 0;

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


}

void MainWindow::clickTrybAuto1()
{
    tryb = 1;
    ui->Auto1->show();
}

void MainWindow::clickTrybAuto2()
{
    tryb = 2;
    ui->label_6->setText("Tryb Automatyczny 2");
    ui->Auto2->show();
    ui->label_3->show();
    ui->Threads->show();
}


void MainWindow::clickTrybManu()
{
    ui->label_6->setText("Tryb Manualny");
    ui->Auto2->show();
    ui->zapis->show();
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


                                            if(tryb==2){
                                                if (!runing){
                                                    ui->auto2_start->setText("Przerwij");
                                                    runing = true;
                                                    ui->auto2_back->setDisabled(true);

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
                                                        std::string path = fileName.toStdString();




                                                        //thread = new GGthread(path,parametry);
                                                        //thread->start();


                                                        for (int i=0;i<parametry["threads"];i++){
                                                            Sleep(50);
                                                           gthredy[i] = new GGthread(path,parametry);
                                                           gthredy[i]->start();
                                                            parametry["thread_no"] = parametry["thread_no"] + 1;
                                                        }






                                                } else{
                                                    ui->auto2_start->setText("Start");
                                                    runing = false;
                                                    ui->auto2_back->setDisabled(false);

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
                                                        std::string path = "F://film2.avi";

                                                    //fthredy[nr-1] = new FFthread(path,parametry);
                                                    //fthredy[nr-1]->start();


                                                } else if(tryb==0){
                                                    ui->Auto2->hide();
                                                    ui->Man->show();
                                                    //run
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
                                                    "/home",
                                                    tr("filmy (*.avi)"));

     string tmp = fileName.toStdString();

     //auto it = std::find(tmp.begin(), tmp.end(), '/');
     //while (it != tmp.end()) {
     //    auto it2 = tmp.insert(it, '/');

         // skip over the slashes we just inserted
     //    it = std::find(it2+1, tmp.end(), '/');
    // }

    fileName = QString::fromStdString(tmp);

    ui->auto2_sel_text1->setText(fileName);
}

void MainWindow::clickAuto2sel2()
{
     dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/home",
                                                      QFileDialog::DontResolveSymlinks);





    ui->auto2_sel_text2->setText(dir);
}



//Tryb Manualny
void MainWindow::clickManback()
{
    ui->Man->hide();
    ui->zapis->hide();

}

void MainWindow::clickMannext()
{
    ui->Man->hide();
    ui->Auto2->show();



}


void MainWindow::updateImg(){




}

void MainWindow::clickMansave()
{


}


void MainWindow::clickAuto1back()
{

    ui->Auto1->hide();
    setDefPara();
    nr = 0;

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


}

void MainWindow::clickMan5plus(){


}

void MainWindow::clickMan1plus(){


}

void MainWindow::clickMan5minus(){


}

void MainWindow::clickMan1minus(){


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

void MainWindow::on_actionAutorzy_triggered()
{
    QMessageBox::about(this,"Autorzy","Paweł Brańka\nKamil Jamróz\nMateusz Juraszek\nMirosław Małek\nRafał Płonka\nMateusz Stanaszek\nDominik Wyroba");
}

void MainWindow::on_actionO_programie_triggered()
{
    QMessageBox::about(this,"O programie","Bardzo dobry program służący do detekcji ruchu w filmach");
}




