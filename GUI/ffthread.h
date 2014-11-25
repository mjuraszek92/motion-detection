#ifndef FFTHREAD_H
#define FFTHREAD_H

#include <QMainWindow>
#include <QtGui>
#include <QTextCodec>
#include <QMessageBox>
#include <QProgressBar>
#include <QString>
#include <QFileDialog>
#include <QThread>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

class FFthread : public QThread{

   Q_OBJECT
   public:
       FFthread(std::string path, std::map<std::string,double> parameters);
       void set(std::string path, std::map<std::string,double> parameters);
   public slots:
       void stop();


   private:
       QMutex m_mutex;
       bool m_stop;
       std::map<std::string,double> parametry;
       std::string path;

       void run();


};

#endif // FFTHREAD_H
