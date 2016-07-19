#ifndef SUPERCODE_H
#define SUPERCODE_H

#include <QWidget>

namespace Ui {
class SuperCode;
}

class SuperCode : public QWidget
{
    Q_OBJECT

public:
    explicit SuperCode(QWidget *parent = 0);
    ~SuperCode();

private:
    Ui::SuperCode *ui;
};

#endif // SUPERCODE_H
