#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化一个字符串列表的内容
    m_strList<<"北京"<<"上海"<<"天津"<<"河北"<<"山东"<<"四川"<<"重庆"<<"广东"<<"河南";

    m_model= new QStringListModel(this);    //创建数据模型
    m_model->setStringList(m_strList);      //为模型设置StringList，会导入StringList的内容

    ui->listView->setModel(m_model);        //为listView设置数据模型
    ui->chkEditable->setChecked(true);
    ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked
                                  | QAbstractItemView::SelectedClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnIniList_clicked()
{
    // 按钮 恢复列表
    m_model->setStringList(m_strList);
}

void MainWindow::on_btnListClear_clicked()
{
    // 按钮 清除列表
    m_model->removeRows(0, m_model->rowCount());
}

void MainWindow::on_btnListAppend_clicked()
{
    // 按钮 添加项
    m_model->insertRow(m_model->rowCount());    // 在末尾插入一行
    QModelIndex index = m_model->index(m_model->rowCount() - 1, 0);
    m_model->setData(index, "new item", Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
}

void MainWindow::on_btnListInsert_clicked()
{
    // 按钮 插入项
    QModelIndex index = ui->listView->currentIndex();   // 获取当前模型索引
    m_model->insertRow(index.row());
    m_model->setData(index, "inserted item", Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
}

void MainWindow::on_btnListDelete_clicked()
{
    // 按钮 删除项
    QModelIndex index = ui->listView->currentIndex();
    m_model->removeRow(index.row());
}

void MainWindow::on_btnListMoveUp_clicked()
{
    // 按钮 上移
    // 获取当前选定的索引
    QModelIndex currentIndex = ui->listView->currentIndex();

    // 获取当前选定的字符串
    QString currentText = m_model->stringList().at(currentIndex.row());

    // 计算新位置的行号
    int newRow = currentIndex.row() - 1;

    // 如果新位置有效，则交换字符串
    if (newRow >= 0)
    {
        // 从模型中删除当前字符串
        m_model->removeRows(currentIndex.row(), 1);

        // 插入当前字符串到新位置
        m_model->insertRows(newRow, 1);
        QStringList stringList = m_model->stringList();
        stringList.replace(newRow, currentText);
        m_model->setStringList(stringList);

        // 更新当前索引
        QModelIndex newIndex = m_model->index(newRow, 0);
        ui->listView->setCurrentIndex(newIndex);
    }
}

void MainWindow::on_btnListMoveDown_clicked()
{
    // 按钮 下移
    // 获取当前选定的索引
    QModelIndex currentIndex = ui->listView->currentIndex();

    // 获取当前选定的字符串
    QString currentText = m_model->stringList().at(currentIndex.row());

    // 计算新位置的行号
    int newRow = currentIndex.row() + 1;

    // 如果新位置有效，则交换字符串
    if (newRow < m_model->rowCount())
    {
        // 从模型中删除当前字符串
        m_model->removeRows(currentIndex.row(), 1);

        // 插入当前字符串到新位置
        m_model->insertRows(newRow, 1);
        QStringList stringList = m_model->stringList();
        stringList.replace(newRow, currentText);
        m_model->setStringList(stringList);

        // 更新当前索引
        QModelIndex newIndex = m_model->index(newRow, 0);
        ui->listView->setCurrentIndex(newIndex);
    }
}

void MainWindow::on_btnListSort_clicked(bool checked)
{
    // 按钮 排序
    if(checked)
        m_model->sort(0, Qt::AscendingOrder);
    else
        m_model->sort(0, Qt::DescendingOrder);
}

void MainWindow::on_chkEditable_clicked(bool checked)
{
    // 复选框 允许编辑
    if(checked)
        ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked
                                   | QAbstractItemView::SelectedClicked);
    else
        ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::on_btnTextClear_clicked()
{
    // 按钮 清空文本
    ui->plainTextEdit->clear();
}

void MainWindow::on_btnTextImport_clicked()
{
    // 按钮 显示数据模型的StringList
    QStringList tmpList = m_model->stringList();
    ui->plainTextEdit->clear();
    for(int i = 0; i < tmpList.size(); ++i)
        ui->plainTextEdit->appendPlainText(tmpList.at(i));
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    QString str1 = QString::asprintf("模型索引:row=%d, column=%d; ",
                                     index.row(), index.column());
    QVariant var = m_model->data(index, Qt::DisplayRole);   // 获取模型数据
    QString str2 = var.toString();
    ui->statusbar->showMessage(str1 + str2);
}
