# Qt_Road
Record the learning process of Qt

# Chapter04 

## smap4_13

1. QTableWidget的每个单元格都需要关联一个QTableWidgetItem对象。每个QTableWidgetItem对象，存储了单元格的文字及格式定义，QTableWidget根据每个单元格对应的QTableWidgetItem的各种属性绘制表格。

2. 通过setData()函数，可以为QTableWidgetItem对象关联用户数据。【用户数据一般不会显示在界面上】

3. QTableWidget有一个editTriggers属性，通过设置该属性可以修改表格的可编辑状态。

# Chapter05

## samp5_1

1. 原来的上移和下移代码，使用数据模型的接口函数moveRow()进行上下移动，但是无法奏效。因此进行了修改，后续可参考该代码。【可能源代码是基于Qt6的】

2. 使用view对象的setModel()函数，构建model/view结构。

3. **QListView和QStringListModel结合组成了model/view结构。**但是不能对QListView里的数据设置图标或复选框，或文字对齐方式，因为QStringListModel内部仅保存字符串列表，没有数据结构保存其他角色的数据。

## samp5_2

1. **QStandardItemModel通常与QTableView组成model/view结构**，实现二维数据的管理。每个单元格是一个项，每一个项是一个QStandardItem对象，可以存储各种角色的数据。

2. 通过设置项选择模型QItemSelectionModel，它的功能是跟踪view组件上的选择操作，可以使用函数QItemSelectionModel::selectedIndexes()返回所有被选中单元格的模型索引。

3. QStandardItemModel可以不仅可以作为表格模型，也可以作为列表模型和树状模型。【以二维数组的形式存储项，就是表格模型；如果表格模型只有一列，就是列表模型；如果在存储项时为项指定父项，就可以构成树状模型】

4. 创建选择模型时需要传递一个数据模型变量作为其参数，这样，选择模型就与数据模型关联，用于跟踪数据模型的项选择操作。

>【To-do】中文乱码问题总是无法解决。


