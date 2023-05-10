#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 窗口关闭时询问是否退出
    QMessageBox::StandardButton result;
    result = QMessageBox::question(this, "确定", "确定要退出本程序吗? ");
    if(result == QMessageBox::Yes)
        event->accept();    // 退出
    else
        event->ignore();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_model = new QStandardItemModel(6, 4, this);
    QStringList header;
    header<<"姓名"<<"性别"<<"学位"<<"部门";
    m_model->setHorizontalHeaderLabels(header); // 设置表头标题
    m_selection = new QItemSelectionModel(m_model); // 创建选择模型
    connect(m_selection, &QItemSelectionModel::currentChanged,
            this, &MainWindow::do_model_currentChanged);

    ui->tableView->setModel(m_model);   // 设置数据模型
    ui->tableView->setSelectionModel(m_selection);  // 设置选择模型
    setCentralWidget(ui->tableView);
    // 创建状态栏组件
    labCellPos = new QLabel("当前单元格: ", this);
    labCellPos->setMinimumWidth(180);
    labCellPos->setAlignment(Qt::AlignHCenter);
    ui->statusBar->addWidget(labCellPos);
    labCellText = new QLabel("单元格内容: ", this);
    labCellText->setMinimumWidth(200);
    ui->statusBar->addWidget(labCellText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_model_currentChanged(const QModelIndex &current,
                                         const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if(current.isValid()){
        labCellPos->setText(QString::asprintf("当前单元格: %d行，%d列", current.row(),
                                              current.column()));   // 显示模型索引的行号和列号
        QStandardItem *aItem;
        aItem = m_model->itemFromIndex(current);    // 从模型索引获得Item
        this->labCellText->setText("单元格内容: "+aItem->text());    // 显示Item的文字内容
    }
}

