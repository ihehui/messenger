#include "imageviewercontroler.h"
#include "ui_imageviewercontroler.h"




ImageViewerControler::ImageViewerControler(QWidget *parent, Qt::WindowFlags fl) :
    QWidget(parent, fl),
    ui(new Ui::ImageViewerControlerUI)
{

    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground, true);

    setMouseTracking(true);

}

ImageViewerControler::~ImageViewerControler()
{
    delete ui;
}

void ImageViewerControler::reset(){
    ui->dial->setValue(180);
}

void ImageViewerControler::on_toolButtonRotateLeft_clicked(){


    int value = ui->dial->value();

    int a = value / 90 - 1;
    if(a <= 0){ a = 4;}
    ui->dial->setValue(a*90);

}

void ImageViewerControler::on_dial_valueChanged(int value){

    if(value <= 180){
        emit signalRotate(180 + value);
    }else{
        emit signalRotate(value - 180);
    }


}

void ImageViewerControler::on_toolButtonRotateRight_clicked(){

    int value = ui->dial->value();

    int a = value / 90 + 1;
    if(a >= 4){a = 0;}
    ui->dial->setValue(a*90);

}



