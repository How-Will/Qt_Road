#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>

void MainWindow::openTable()
{
    // 1. 创建数据模型，打开数据表
    tabModel = new QSqlTableModel(this, DB);    // 数据模型
    tabModel->setTable("employee");     // 设置数据表
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);  // 数据保存方式
    tabModel->setSort(tabModel->fieldIndex("empNo"), Qt::AscendingOrder);   // 排序
    if (!(tabModel->select()))	//查询数据失败
    {
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n" + tabModel->lastError().text());
        return;
    }
    showRecordCount();      //显示记录条数

    // 2. 设置字段显示标题
    tabModel->setHeaderData(tabModel->fieldIndex("empNo"), Qt::Horizontal, "工号");
    tabModel->setHeaderData(tabModel->fieldIndex("Name"), Qt::Horizontal, "姓名");
    tabModel->setHeaderData(tabModel->fieldIndex("Gender"), Qt::Horizontal, "性别");
    tabModel->setHeaderData(tabModel->fieldIndex("Birthday"), Qt::Horizontal, "出生日期");
    tabModel->setHeaderData(tabModel->fieldIndex("Province"), Qt::Horizontal, "省份");
    tabModel->setHeaderData(tabModel->fieldIndex("Department"), Qt::Horizontal, "部门");
    tabModel->setHeaderData(tabModel->fieldIndex("Salary"), Qt::Horizontal, "工资");
    // 这两个字段不在tableView中显示
    tabModel->setHeaderData(tabModel->fieldIndex("Memo"), Qt::Horizontal, "备注");
    tabModel->setHeaderData(tabModel->fieldIndex("Photo"), Qt::Horizontal, "照片");

    // 3. 创建选择模型
    selModel = new QItemSelectionModel(tabModel, this);
    // 当前行或列变化时，selModel发射currentChanged()信号
    connect(selModel, &QItemSelectionModel::currentChanged,
            this, &MainWindow::do_currentChanged);
    // 当前行或列变化时，selModel发射currentRowChanged()信号
    connect(selModel, &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::do_currentRowChanged);

    // 4. model/view结构
    ui->tableView->setModel(tabModel);  // 设置数据模型
    ui->tableView->setSelectionModel(selModel); // 设置选择模型
    ui->tableView->setColumnHidden(tabModel->fieldIndex("Memo"), true); // 隐藏列
    ui->tableView->setColumnHidden(tabModel->fieldIndex("Photo"), true);

    // 5. 为tableView中的性别和部门两个字段设置自定义代理
    QStringList strList;
    strList<<"男"<<"女";
    bool isEditable = false;
    delegateSex.setItems(strList, isEditable);
    ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("Gender"),
                                            &delegateSex);  // 设置代理

    strList.clear();
    strList<<"销售部"<<"技术部"<<"生产部"<<"行政部";
    isEditable = true;
    delegateDepart.setItems(strList, isEditable);
    ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("Department"),
                                            &delegateDepart);

    // 6. 创建界面组件与模型的字段的数据映射
    dataMapper = new QDataWidgetMapper(this);
    dataMapper->setModel(tabModel); // 设置数据模型
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    // 界面组件与模型的具体字段的映射
    dataMapper->addMapping(ui->dbSpinEmpNo, tabModel->fieldIndex("empNo"));
    dataMapper->addMapping(ui->dbEditName,tabModel->fieldIndex("Name"));
    dataMapper->addMapping(ui->dbComboSex,tabModel->fieldIndex("Gender"));
    dataMapper->addMapping(ui->dbEditBirth,tabModel->fieldIndex("Birthday"));
    dataMapper->addMapping(ui->dbComboProvince,tabModel->fieldIndex("Province"));
    dataMapper->addMapping(ui->dbComboDep,tabModel->fieldIndex("Department"));
    dataMapper->addMapping(ui->dbSpinSalary,tabModel->fieldIndex("Salary"));
    dataMapper->addMapping(ui->dbEditMemo,tabModel->fieldIndex("Memo"));
    dataMapper->toFirst();  //移动到首记录

    // 7. 获取字段名称列表，填充groupBoxSort
    getFieldNames();

    // 8. 跟新action和界面组件的使能状态
    ui->actOpenDB->setEnabled(false);   // 不能再打开数据库了
    ui->actRecAppend->setEnabled(true);
    ui->actRecInsert->setEnabled(true);
    ui->actRecDelete->setEnabled(true);
    ui->actScan->setEnabled(true);
    ui->groupBoxSort->setEnabled(true); // 排序分组框
    ui->groupBoxFilter->setEnabled(true);   // 数据过滤分组框
}

void MainWindow::getFieldNames()
{
    QSqlRecord emptyRec = tabModel->record();   // 获取空记录，只有字段名
    for(int i = 0; i < emptyRec.count(); ++i)
        ui->comboFields->addItem(emptyRec.fieldName(i));
}

void MainWindow::showRecordCount()
{//在状态栏上显示数据模型的记录条数
    ui->statusBar->showMessage(QString("记录条数: %1").arg(tabModel->rowCount()));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->splitter);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);    // 项选择
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);    // 单项选择
    ui->tableView->setAlternatingRowColors(true);

    connect(ui->radioBtnAscend, &QRadioButton::clicked, this, &MainWindow::do_setSortOrder);
    connect(ui->radioBtnDescend, &QRadioButton::clicked, this, &MainWindow::do_setSortOrder);

    connect(ui->radioBtnMan, &QRadioButton::clicked, this, &MainWindow::do_setFilter);
    connect(ui->radioBtnWoman, &QRadioButton::clicked, this, &MainWindow::do_setFilter);
    connect(ui->radioBtnBoth, &QRadioButton::clicked, this, &MainWindow::do_setFilter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    ui->actSubmit->setEnabled(tabModel->isDirty()); // 有未更新到数据库的修改时可用
    ui->actRevert->setEnabled(tabModel->isDirty());
}

void MainWindow::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    // 行切换时的状态控制
    ui->actRecDelete->setEnabled(current.isValid());
    ui->actPhoto->setEnabled(current.isValid());
    ui->actPhotoClear->setEnabled(current.isValid());

    if(!current.isValid()){ // 清除图片
        ui->dbLabPhoto->clear();
        return;
    }

    dataMapper->setCurrentIndex(current.row()); // 更新数据映射的行号
    int curRecNo = current.row();   // 获取行号
    QSqlRecord curRec = tabModel->record(curRecNo); // 获取当前记录
    if(curRec.isNull("Photo"))  // Photo字段内容为空
        ui->dbLabPhoto->clear();
    else{
        QByteArray data = curRec.value("Photo").toByteArray();
        QPixmap pic;
        pic.loadFromData(data);
        ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->size().width()));
    }
}


void MainWindow::on_actOpenDB_triggered()
{
    // 按钮 打开
    QString aFile = QFileDialog::getOpenFileName(this, "选择文件","",
                                                 "SQLite数据库(*.db3)");
    if(aFile.isEmpty())
        return;
    // 打开SQLite数据库
    DB = QSqlDatabase::addDatabase("QSQLITE");  // 添加 SQLITE 数据库驱动


    // 打开数据库前，设置数据库登录参数
    DB.setDatabaseName(aFile);
    //    DB.setHostName();
    //    DB.setUserName();
    //    DB.setPassword();

    if(!DB.open())  // 打开数据库失败
        QMessageBox::warning(this, "错误", "打开数据库失败");
    else
        openTable();
}

void MainWindow::on_actRecInsert_triggered()
{
    // 按钮 插入一条记录
    QModelIndex curIndex = ui->tableView->currentIndex();   // 当前行的模型索引
    QSqlRecord rec = tabModel->record();    // 获取一条空记录，只有字段定义
    tabModel->insertRecord(curIndex.row(), rec);    // 在当前行前面插入一条记录
    selModel->clearSelection();
    selModel->setCurrentIndex(curIndex, QItemSelectionModel::Select);   // 设置当前行
    showRecordCount();
}

void MainWindow::on_actRecAppend_triggered()
{
    // 按钮 添加一条记录
    QSqlRecord rec = tabModel->record();    // 获取一条空记录，只有字段定义
    rec.setValue(tabModel->fieldIndex("empNo"), 2000 + tabModel->rowCount());
    rec.setValue(tabModel->fieldIndex("Gender"), "男");
    tabModel->insertRecord(tabModel->rowCount(), rec);  // 插入数据模型的最后

    selModel->clearSelection();
    QModelIndex curIndex = tabModel->index(tabModel->rowCount() - 1, 1);
    selModel->setCurrentIndex(curIndex, QItemSelectionModel::Select);   // 设置当前行
    showRecordCount();
}

void MainWindow::on_actRecDelete_triggered()
{
    // 按钮 删除当前记录
    QModelIndex curIndex = selModel->currentIndex();    // 获取当前选择单元格的模型索引
    tabModel->removeRow(curIndex.row());    // 删除当前行
    showRecordCount();
}

void MainWindow::on_actSubmit_triggered()
{
    // 按钮 保存修改
    bool res = tabModel->submitAll();
    if(!res)
        QMessageBox::information(this, "消息", "数据保存错误，错误消息\n"
                                 + tabModel->lastError().text());
    else{
        ui->actSubmit->setEnabled(false);
        ui->actRevert->setEnabled(false);
    }
    showRecordCount();
}

void MainWindow::on_actRevert_triggered()
{
    // 按钮 取消修改
    tabModel->revertAll();
    ui->actSubmit->setEnabled(false);
    ui->actRevert->setEnabled(false);
    showRecordCount();
}

void MainWindow::on_actPhoto_triggered()
{
    // 按钮 设置照片
    QString aFile = QFileDialog::getOpenFileName(this, "选择图片文件", "", "照片(*.jpg)");
    if(aFile.isEmpty())
        return;
    QByteArray data;
    QFile *file = new QFile(aFile);
    file->open(QIODevice::ReadOnly);
    data = file->readAll(); // 读取图片数据为字节数据数组
    file->close();

    int curRecNo = selModel->currentIndex().row();
    QSqlRecord curRec = tabModel->record(curRecNo); // 获取当前记录
    curRec.setValue("Photo", data); // 设置字段数据
    tabModel->setRecord(curRecNo, curRec);  // 修改记录
    QPixmap pic;
    pic.load(aFile);
    ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->width()));
}

void MainWindow::on_actPhotoClear_triggered()
{
    // 按钮 清除照片
    int curRecNo = selModel->currentIndex().row();
    QSqlRecord curRec = tabModel->record(curRecNo); // 获取当前记录
    curRec.setNull("Photo");    // 设置为空值
    tabModel->setRecord(curRecNo, curRec);  // 修改记录
    ui->dbLabPhoto->clear();
}

void MainWindow::on_actScan_triggered()
{
    // 按钮 涨工资
    if(tabModel->rowCount() == 0)
        return;
    for(int i = 0; i < tabModel->rowCount(); ++i){
        QSqlRecord curRec = tabModel->record(i);
        float salary = curRec.value("Salary").toFloat();
        salary *= 1.1;
        curRec.setValue("Salary", salary);  // 更新字段数据
        tabModel->setRecord(i, curRec); // 更新记录
    }
    if(tabModel->submitAll())
        QMessageBox::information(this, "消息", "涨工资数据计算完毕");
}

void MainWindow::on_comboFields_currentIndexChanged(int index)
{
    if(ui->radioBtnAscend->isChecked())
        tabModel->setSort(index, Qt::AscendingOrder);
    else
        tabModel->setSort(index, Qt::DescendingOrder);
    tabModel->select(); // 刷新记录到模型
}

void MainWindow::do_setSortOrder()
{
    if(ui->radioBtnAscend->isChecked())
        tabModel->setSort(ui->comboFields->currentIndex(), Qt::AscendingOrder);
    else
        tabModel->setSort(ui->comboFields->currentIndex(), Qt::DescendingOrder);
    tabModel->select();
}

void MainWindow::do_setFilter()
{
    if(ui->radioBtnMan->isChecked())
        tabModel->setFilter(" Gender='男' ");
    else if(ui->radioBtnWoman->isChecked())
        tabModel->setFilter(" Gender='女' ");
    else
        tabModel->setFilter("");
    showRecordCount();
}
