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


void PulseDataReceiver::newSeries(QXYSeries * series, QValueAxis *x_axis, float timespan,QLCDNumber* lcdn){
    m_series = series;
    m_axis = x_axis;
    newSer = true;
    viewing_timespan = timespan;
    offest = 0;
    m_lcd = lcdn;
}

qint64 PulseDataReceiver::writeData(const char *data, qint64 maxSize){
    InputDataStruct* data_i = (InputDataStruct*)data;
    qint64 range = 50;
    QVector<QPointF> oldPoints = m_series->pointsVector();
    QVector<QPointF> points;
    if(oldPoints.count() < range*viewing_timespan) {
        points = m_series->pointsVector();
    } else {
        offest += maxSize;
        for(int i = maxSize; i < oldPoints.count(); i ++){
            points.append(QPointF((offest + i - maxSize)/50.0,oldPoints.at(i).y()));
        }
    }

    qint64 size = points.count();
    for(int k = 0; k < maxSize; k++){
        points.append(QPointF((offest + k + size)/50.0,data_i[k].raw_value));
    }
    m_series->replace(points);
    //update axis
    m_axis->setRange(offest/50.0, offest/50.0 + viewing_timespan);
    m_lcd->display(data_i[maxSize-1].bpm);
    delete data_i;
    return maxSize;
}

