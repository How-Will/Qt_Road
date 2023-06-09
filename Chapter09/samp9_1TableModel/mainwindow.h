#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDataWidgetMapper>
#include <QFileDialog>
#include <QMessageBox>
#include <QItemSelectionModel>
#include "tcomboboxdelegate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QSqlDatabase DB;    // 数据库连接
    QSqlTableModel *tabModel;   // 数据模型
    QItemSelectionModel *selModel;  // 选择模型
    QDataWidgetMapper *dataMapper;  // 数据映射
    TComboBoxDelegate delegateSex;  // 自定义代理，性别
    TComboBoxDelegate delegateDepart;   // 自定义代理，部门
    void openTable();   // 打开数据表
    void getFieldNames();   // 获取字段名称，填充“排序字段”下拉列表框
    void showRecordCount(); // 在状态栏上显示记录条数
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void do_currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void do_currentRowChanged(const QModelIndex& current, const QModelIndex &previous);

    void on_actOpenDB_triggered();

    void on_actRecInsert_triggered();

    void on_actRecAppend_triggered();

    void on_actRecDelete_triggered();

    void on_actSubmit_triggered();

    void on_actRevert_triggered();

    void on_actPhoto_triggered();

    void on_actPhotoClear_triggered();

    void on_actScan_triggered();

    void on_comboFields_currentIndexChanged(int index);

    // 自定义槽函数，用于选择升降排序
    void do_setSortOrder();

    // 自定义槽函数，用于设置显示筛选条件
    void do_setFilter();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
