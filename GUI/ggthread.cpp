#include "ggthread.h"
#include "thread_auto.cpp"

GGthread::GGthread(std::string pathd, std::map<std::string, double> parameters)
{
    parametry = parameters;
    path = pathd;

}

void GGthread::stop(){

        qDebug()<<"Thread::stop called from main thread: "<<currentThreadId();
        QMutexLocker locker(&m_mutex);
        m_stop=true;

}

void GGthread::set(std::string path, std::map<std::string,double> parameters){



}

void GGthread::run(){




        qDebug() << "hello";
      motion_detection_threads_auto(path,parametry);
       qDebug() << "END";


}
