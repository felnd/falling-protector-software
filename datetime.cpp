#include "datetime.h"
#include "ui_datetime.h"

DateTime::DateTime(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DateTime)
{
    ui->setupUi(this);
}

DateTime::~DateTime()
{
    delete ui;
}

void DateTime::on_calendarWidget_selectionChanged()
{
    QDateTime nowdays(ui->calendarWidget->selectedDate());
    emit sendTime(nowdays);
    this->close();
}
