# Qt_Road
Record the learning process of Qt

# Chapter04 

## smap4_13

1. QTableWidget的每个单元格都需要关联一个QTableWidgetItem对象。每个QTableWidgetItem对象，存储了单元格的文字及格式定义，QTableWidget根据每个单元格对应的QTableWidgetItem的各种属性绘制表格。

2. 通过setData()函数，可以为QTableWidgetItem对象关联用户数据。【用户数据一般不会显示在界面上】

3. QTableWidget有一个editTriggers属性，通过设置该属性可以修改表格的可编辑状态。
