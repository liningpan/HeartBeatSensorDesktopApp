#include "pulsedevice.h"

PulseDevice::PulseDevice(QSerialPortInfo serialInfo, QObject* parent):QIODevice(parent)
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
    unsigned char* data = new unsigned char[512];
    int l = serialPort->read((char*)data, 512);
//    for(int i = 0 ; i < l ; i ++){
//        qDebug()<<QString::number(data[i],16)<<endl;
//    }
    if(l > 0){
        outputDevice->write((char*)data,l);
//        short* nudata = (short*) data;
//        for(int i = 0 ; i < l/2 ; i ++){
//            qDebug()<<nudata[i]<<endl;
//        }
        byteLeft -= l/2;
        //qDebug()<<byteLeft<<endl;
    }
    if(byteLeft > 0){
        timer->start();
    }else{
        emit(finishRead());
    }
}
void PulseDevice::writeCommand(int arg1, int arg2){
    char* data = new char[5];
    data[0] = arg1;
    data[1] = arg2;
    data[2] = '\n';
    data[3] = '\0';
    serialPort->write(data,3);
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
    qDebug()<<duration;
    serialPort->clear();
    writeCommand(2,duration);
    qDebug()<<int(1000.0/sampleRate*updateRate);
    timer->setInterval(int(1000.0/sampleRate*updateRate));
    timer->setSingleShot(true);
    byteLeft = duration * sampleRate;
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
