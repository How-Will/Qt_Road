#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStringList>
#include <random>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    // 单元格的类型，在创建单元格时用于设置type
    enum CellType{
        ctName = 1000,
        ctSex,
        ctBirth,
        ctNation,
        ctPartyM,
        ctScore
    };
    // 各字段在表格中的列号
    enum FieldColNum{
        colName = 0,
        colSex,
        colBirth,
        colNation,
        colScore,
        colPartyM
    };
    // 放置与状态栏上的组件（无法可视化设计，只能用代码）
    QLabel *labCellIndex;
    QLabel *labCellType;
    QLabel *labStudId;
    // 为某一行创建items
    void createItemsARow(int rowNo, QString name, QString sex, QDate birth,
                         QString nation, bool isPM, int score);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSetHeader_clicked();

    void on_btnIniData_clicked();

    void on_tableInfo_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_btnInsertRow_clicked();

    void on_btnAppendRow_clicked();

    void on_btnDelCurRow_clicked();

    void on_chkBoxTabEditable_clicked(bool checked);

    void on_chkBoxHeaderV_clicked(bool checked);

    void on_chkBoxHeaderH_clicked(bool checked);

    void on_chkBoxRowColor_clicked(bool checked);

    void on_btnReadToEdit_clicked();

    void on_btnSetRows_clicked();

    void on_btnAutoHeght_clicked();

    void on_btnAutoWidth_clicked();

    // 自定义槽函数
    void do_setSelectWay();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
