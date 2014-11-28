/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionBrak;
    QAction *actionO_programie;
    QAction *actionAutorzy;
    QWidget *centralWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QWidget *Auto2;
    QPushButton *auto2_back;
    QPushButton *auto2_start;
    QPushButton *auto2_select1;
    QPushButton *auto2_select2;
    QLineEdit *auto2_sel_text1;
    QLineEdit *auto2_sel_text2;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *Threads;
    QLabel *label_3;
    QLabel *label_4;
    QRadioButton *auto2_met1;
    QRadioButton *auto2_met2;
    QLabel *label_6;
    QLineEdit *FrameSkip;
    QLineEdit *GapSize;
    QLineEdit *MotionLen;
    QLineEdit *BeforeMove;
    QLineEdit *AfterMove;
    QLineEdit *Area;
    QLineEdit *History;
    QLineEdit *Mixtures;
    QLabel *label_5;
    QLabel *label_11;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QLabel *label_23;
    QLabel *label_24;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *label_27;
    QPushButton *auto2_def;
    QWidget *zapis;
    QRadioButton *zapis1;
    QRadioButton *zapis2;
    QLabel *label_12;
    QWidget *Man;
    QPushButton *man_back;
    QPushButton *man_next;
    QPushButton *man_save;
    QSlider *horizontalSlider;
    QPushButton *man_play;
    QPushButton *man_5plus;
    QPushButton *man_5minus;
    QPushButton *man_1plus;
    QPushButton *man_1minus;
    QLabel *label_22;
    QPushButton *go_start;
    QPushButton *go_end;
    QPushButton *set_start;
    QPushButton *set_end;
    QWidget *Auto1;
    QLabel *label_10;
    QPushButton *auto1_back;
    QPushButton *auto1_add;
    QFrame *line;
    QFrame *line_3;
    QLabel *label_28;
    QLabel *label_29;
    QLabel *label_30;
    QTextEdit *Lp;
    QTextEdit *Name;
    QTextEdit *Status;
    QMenuBar *menuBar;
    QMenu *menuPlik;
    QMenu *menuEdycja;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(860, 640);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionBrak = new QAction(MainWindow);
        actionBrak->setObjectName(QStringLiteral("actionBrak"));
        actionO_programie = new QAction(MainWindow);
        actionO_programie->setObjectName(QStringLiteral("actionO_programie"));
        actionAutorzy = new QAction(MainWindow);
        actionAutorzy->setObjectName(QStringLiteral("actionAutorzy"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(350, 120, 171, 61));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(350, 280, 171, 61));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setEnabled(true);
        pushButton_3->setGeometry(QRect(350, 410, 171, 61));
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(350, 200, 171, 61));
        Auto2 = new QWidget(centralWidget);
        Auto2->setObjectName(QStringLiteral("Auto2"));
        Auto2->setGeometry(QRect(0, 30, 861, 611));
        auto2_back = new QPushButton(Auto2);
        auto2_back->setObjectName(QStringLiteral("auto2_back"));
        auto2_back->setGeometry(QRect(640, 450, 171, 61));
        auto2_start = new QPushButton(Auto2);
        auto2_start->setObjectName(QStringLiteral("auto2_start"));
        auto2_start->setGeometry(QRect(640, 360, 171, 61));
        auto2_select1 = new QPushButton(Auto2);
        auto2_select1->setObjectName(QStringLiteral("auto2_select1"));
        auto2_select1->setGeometry(QRect(640, 50, 171, 31));
        auto2_select2 = new QPushButton(Auto2);
        auto2_select2->setObjectName(QStringLiteral("auto2_select2"));
        auto2_select2->setGeometry(QRect(640, 100, 171, 31));
        auto2_sel_text1 = new QLineEdit(Auto2);
        auto2_sel_text1->setObjectName(QStringLiteral("auto2_sel_text1"));
        auto2_sel_text1->setEnabled(true);
        auto2_sel_text1->setGeometry(QRect(230, 49, 391, 31));
        auto2_sel_text2 = new QLineEdit(Auto2);
        auto2_sel_text2->setObjectName(QStringLiteral("auto2_sel_text2"));
        auto2_sel_text2->setEnabled(true);
        auto2_sel_text2->setGeometry(QRect(230, 99, 391, 31));
        label = new QLabel(Auto2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(120, 60, 91, 16));
        label_2 = new QLabel(Auto2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(120, 110, 91, 16));
        Threads = new QLineEdit(Auto2);
        Threads->setObjectName(QStringLiteral("Threads"));
        Threads->setGeometry(QRect(720, 190, 31, 20));
        label_3 = new QLabel(Auto2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(630, 190, 71, 20));
        label_4 = new QLabel(Auto2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(350, 200, 91, 16));
        auto2_met1 = new QRadioButton(Auto2);
        auto2_met1->setObjectName(QStringLiteral("auto2_met1"));
        auto2_met1->setGeometry(QRect(460, 190, 131, 17));
        auto2_met2 = new QRadioButton(Auto2);
        auto2_met2->setObjectName(QStringLiteral("auto2_met2"));
        auto2_met2->setGeometry(QRect(460, 210, 111, 17));
        label_6 = new QLabel(Auto2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(330, 0, 231, 31));
        QFont font;
        font.setPointSize(18);
        label_6->setFont(font);
        FrameSkip = new QLineEdit(Auto2);
        FrameSkip->setObjectName(QStringLiteral("FrameSkip"));
        FrameSkip->setGeometry(QRect(190, 280, 41, 20));
        GapSize = new QLineEdit(Auto2);
        GapSize->setObjectName(QStringLiteral("GapSize"));
        GapSize->setGeometry(QRect(190, 310, 41, 20));
        MotionLen = new QLineEdit(Auto2);
        MotionLen->setObjectName(QStringLiteral("MotionLen"));
        MotionLen->setGeometry(QRect(190, 340, 41, 20));
        BeforeMove = new QLineEdit(Auto2);
        BeforeMove->setObjectName(QStringLiteral("BeforeMove"));
        BeforeMove->setGeometry(QRect(190, 370, 41, 20));
        AfterMove = new QLineEdit(Auto2);
        AfterMove->setObjectName(QStringLiteral("AfterMove"));
        AfterMove->setGeometry(QRect(460, 280, 41, 20));
        Area = new QLineEdit(Auto2);
        Area->setObjectName(QStringLiteral("Area"));
        Area->setGeometry(QRect(460, 310, 41, 20));
        History = new QLineEdit(Auto2);
        History->setObjectName(QStringLiteral("History"));
        History->setGeometry(QRect(460, 340, 41, 20));
        Mixtures = new QLineEdit(Auto2);
        Mixtures->setObjectName(QStringLiteral("Mixtures"));
        Mixtures->setGeometry(QRect(460, 370, 41, 20));
        label_5 = new QLabel(Auto2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(70, 280, 91, 16));
        label_11 = new QLabel(Auto2);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(70, 310, 91, 16));
        label_13 = new QLabel(Auto2);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(70, 370, 111, 16));
        label_14 = new QLabel(Auto2);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(350, 280, 101, 16));
        label_15 = new QLabel(Auto2);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(350, 310, 81, 16));
        label_16 = new QLabel(Auto2);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(70, 340, 81, 16));
        label_17 = new QLabel(Auto2);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(350, 340, 81, 16));
        label_18 = new QLabel(Auto2);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(350, 370, 81, 16));
        label_19 = new QLabel(Auto2);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(240, 280, 46, 13));
        label_20 = new QLabel(Auto2);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(240, 310, 46, 13));
        label_21 = new QLabel(Auto2);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(240, 340, 46, 13));
        label_23 = new QLabel(Auto2);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(240, 370, 46, 13));
        label_24 = new QLabel(Auto2);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(510, 280, 46, 13));
        label_25 = new QLabel(Auto2);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setGeometry(QRect(510, 310, 46, 13));
        label_26 = new QLabel(Auto2);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(510, 340, 46, 13));
        label_27 = new QLabel(Auto2);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setGeometry(QRect(510, 370, 46, 13));
        auto2_def = new QPushButton(Auto2);
        auto2_def->setObjectName(QStringLiteral("auto2_def"));
        auto2_def->setGeometry(QRect(100, 460, 75, 23));
        zapis = new QWidget(Auto2);
        zapis->setObjectName(QStringLiteral("zapis"));
        zapis->setGeometry(QRect(40, 170, 261, 80));
        zapis1 = new QRadioButton(zapis);
        zapis1->setObjectName(QStringLiteral("zapis1"));
        zapis1->setGeometry(QRect(170, 20, 82, 17));
        zapis2 = new QRadioButton(zapis);
        zapis2->setObjectName(QStringLiteral("zapis2"));
        zapis2->setGeometry(QRect(170, 40, 82, 17));
        label_12 = new QLabel(zapis);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(30, 30, 131, 16));
        Man = new QWidget(centralWidget);
        Man->setObjectName(QStringLiteral("Man"));
        Man->setGeometry(QRect(0, 0, 851, 601));
        man_back = new QPushButton(Man);
        man_back->setObjectName(QStringLiteral("man_back"));
        man_back->setGeometry(QRect(640, 500, 171, 61));
        man_next = new QPushButton(Man);
        man_next->setObjectName(QStringLiteral("man_next"));
        man_next->setGeometry(QRect(640, 420, 171, 61));
        man_save = new QPushButton(Man);
        man_save->setObjectName(QStringLiteral("man_save"));
        man_save->setGeometry(QRect(640, 340, 171, 61));
        horizontalSlider = new QSlider(Man);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(80, 540, 381, 19));
        horizontalSlider->setOrientation(Qt::Horizontal);
        man_play = new QPushButton(Man);
        man_play->setObjectName(QStringLiteral("man_play"));
        man_play->setGeometry(QRect(240, 500, 51, 23));
        man_5plus = new QPushButton(Man);
        man_5plus->setObjectName(QStringLiteral("man_5plus"));
        man_5plus->setGeometry(QRect(340, 500, 31, 23));
        man_5minus = new QPushButton(Man);
        man_5minus->setObjectName(QStringLiteral("man_5minus"));
        man_5minus->setGeometry(QRect(160, 500, 31, 23));
        man_1plus = new QPushButton(Man);
        man_1plus->setObjectName(QStringLiteral("man_1plus"));
        man_1plus->setGeometry(QRect(300, 500, 31, 23));
        man_1minus = new QPushButton(Man);
        man_1minus->setObjectName(QStringLiteral("man_1minus"));
        man_1minus->setGeometry(QRect(200, 500, 31, 23));
        label_22 = new QLabel(Man);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(40, 60, 501, 401));
        go_start = new QPushButton(Man);
        go_start->setObjectName(QStringLiteral("go_start"));
        go_start->setGeometry(QRect(590, 170, 111, 41));
        go_end = new QPushButton(Man);
        go_end->setObjectName(QStringLiteral("go_end"));
        go_end->setGeometry(QRect(590, 220, 111, 41));
        set_start = new QPushButton(Man);
        set_start->setObjectName(QStringLiteral("set_start"));
        set_start->setGeometry(QRect(710, 170, 111, 41));
        set_end = new QPushButton(Man);
        set_end->setObjectName(QStringLiteral("set_end"));
        set_end->setGeometry(QRect(710, 220, 111, 41));
        Auto1 = new QWidget(centralWidget);
        Auto1->setObjectName(QStringLiteral("Auto1"));
        Auto1->setGeometry(QRect(0, 0, 861, 601));
        label_10 = new QLabel(Auto1);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(330, 30, 231, 31));
        label_10->setFont(font);
        auto1_back = new QPushButton(Auto1);
        auto1_back->setObjectName(QStringLiteral("auto1_back"));
        auto1_back->setGeometry(QRect(640, 500, 171, 61));
        auto1_add = new QPushButton(Auto1);
        auto1_add->setObjectName(QStringLiteral("auto1_add"));
        auto1_add->setGeometry(QRect(640, 180, 171, 61));
        line = new QFrame(Auto1);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(70, 130, 501, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(Auto1);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(450, 100, 20, 441));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);
        label_28 = new QLabel(Auto1);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setGeometry(QRect(80, 110, 51, 21));
        QFont font1;
        font1.setPointSize(12);
        label_28->setFont(font1);
        label_29 = new QLabel(Auto1);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setGeometry(QRect(250, 110, 51, 21));
        label_29->setFont(font1);
        label_30 = new QLabel(Auto1);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setGeometry(QRect(490, 110, 51, 21));
        label_30->setFont(font1);
        Lp = new QTextEdit(Auto1);
        Lp->setObjectName(QStringLiteral("Lp"));
        Lp->setGeometry(QRect(70, 100, 61, 441));
        Name = new QTextEdit(Auto1);
        Name->setObjectName(QStringLiteral("Name"));
        Name->setGeometry(QRect(120, 100, 341, 441));
        Status = new QTextEdit(Auto1);
        Status->setObjectName(QStringLiteral("Status"));
        Status->setGeometry(QRect(460, 100, 111, 441));
        Lp->raise();
        Status->raise();
        Name->raise();
        label_10->raise();
        auto1_back->raise();
        auto1_add->raise();
        line->raise();
        line_3->raise();
        label_28->raise();
        label_29->raise();
        label_30->raise();
        MainWindow->setCentralWidget(centralWidget);
        pushButton->raise();
        pushButton_2->raise();
        pushButton_3->raise();
        pushButton_4->raise();
        Auto1->raise();
        Auto2->raise();
        Man->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 860, 21));
        menuPlik = new QMenu(menuBar);
        menuPlik->setObjectName(QStringLiteral("menuPlik"));
        menuEdycja = new QMenu(menuBar);
        menuEdycja->setObjectName(QStringLiteral("menuEdycja"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setEnabled(true);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuPlik->menuAction());
        menuBar->addAction(menuEdycja->menuAction());
        menuPlik->addAction(actionExit);
        menuEdycja->addAction(actionO_programie);
        menuEdycja->addAction(actionAutorzy);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionBrak->setText(QApplication::translate("MainWindow", "O programie", 0));
        actionO_programie->setText(QApplication::translate("MainWindow", "O programie", 0));
        actionAutorzy->setText(QApplication::translate("MainWindow", "Autorzy", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Tryb Auto Multi", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Tryb Manualny", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Wyj\305\233cie", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "Tryb Auto Solo", 0));
        auto2_back->setText(QApplication::translate("MainWindow", "Wstecz", 0));
        auto2_start->setText(QApplication::translate("MainWindow", "Start", 0));
        auto2_select1->setText(QApplication::translate("MainWindow", "Przegl\304\205daj", 0));
        auto2_select2->setText(QApplication::translate("MainWindow", "Przegl\304\205daj", 0));
        label->setText(QApplication::translate("MainWindow", "\305\232cie\305\274ka Odczytu:", 0));
        label_2->setText(QApplication::translate("MainWindow", "\305\232cie\305\274ka Zapisu:", 0));
        label_3->setText(QApplication::translate("MainWindow", "Ilo\305\233\304\207 w\304\205tk\303\263w:", 0));
        label_4->setText(QApplication::translate("MainWindow", "Metoda Detekcji:", 0));
        auto2_met1->setText(QApplication::translate("MainWindow", "Obejmowanie ramek", 0));
        auto2_met2->setText(QApplication::translate("MainWindow", "Metoda Gaussa", 0));
        label_6->setText(QApplication::translate("MainWindow", "Tryb Manualny", 0));
        label_5->setText(QApplication::translate("MainWindow", "Pomini\304\231cie ramek:", 0));
        label_11->setText(QApplication::translate("MainWindow", "D\305\202ugo\305\233\304\207 przerwy:", 0));
        label_13->setText(QApplication::translate("MainWindow", "Wyprzedzanie ruchu:", 0));
        label_14->setText(QApplication::translate("MainWindow", "Przed\305\202u\305\274anie ruchu:", 0));
        label_15->setText(QApplication::translate("MainWindow", "Pole obiektu:", 0));
        label_16->setText(QApplication::translate("MainWindow", "D\305\202ugo\305\233\304\207 ruchu:", 0));
        label_17->setText(QApplication::translate("MainWindow", "Historia:", 0));
        label_18->setText(QApplication::translate("MainWindow", "Ilo\305\233\304\207 mieszanin:", 0));
        label_19->setText(QApplication::translate("MainWindow", "(0-N)", 0));
        label_20->setText(QApplication::translate("MainWindow", "(0-N)", 0));
        label_21->setText(QApplication::translate("MainWindow", "(1-N)", 0));
        label_23->setText(QApplication::translate("MainWindow", "(0-N)", 0));
        label_24->setText(QApplication::translate("MainWindow", "(0-N)", 0));
        label_25->setText(QApplication::translate("MainWindow", "(0-N)", 0));
        label_26->setText(QApplication::translate("MainWindow", "(10-N)", 0));
        label_27->setText(QApplication::translate("MainWindow", "(2-7)", 0));
        auto2_def->setText(QApplication::translate("MainWindow", "Domy\305\233lne", 0));
        zapis1->setText(QApplication::translate("MainWindow", "Pami\304\231\304\207 RAM", 0));
        zapis2->setText(QApplication::translate("MainWindow", "Dysk", 0));
        label_12->setText(QApplication::translate("MainWindow", "Przechowywanie danych:", 0));
        man_back->setText(QApplication::translate("MainWindow", "Wstecz", 0));
        man_next->setText(QApplication::translate("MainWindow", "Nast\304\231pny", 0));
        man_save->setText(QApplication::translate("MainWindow", "Zapisz", 0));
        man_play->setText(QApplication::translate("MainWindow", "play", 0));
        man_5plus->setText(QApplication::translate("MainWindow", "+5", 0));
        man_5minus->setText(QApplication::translate("MainWindow", "-5", 0));
        man_1plus->setText(QApplication::translate("MainWindow", "+1", 0));
        man_1minus->setText(QApplication::translate("MainWindow", "-1", 0));
        label_22->setText(QString());
        go_start->setText(QApplication::translate("MainWindow", "id\305\272 na\n"
" pocz\304\205tek fragmentu", 0));
        go_end->setText(QApplication::translate("MainWindow", "id\305\272 na\n"
" koniec fragmentu", 0));
        set_start->setText(QApplication::translate("MainWindow", "ustaw\n"
" pocz\304\205tek fragmentu", 0));
        set_end->setText(QApplication::translate("MainWindow", "ustaw\n"
" pocz\304\205tek fragmentu", 0));
        label_10->setText(QApplication::translate("MainWindow", "Tryb Automatyczny", 0));
        auto1_back->setText(QApplication::translate("MainWindow", "Wstecz", 0));
        auto1_add->setText(QApplication::translate("MainWindow", "Dodaj", 0));
        label_28->setText(QApplication::translate("MainWindow", "Lp.", 0));
        label_29->setText(QApplication::translate("MainWindow", "Tytu\305\202", 0));
        label_30->setText(QApplication::translate("MainWindow", "Status", 0));
        menuPlik->setTitle(QApplication::translate("MainWindow", "Plik", 0));
        menuEdycja->setTitle(QApplication::translate("MainWindow", "Pomoc", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
