#include "index.h"
#include "ui_index.h"

Index::Index(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Index)
{
    ui->setupUi(this);
}

Index::~Index()
{
    delete ui;
}

Index &Index::getInstance()
{
    static Index instance;
    return instance;
}

Friend *Index::getFriend()
{
    return ui->friendPage;//把friend界面传出
}

File *Index::getFile()
{
    return ui->filePage;
}

void Index::on_friend_Pb_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);//切换当前的界面,0为第一个
}

void Index::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
