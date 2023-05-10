#include "tdialogsize.h"
#include "ui_tdialogsize.h"
#include <QMessageBox>

TDialogSize::TDialogSize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialogSize)
{
    ui->setupUi(this);
}

TDialogSize::~TDialogSize()
{
    QMessageBox::information(this, "提示", "TDialog对话框被删除");
    delete ui;
}

int TDialogSize::rowCount()
{
    //  用于主窗口调用获得行数的输入值
    return ui
}

int TDialogSize::columnCount()
{
    // 用于主窗口调用获得列数的输入值
}

void TDialogSize::setRowColumn(int row, int column)
{

}
