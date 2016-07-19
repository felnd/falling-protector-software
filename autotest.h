#ifndef AUTOTEST_H
#define AUTOTEST_H

#include <QDialog>

namespace Ui {
class AutoTest;
}

class AutoTest : public QDialog
{
    Q_OBJECT

public:
    explicit AutoTest(QWidget *parent = 0);
    ~AutoTest();

private:
    Ui::AutoTest *ui;
};

#endif // AUTOTEST_H
