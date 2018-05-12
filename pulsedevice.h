#ifndef PULSEDEVICE_H
#define PULSEDEVICE_H

#include <QObject>
#include <QIODevice>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <memory.h>
#include <cstring>
#include <QDebug>
#include "pulsedatareceiver.h"

class PulseDevice : public QIODevice
{
    Q_OBJECT
private:
    void writeCommand(int arg1, int arg2);
    QSerialPort* serialPort;
    QIODevice* outputDevice;
    QTimer* timer;
    int sampleRate;
    int updateRate;
    int duration;
    int byteLeft;
    bool power;
    bool deviceSet;
    char cycbuf[512];
    int st;
    int ed;
    qint64 readData(char *data, qint64 maxlen){return 0;}
    qint64 writeData(const char *data, qint64 len){return 0;}
public:
    explicit PulseDevice(QSerialPortInfo serialInfo, QObject* parent = 0);
    ~PulseDevice();
    void setSampleRate(int);
    void setUpdateRate(int);
    int getSampleRate(){ return sampleRate; }
    int getDuration(){ return duration; }
    void setDuration(int);
    void togglePower();
    void start();
    void stop();
    void setDevice(QIODevice* outd);
private slots:
    void timeoutAndListen();
//    PulseDevice();
signals:
    void finishRead();
};

#endif // PULSEDEVICE_H
