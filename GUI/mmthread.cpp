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

#include "mmthread.h"
#include "manualChanged.cpp"

mmthread::mmthread(std::string pathd, std::map<std::string, double> parameters, std::deque<std::vector<uchar> > *allFrames, std::deque<int> *motion, cv::VideoCapture *movie, int *width, int *height, int *counter)
{
    parametry = parameters;
    path = pathd;
    tallFrames = allFrames;
    tmotion = motion;
    tmovie = movie;
    twidth = width;
    theight = height;
    tcounter = counter;

//    ttallFrames = allFrames;
//    ttmotion = motion;
//    ttmovie = movie;
//    ttwidth = width;
//    ttheight = height;
//    ttcounter = counter;


}

void mmthread::stop(){

        qDebug()<<"Thread::stop called from main thread: "<<currentThreadId();
        QMutexLocker locker(&m_mutex);
        m_stop=true;

}



void mmthread::run(){




        qDebug() << "hello";
       motion_detection(path,parametry, *tallFrames, *tmotion, *tmovie, *twidth, *theight, *tcounter);
       qDebug() << "END";


}
