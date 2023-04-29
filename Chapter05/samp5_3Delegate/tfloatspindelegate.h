#ifndef TFLOATSPINDELEGATE_H
#define TFLOATSPINDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QDoubleSpinBox>

class TFloatSpinDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TFloatSpinDelegate(QObject *parent = nullptr);

    // 需要重写如下4个虚函数
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

};

#endif // TFLOATSPINDELEGATE_H
