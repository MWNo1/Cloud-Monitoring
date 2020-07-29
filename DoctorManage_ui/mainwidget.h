#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();


private slots:
    void on_patientButton_clicked();

    void on_deviceButton_clicked();

    void on_realtimeButton_clicked();

    void on_historyButton_clicked();

    void on_bindButton_clicked();

    void on_bindOpButton_clicked();

    void on_pushButton_4_clicked();



    void on_addpatientButton_clicked();

    void on_removepatientButton_clicked();



    void on_selectpatientButton_clicked();

    void on_adddevicebutton_clicked();

    void on_removedevicebutton_clicked();

private:
    Ui::MainWidget *ui;
    QSqlDatabase db;
    QSqlQueryModel *patientComboBoxModel;
    QSqlQueryModel *deviceComboBoxModel;
    QSqlQueryModel *Model;
    QSqlQueryModel *Model1;
    QSqlRelationalTableModel* patientDeviceModel;
    QSqlRelationalTableModel* patientModel;
    QSqlRelationalTableModel* deviceModel;

};

#endif // MAINWIDGET_H
