#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QDebug>
#include <QSqlQuery>
#include <QVariant>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRelation>
#include <QSqlRelationalDelegate>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    ui->patientButton->setMinimumHeight(30);
    ui->deviceButton->setMinimumHeight(30);
    ui->bindButton->setMinimumHeight(30);
    ui->realtimeButton->setMinimumHeight(30);
    ui->historyButton->setMinimumHeight(30);


    // 1.加载数据库驱动
    db = QSqlDatabase::addDatabase("QSQLITE");
    // 2.设置数据库名
    db.setDatabaseName("C:/Users/WQM/Desktop/test.db");
    // 3.打开数据库
    if (!db.open())
    {
        qDebug() << "打开失败";
    }

    patientComboBoxModel = new QSqlQueryModel(this);
    deviceComboBoxModel = new QSqlQueryModel(this);
    Model = new QSqlQueryModel(this);
    Model1 = new QSqlQueryModel(this);

    ui->patientComboBox->setModel(patientComboBoxModel);
    ui->deviceCombox->setModel(deviceComboBoxModel);


    patientComboBoxModel->setQuery("select paitientId from patient where paitientID not in (select paitientID from patient_device);", db);
    deviceComboBoxModel->setQuery("select dev_serial from device where dev_id not in (select devId from patient_device);", db);

    //绑定列表显示


    // 绑定列表显示
    //显示绑定列表
    patientDeviceModel = new QSqlRelationalTableModel(this,db);
    patientDeviceModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    patientDeviceModel->setTable("patient_device");  // 设置模型操作的表。
    patientDeviceModel->setRelation(1, QSqlRelation("patient", "paitientId", "name"));
    patientDeviceModel->setRelation(2, QSqlRelation("Device", "dev_id", "dev_serial"));
    patientDeviceModel->select();
    ui->patient_device_tableView->setModel(patientDeviceModel);

    //显示病人列表
    patientModel = new QSqlRelationalTableModel(this,db);
    patientModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    patientModel->setTable("patient");  // 设置模型操作的表。
    patientModel->select();
    ui->patienttableView->setModel(patientModel);


    //显示设备列表
    deviceModel = new QSqlRelationalTableModel(this,db);
    deviceModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    deviceModel->setTable("Device");  // 设置模型操作的表。
    deviceModel->select();
    ui->devicetableview->setModel(deviceModel);
    // 设置代理
    ui->patient_device_tableView->setItemDelegate(new QSqlRelationalDelegate(ui->patient_device_tableView));



}

MainWidget::~MainWidget()
{
    delete ui;
}


void MainWidget::on_patientButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}

void MainWidget::on_deviceButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWidget::on_bindButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWidget::on_realtimeButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWidget::on_historyButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);

}


// 绑定
void MainWidget::on_bindOpButton_clicked()
{
    // 病人id
    int patientId = ui->patientComboBox->currentText().toInt();


    // 设备id
    int devId;
    QString serial = ui->deviceCombox->currentText();
    QSqlQuery query(db);
    query.prepare("select dev_id from device where dev_serial = :serial;");
    query.bindValue(":serial", QVariant(serial));
    if (!query.exec())
    {
        qDebug() << "绑定失败！";
        QMessageBox::warning(this, "提示", "绑定失败", QMessageBox::Ok);
        return;
    }
    if (query.next())
    {
        devId = query.value(0).toInt();
    }

    // 绑定
    query.prepare("insert into patient_device values(null, :patientId, :devId);");
    query.bindValue(":patientId", patientId);
    query.bindValue(":devId", devId);
    if (!query.exec())
    {
        qDebug() << "绑定失败";
        return ;
    }
    qDebug() << "绑定成功";

    // 刷新绑定列表模型

    // 刷新病人comboBox模型 // 刷新设备comboBox模型
    patientComboBoxModel->setQuery("select paitientId from patient where paitientID not in (select paitientID from patient_device);", db);
    deviceComboBoxModel->setQuery("select dev_serial from device where dev_id not in (select devId from patient_device);", db);


}

void MainWidget::on_pushButton_4_clicked()
{
    patientDeviceModel->submitAll();
}

//添加病人
void MainWidget::on_addpatientButton_clicked()
{
    QSqlQuery query(db);
    if (!query.exec("insert into patient values(null, 'lily', '女', 18);"))
    {
        qDebug() << "insert fail" << query.lastError().text();
        return ;
    }

    Model->setQuery("select * from patient;", db);
    ui->patienttableView->setModel(Model);
}
//删除病人
void MainWidget::on_removepatientButton_clicked()
{
    QSqlQuery query(db);
    QString content = ui->lineEdit1->text();
    QString sql;
    if (content.isEmpty())
    {
        sql = "select * from patient;";
    }
    else
    {
        sql = QString("delete from patient where name ='%1';").arg(content);
    }

    Model->setQuery(sql, db);
    Model->setQuery("select * from patient;", db);
    ui->patienttableView->setModel(Model);

}

//选择病人
void MainWidget::on_selectpatientButton_clicked()
{
    QSqlQuery query(db);
    QString content = ui->lineEdit->text();
    QString sql;
    if (content.isEmpty())
    {
        sql = "select * from patient;";
    }
    else
    {
        sql = QString("select * from patient where name='%1';").arg(content);
    }

    Model->setQuery(sql, db);
    ui->patienttableView->setModel(Model);
}

//添加设备
void MainWidget::on_adddevicebutton_clicked()
{
    QSqlQuery query(db);
    if (!query.exec("insert into Device values(null, 'dev-11', datetime(CURRENT_TIMESTAMP,'localtime'));"))
    {
        qDebug() << "insert fail" << query.lastError().text();
        return ;
    }

    Model1->setQuery("select * from Device;", db);
    ui->devicetableview->setModel(Model1);
}

//移除设备
void MainWidget::on_removedevicebutton_clicked()
{
    QSqlQuery query(db);
    QString content = ui->lineEdit2->text();
    QString sql;
    if (content.isEmpty())
    {
        sql = "select * from Device;";
    }
    else
    {
        sql = QString("delete from Device where dev_serial ='%1';").arg(content);
    }

    Model1->setQuery(sql, db);
    Model1->setQuery("select * from Device;", db);
    ui->devicetableview->setModel(Model1);
}
