#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::createItemsARow(int rowNo, QString name, QString sex,
                                 QDate birth, QString nation, bool isPM,
                                 int score)
{
    uint studId = 202105000;    // 学号基数
    // 姓名
    QTableWidgetItem *item = new QTableWidgetItem(name, MainWindow::ctName);    // type为ctName
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    studId += rowNo;    // 学号 = 基号 + 行数
    item->setData(Qt::UserRole, QVariant(studId));   // 设置studId为用户数据
    ui->tableInfo->setItem(rowNo, MainWindow::colName, item);   // 为单元格设置item

    // 性别
    item = new QTableWidgetItem(sex, MainWindow::ctSex);
    QIcon icon;
    if(sex == "男")
        icon.addFile(":/images/icons/boy.ico");
    else
        icon.addFile(":/images/icons/girl.ico");
    item->setIcon(icon);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled; // “性别”单元格不允许编辑
    item->setFlags(flags);
    ui->tableInfo->setItem(rowNo, MainWindow::colSex, item);

    // 出生日期
    QString str = birth.toString("yyyy-MM-dd"); // 日期转化为字符串
    item = new QTableWidgetItem(str, MainWindow::ctBirth);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo, MainWindow::colBirth, item);

    // 民族
    item = new QTableWidgetItem(nation, MainWindow::ctNation);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo, MainWindow::colNation, item);

    // 是否党员
    item = new QTableWidgetItem("党员", MainWindow::ctPartyM);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    // "党员"单元格不允许编辑，但可以更改复选状态
    flags = Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    item->setFlags(flags);
    if(isPM)
        item->setCheckState(Qt::Checked);
    else
        item->setCheckState(Qt::Unchecked);
    item->setBackground(QBrush(Qt::yellow));
    ui->tableInfo->setItem(rowNo, MainWindow::colPartyM, item);

    // 分数
    str.setNum(score);
    item = new QTableWidgetItem(str, MainWindow::ctScore);  // type为ctScore
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo, MainWindow::colScore, item);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 状态栏初始化
    labCellIndex = new QLabel("当前单元格坐标: ", this);
    labCellIndex->setMinimumWidth(250);
    labCellType = new QLabel("当前单元格类型: ", this);
    labCellType->setMinimumWidth(200);
    labStudId = new QLabel("学生ID: ", this);
    labStudId->setMinimumWidth(200);

    ui->statusBar->addWidget(labCellIndex);
    ui->statusBar->addWidget(labCellType);
    ui->statusBar->addWidget(labStudId);

    // 手动关联信号与槽函数
    connect(ui->rBtnSelectItem, SIGNAL(clicked()), this, SLOT(do_setSelectWay()));
    connect(ui->rBtnSelectRow, SIGNAL(clicked()), this, SLOT(do_setSelectWay()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnSetHeader_clicked()
{
    QStringList headerText;
    headerText<<"姓名"<<"性别"<<"出生日期"<<"民族"<<"分数"<<"是否党员";
    ui->tableInfo->setColumnCount(headerText.size());   // 设置表格的列数
    for(int i = 0; i < ui->tableInfo->columnCount(); i++){
        QTableWidgetItem *headerItem = new QTableWidgetItem(headerText[i]); // 创建item
        QFont font = headerItem->font();    // 获取原有字体设置
        font.setBold(true);
        font.setPointSize(11);
        headerItem->setForeground(QBrush(Qt::red)); // 设置单元格
        headerItem->setFont(font);
        ui->tableInfo->setHorizontalHeaderItem(i, headerItem);  // 设置表头单元格的item
    }
}

void MainWindow::on_btnIniData_clicked()
{
    QDate birth(2001, 4, 6);    // 初始化日期
    ui->tableInfo->clearContents(); // 只清除工作区，不清除表头
    // 随机数生成
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(60, 99);    // 随机数范围为[60, 100)

    for(int i = 0; i < ui->tableInfo->rowCount(); i++){
        QString strName = QString("学生%1").arg(i);
        QString strSex = ((i % 2) == 0) ? "男" : "女";
        bool isParty = ((i % 2) == 0) ? false : true;
        int score = dis(gen);
        createItemsARow(i, strName, strSex, birth, "汉族", isParty, score);   // 为某一行创建items
        birth = birth.addDays(20);  // 日期加20天
    }
}

void MainWindow::on_tableInfo_currentCellChanged(int currentRow, int currentColumn,
                                                 int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);

    QTableWidgetItem* item = ui->tableInfo->item(currentRow, currentColumn);    // 获取item
    if(item == nullptr)
        return;
    labCellIndex->setText(QString::asprintf("当前单元格坐标: %d 行, %d 列",
                                            currentRow, currentColumn));
    int cellType = item->type();
    labCellType->setText(QString::asprintf("当前单元格类型: %d", cellType));

    item = ui->tableInfo->item(currentRow, MainWindow::colName);    // 当前行第一列的item
    uint ID = item->data(Qt::UserRole).toUInt();    // 读取用户数据
    labStudId->setText(QString::asprintf("学生ID: %d", ID));
}

void MainWindow::on_btnInsertRow_clicked()
{
    // 按钮 插入行
    int curRow = ui->tableInfo->currentRow();   // 当前行的行号
    ui->tableInfo->insertRow(curRow);   // 插入一行，但不会自动为单元格创建item
    createItemsARow(curRow, "新生", "男",
                    QDate::fromString("2002-10-1", "yyyy-M-d"),"苗族", true, 80);
}

void MainWindow::on_btnAppendRow_clicked()
{
    // 按钮 添加行
    int curRow = ui->tableInfo->rowCount();
    ui->tableInfo->insertRow(curRow);
    createItemsARow(curRow, "新生", "女",
                    QDate::fromString("2002-6-5", "yyyy-M-d"), "满族", false, 76);
}

void MainWindow::on_btnDelCurRow_clicked()
{
    // 按钮 删除当前行
    int curRow = ui->tableInfo->currentRow();
    ui->tableInfo->removeRow(curRow);   // 删除当前行及其items
}

void MainWindow::on_chkBoxTabEditable_clicked(bool checked)
{
    if(checked) // 允许编辑，双击或获取焦点后点击可进入编辑状态
        ui->tableInfo->setEditTriggers(QAbstractItemView::DoubleClicked
                                       | QAbstractItemView::SelectedClicked);
    else
        ui->tableInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void MainWindow::on_chkBoxHeaderV_clicked(bool checked)
{
    // 是否显示垂直表头
    ui->tableInfo->verticalHeader()->setVisible(checked);
}

void MainWindow::on_chkBoxHeaderH_clicked(bool checked)
{
    // 是否显示水平表头
    ui->tableInfo->horizontalHeader()->setVisible(checked);
}

void MainWindow::on_chkBoxRowColor_clicked(bool checked)
{
    // 间隔行底色
    ui->tableInfo->setAlternatingRowColors(checked);
}

void MainWindow::on_btnReadToEdit_clicked()
{
    // 遍历表格读取数据
    QTableWidgetItem *item;
    ui->textEdit->clear();
    for(int i = 0; i < ui->tableInfo->rowCount(); ++i){ // 逐行处理
        QString str = QString::asprintf("第 %d 行: ", i+1);
        for(int j = 0; j < ui->tableInfo->columnCount() - 1; ++j){
            // 逐列处理，最后一列单独处理
            item = ui->tableInfo->item(i, j);
            str = str + item->text() + "  ";
        }
        item = ui->tableInfo->item(i, colPartyM);
        if(item->checkState() == Qt::Checked)
            str = str + "党员";
        else
            str = str + "群众";
        ui->textEdit->appendPlainText(str);
    }
}

void MainWindow::on_btnSetRows_clicked()
{
    ui->tableInfo->setRowCount(ui->spinRowCount->value());//设置数据区行数
    ui->tableInfo->setAlternatingRowColors(ui->chkBoxRowColor->isChecked()); //设置交替行背景颜色
}

void MainWindow::on_btnAutoHeght_clicked()
{
    // 按钮 自动调节行高
    ui->tableInfo->resizeRowsToContents();
}

void MainWindow::on_btnAutoWidth_clicked()
{
    // 按钮 自动调节列长
    ui->tableInfo->resizeColumnsToContents();
}

void MainWindow::do_setSelectWay()
{

    if(ui->rBtnSelectItem->isChecked())    //选择方式：单元格选择
        ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectItems);
    else    // 行选择
        ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
}
