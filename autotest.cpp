#include "autotest.h"
#include "ui_autotest.h"
#include "qpainter.h"


AutoTest::AutoTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AutoTest)
{
    ui->setupUi(this);
    QPainter painter(ui->widget);
    painter.setBrush(QColor(12,12,12));
    painter.drawLine(65,7,650,7);
}

AutoTest::~AutoTest()
{
    delete ui;
}
