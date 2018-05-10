#include "pulsedatareceiver.h"
#include <QDateTime>

PulseDataReceiver::PulseDataReceiver(QXYSeries * series, QObject *parent) :
    QIODevice(parent),
    m_series(series)
{
}

qint64 PulseDataReceiver::readData(char *data, qint64 maxSize){
    Q_UNUSED(data)
    Q_UNUSED(maxSize)
    return -1;
}


void PulseDataReceiver::newSeries(QXYSeries * series, float inter){
    m_series = series;
    newSer = true;
    interval = inter;
}

qint64 PulseDataReceiver::writeData(const char *data, qint64 maxSize){
    if(newSer){
        index = 0;
    }newSer = false;
    short* s_data = (short*) data;
    for(int i = 0; i < maxSize/2; i ++){
        qDebug()<<s_data[i]<<endl;
        m_series->append(index * interval,s_data[i]);
        index++;
    }
    return maxSize;
}

