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


}

void MainWindow::setDefPara(){

    FramesSkip = 0;
    GapSize = 10;
    MotionLen = 10;
    BeforeMove = 30;
    AfterMove = 30;
    Area = 0.005;
    History = 100;
    Mixtures = 3;
    Threads = 1;
    Metod = 1;

    ui->FrameSkip->setText(QString().number(FramesSkip));
    ui->GapSize->setText(QString().number(GapSize));
    ui->MotionLen->setText(QString().number(MotionLen));
    ui->BeforeMove->setText(QString().number(BeforeMove));
    ui->AfterMove->setText(QString().number(AfterMove));
    ui->Area->setText(QString().number(Area));
    ui->History->setText(QString().number(History));
    ui->Mixtures->setText(QString().number(Mixtures));
    ui->Threads->setText(QString().number(Threads));

    ui->auto2_met1->toggle();

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
        Metod = 1;
    } else {
        Metod = 2;
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

                                                    //run
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
                                                    tr("All (*.*)"));
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

}

void MainWindow::clickMannext()
{
    ui->Man->hide();
    ui->Auto2->show();

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

void MainWindow::on_actionAutorzy_triggered()
{
    QMessageBox::about(this,"Autorzy","Paweł Brańka\nKamil Jamróz\nMateusz Juraszek\nMirosław Małek\nRafał Płonka\nMateusz Stanaszek\nDominik Wyroba");
}

void MainWindow::on_actionO_programie_triggered()
{
    QMessageBox::about(this,"O programie","Bardzo dobry program służący do detekcji ruchu w filmach");
}



