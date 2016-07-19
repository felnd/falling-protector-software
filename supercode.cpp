#include "supercode.h"
#include "ui_supercode.h"

SuperCode::SuperCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SuperCode)
{
    ui->setupUi(this);
}

SuperCode::~SuperCode()
{
    delete ui;
}
