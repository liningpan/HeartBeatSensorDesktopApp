#include "pulsedevice.h"

PulseDevice::PulseDevice(QSerialPortInfo serialInfo, QObject* parent):QIODevice(parent),
    st(0),
    ed(0)
{
    serialPort = new QSerialPort(serialInfo);
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->open(QIODevice::ReadWrite);
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutAndListen()));
    sampleRate = 10;
    updateRate = 10;
    duration = 1;
    power = false;
}

void PulseDevice::timeoutAndListen(){
    //read new data
    qDebug()<<"Ready to read";
    char* buf = new char[2048];
    memset(buf,0,2048);
    int len = serialPort->read(buf,1024);
    int l = strlen(buf);
    for(int i = 0; i < l; i++){
        if(buf[i] == '\n'){
            //convert
            qDebug()<<st<<ed;
            char* data = new char[ed - st + 1];
            for(int j = st; j < ed; j ++){
                data[j - st] = cycbuf[j % 512];
            }data[ed - st] = '\0';
            qDebug()<<QString(data);
            QStringList list = QString(data).split(',');
            InputDataStruct* ids = new InputDataStruct;
            if(list.size()<3){
                continue;
            }
            ids->bmp = list[0].toInt();
            ids->raw_value = list[2].toInt();
            outputDevice->write((char*)ids,1);
            st = ed;
        } else {
            cycbuf[ed%512] = buf[i];
            //qDebug()<<st<<ed;
            ed++;
        }
    }
    delete[] buf;
}
void PulseDevice::writeCommand(int arg1, int arg2){
//    char* data = new char[5];
//    data[0] = arg1;
//    data[1] = arg2;
//    data[2] = '\n';
//    data[3] = '\0';
//    serialPort->write(data,3);
}
void PulseDevice::setDevice(QIODevice *outd){
    outputDevice = outd;
    outputDevice->open(ReadWrite);
    deviceSet = true;
}
void PulseDevice::setSampleRate(int index){
    int sampleRateOptions[]={10,20,50,100};
    writeCommand(3,index+1);
    sampleRate = sampleRateOptions[index];
}

void PulseDevice::setUpdateRate(int index){
    int updateRateOptions[]={10,20,50,100};
    writeCommand(4,index+1);
    updateRate = updateRateOptions[index];
}

void PulseDevice::setDuration(int value){
    duration = value;
}

void PulseDevice::start(){
    serialPort->clear();
    timer->setInterval(20);
    timer->setSingleShot(false);
    timer->start();
}

void PulseDevice::stop(){
    timer->stop();
}

void PulseDevice::togglePower(){
    power = !power;
    writeCommand(1,power);
}

PulseDevice::~PulseDevice(){
    delete serialPort;
    delete timer;
}
