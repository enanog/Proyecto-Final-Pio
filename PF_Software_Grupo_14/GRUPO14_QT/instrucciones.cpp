#include "instrucciones.h"
#include "ui_instrucciones.h"


Instrucciones::Instrucciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Instrucciones)
{
    ui->setupUi(this);
    QImage fondo(":/Imagenes/instrucciones.jpg");
    //this->setStyleSheet("background-image: url(:/Imagenes/fondazo.png;");
    //ui->label->setFixedSize(fondo.size());
    ui->label->setPixmap(QPixmap::fromImage(fondo));
}

Instrucciones::~Instrucciones()
{
    delete ui;
}

void Instrucciones::on_pushButton_clicked()
{
    ui->pushButton->setStyleSheet("background-color: rgba(50, 50, 50, 0); border: 2px solid white; color: white;");
    accept();
}

