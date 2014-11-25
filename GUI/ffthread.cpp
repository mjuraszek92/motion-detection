#include "ffthread.h"
#include "full_auto.cpp"

FFthread::FFthread(std::string pathd, std::map<std::string, double> parameters)
{
    parametry = parameters;
    path = pathd;

}

void FFthread::stop(){

        qDebug()<<"Thread::stop called from main thread: "<<currentThreadId();
        QMutexLocker locker(&m_mutex);
        m_stop=true;

}

void FFthread::set(std::string path, std::map<std::string,double> parameters){



}

void FFthread::run(){




        qDebug() << "hello";
       motion_detection_full_auto(path,parametry);
       qDebug() << "END";


}
