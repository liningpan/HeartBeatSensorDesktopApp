#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pulsedatareceiver.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pdr(new PulseDataReceiver(0,this))
{
    ui->setupUi(this);
    on_updateButton_clicked();
    //initially disable all input except COM
    connected=false;
    setSampleParameterDisable(true);

    m_chart = new QChart;
    ui->chartView->setChart(m_chart);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(releaseStart()));
}

void MainWindow::releaseStart(){
    setSampleParameterDisable(false);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createNewSeries(){
    m_chart->removeAllSeries();
    currentSeries = new QLineSeries(this);
    pdr->newSeries(currentSeries, 1.0/pd->getSampleRate());
    pd->setDevice(pdr);
    x = new QValueAxis(this);
    x->setRange(0,pd->getDuration());
    x->setLabelFormat("%.2f");
    x->setTitleText("Time(sec)");

    y = new QValueAxis(this);
    y->setRange(0,1024);
    y->setLabelFormat("%d");
    y->setTitleText("ReadOut");

    m_chart->addSeries(currentSeries);

    m_chart->setAxisX(x);
    m_chart->setAxisY(y);

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

void MainWindow::setSampleParameterDisable(bool disable){
    ui->sampleRateBox->setDisabled(disable);
    ui->updateRateBox->setDisabled(disable);
    ui->durationBox->setDisabled(disable);
    ui->startButton->setDisabled(disable);
}
void MainWindow::on_connectButton_clicked(){
    int index = ui->COMPortComboBox->currentIndex();
    pd = new PulseDevice(serialPortList.at(index));
    connect(pd,SIGNAL(finishRead()),this,SLOT(finishRead()));
    ui->COMPortComboBox->setDisabled(true);
    ui->updateButton->setDisabled(true);
    ui->connectButton->setText("Disconnect");
    //enable everything else
    setSampleParameterDisable(false);
}

void MainWindow::on_startButton_clicked(){
    setSampleParameterDisable(true);
    createNewSeries();
    timer->start(1000*pd->getDuration());
    pd->start();
}

void MainWindow::on_sampleRateBox_currentIndexChanged(int v){
    pd->setSampleRate(ui->sampleRateBox->currentIndex());
}

void MainWindow::on_updateRateBox_currentIndexChanged(int v){
    pd->setUpdateRate(ui->updateRateBox->currentIndex());
}

void MainWindow::on_durationBox_valueChanged(int v){
    pd->setDuration(ui->durationBox->value());
}

void MainWindow::on_powerSwitch_clicked(){
    pd->togglePower();
}

void MainWindow::finishRead(){
    setSampleParameterDisable(false);
}
