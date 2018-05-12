#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pulsedatareceiver.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pdr(new PulseDataReceiver(0,this)),
    start(false),
    connected(false)
{
    ui->setupUi(this);
    on_updateButton_clicked();
    //initially disable all input except COM

    //setSampleParameterDisable(true);
    ui->startButton->setDisabled(true);
    m_chart = new QChart;
    ui->chartView->setChart(m_chart);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(releaseStart()));
}

void MainWindow::releaseStart(){
    //setSampleParameterDisable(false);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateBpm(int bpm){
    ui->bpmLCD->display(bpm);
}

void MainWindow::createNewSeries(){
    m_chart->removeAllSeries();
    currentSeries = new QLineSeries(this);

    pd->setDevice(pdr);
    x = new QValueAxis(this);
    x->setRange(0,ui->durationBox->value());
    x->setLabelFormat("%.1f");
    x->setTitleText("Time(sec)");

    y = new QValueAxis(this);
    y->setRange(0,1024);
    y->setLabelFormat("%d");
    y->setTitleText("ReadOut");

    m_chart->addSeries(currentSeries);

    m_chart->setAxisX(x);
    m_chart->setAxisY(y);
    pdr->newSeries(currentSeries,x, ui->durationBox->value(),ui->bpmLCD);
    currentSeries->attachAxis(x);
    currentSeries->attachAxis(y);
}
void MainWindow::on_updateButton_clicked(){
    serialPortList = QSerialPortInfo().availablePorts();
    ui->COMPortComboBox->clear();
    for(int i = 0; i < serialPortList.size();i++){
        QSerialPortInfo tmp = serialPortList.at(i);
        ui->COMPortComboBox->addItem(tmp.portName()+ " " +tmp.description());
    }
}

void MainWindow::on_connectButton_clicked(){
    if(!connected){
        int index = ui->COMPortComboBox->currentIndex();
        pd = new PulseDevice(serialPortList.at(index));
        ui->COMPortComboBox->setDisabled(true);
        ui->updateButton->setDisabled(true);
        ui->connectButton->setText("Disconnect");
        ui->startButton->setDisabled(false);
    } else {
        if(start){
            on_startButton_clicked();
        }
        delete pd;
        ui->COMPortComboBox->setDisabled(false);
        ui->updateButton->setDisabled(false);
        ui->connectButton->setText("Connect");
        ui->startButton->setDisabled(true);
    }
    connected = !connected;
    //enable everything else
    //setSampleParameterDisable(false);
}

void MainWindow::on_startButton_clicked(){
//    setSampleParameterDisable(true);
    if(!start){
        createNewSeries();
        pd->start();
        ui->startButton->setText("Stop");
    }
    else{
        pd->stop();
        ui->startButton->setText("Start");
    }
    start = !start;
}

void MainWindow::on_durationBox_valueChanged(int v){
    pd->setDuration(ui->durationBox->value());
}

void MainWindow::finishRead(){
    //setSampleParameterDisable(false);
}
