#include "tformtable.h"
#include "ui_tformtable.h"

TFormTable::TFormTable(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TFormTable)
{
    ui->setupUi(this);

    m_model = new QStandardItemModel(12, 6, this);  // 数据模型
    m_selection = new QItemSelectionModel(m_model, this);   // 选择模型
    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionModel(m_selection);
}

TFormTable::~TFormTable()
{
    delete ui;
}

void TFormTable::on_actSize_triggered()
{
    // 定义表格大小的按钮
    TDialogSize *dlgTableSize = new TDialogSize(this);
    dlgTableSize->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    dlgTableSize->setRowColumn(m_model->rowCount(), m_model->columnCount());
    int ret = dlgTableSize->exec();

    if(ret == QDialog::Accepted){
        int cols = dlgTableSize->columnCount();
        m_model->setColumnCount(cols);
        int rows = dlgTableSize->rowCount();
        m_model->setRowCount(rows);
    }

    delete dlgTableSize;
}

void TFormTable::on_actSetHeader_triggered()
{
    // 设置表头按钮
    if(dlgSetHeaders == nullptr)
        dlgSetHeaders = new TDialogHeaders(this);
    if(dlgSetHeaders->headerList().count() != m_model->columnCount()){
        QStringList strList;
        for(int i = 0; i < m_model->columnCount(); i++) // 获取现有的表头标题
            strList.append(m_model->headerData(i, Qt::Horizontal,
                                               Qt::DisplayRole).toString());
        dlgSetHeaders->setHeaderList(strList);  // 用以对话框初始化显示
    }

    int ret = dlgSetHeaders->exec();
    if(ret == QDialog::Accepted){
        QStringList strList = dlgSetHeaders->headerList();
        m_model->setHorizontalHeaderLabels(strList);
    }
}
