#ifndef SETENVIRONMENT_H
#define SETENVIRONMENT_H
#define CURRENTNO 13

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class setEnvironment;
}

class setEnvironment : public QWidget
{
    Q_OBJECT

public:
    explicit setEnvironment(QWidget *parent = 0);
    ~setEnvironment();
    QStringList getDatas();
    bool linkdatas(QStringList *inputDatalink);
signals:
    void closed();
    void tiaosu();
    void autotest();
    void parament();

private slots:
    void on_OK_clicked();

    void on_cancel_clicked();

    void on_recovery_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::setEnvironment *ui;
    QStringList *control_datas;

private:
    void closeEvent(QCloseEvent *e);
};

#endif // SETENVIRONMENT_H
