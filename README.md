# Qt_Road
Record the learning process of Qt

# Chapter04 

## samp4_9

1. QComboBox使用Model/View结构存储和显示下拉列表的数据，下拉列表的数据实际上存储在QStandardItemModel模型里，并通过QListView的子类组件显示。

2. ComboBox下拉列表的每个项（item）还可以使用函数setItemData()为一个项设置多个QVariant类型的用户数据，用户数据并不显示在界面上。

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

## samp5_3

1. QStyledItemDelegate是视图组件使用的默认的代理类，自定义代理类需要从QStyledItemDelegate类继承，并实现该类中定义的4个虚函数。

2. 实例化自定义代理对象后，利用视图组件的setItemDelegateForColumn()函数，为不同的列设置自定义代理。

# Chapter06

## samp6_1

1. **自生事件**自动进入系统队列，函数`postEvent(QObject *receiver,, QEvent *event, int priority = Qt::NormalEventPriority)`产生**发布事件**进入Qt事件列队，自生事件与发布事件的处理是**异步**的。而函数`sendEvent(QObject *receiver,, QEvent *event)`产生**发送事件**，该函数以**同步**模式运行，即需要等待对象处理完事件后才退出。

2. 应用程序的事件循环只处理自生事件和发布事件，而不会处理发送事件，因为发送事件由应用程序直接派发给某个对象，以同步模式运行。

3. 如果从QWidget或其派生类继承自定义了一个类，需要对某种类型的事件进行处理，**首先查找是否在QWidget中已有与事件类型对应的事件处理函数**，对其进行重写即可。**如果需要处理的事件在QWidget中没有定义对应的事件处理函数**，就需要重写函数event()，判断事件类型后调用给自定义的事件处理函数。

4. 按下上，下，左，右方向键时不会产生QEvent::KeyPress类型的事件，只会在按键释放时产生QEvent::KeyRelease类型的事件。

## samp6_2

1. **使用创建C++类向导自动生成的类，其构造函数没有任何参数，这样会有问题，因为界面组件必须有一个父容器组件。**

    ```c++
    TMyLabel(QWidget *parent = nullptr);    // 构造函数需要按此参数改写
    ```

2. 在自定义类里重写event()，对特定事件类型进行处理后，还需要调用父类的event()去处理其他典型事件。

3. 使用提升法提升组件的类后，提升成的类里新定义的属性，信号等不会在Qt Creator环境里显示出来。

4. 一个界面组件如果要对事件进行处理，需要从父类继承定义一个新类，在新类里重写event()函数，对特定事件类型进行判断处理，或者将事件转换为信号。

## samp6_3

1. 事件过滤器是处理事件的一种方法，可以将一个对象的事件委托给另一个对象来监视并处理，这样，就不需要为了处理某种事件而新定义一个类。

# Chapter07

## samp7_1

1. QProgressDialog有一个canceled()信号，点击进度对话框上的“取消”按钮时会触发这个信号。

## samp7_2

1. 属性modal表示使用函数QWidget::show()显示对话框的时候，对话框是否以模态方式显示。【modal为true，表示用户只能在对话框上进行操作，只有关闭对话框后，才能返回程序窗口进行操作。modal为false，表示用户可以同时在对话框和程序窗口上操作】

2. 创建一个对话框后，有3个函数用于显示对话框：

    - `QWidget::show()`：一般用于以非模态方式显示对话框，这种对话框可以与主窗口进行交互操作。没有返回值。

    - `QDialog::exec()`：以模态方式显示对话框，并且返回值为对话框操作结果。

    - `QDialog::open()`：以模态方式显示对话框，但是之后会立即退出，因此无法获取对话框操作结果。

3. “确定”按钮的代码里一般最后要调用函数accept()，这样函数exec()的返回值就是QDialog::Accepted。而“取消”按钮的代码里一般最后要调用函数reject()，这样函数exec()的返回值就是QDialog::Rejected。

4. 默认设置下，关闭对话框时对话框只是隐藏，而不会从内存中删除。可以通过QWidget的setAttribute(Qt::WA_DeleteOnClose)函数将对话框设置为关闭时自动删除。

## samp7_3

1. 要自定义窗口外观，需要通过函数setWindowFlag()，设置Qt::CustomizeWindowHint特性。

2. 在Qt5中没有setWindowFlag()函数，与之起相同作用的函数是setWindowFlags()。

3. 对于嵌入式的TFormTable窗口，它的工具按钮的statusTip信息显示在主窗口的状态栏上；对于独立的TFormTable窗口，它的工具按钮的statusTip信息显示在自己的状态栏上。

# Chapter09

## samp9_1

1. 要在Qt项目中使用数据库编程功能，需要在.pro文件中添加语句 `QT += sql`。如果要在头文件或源文件中用到Qt SQL模块中的类，需要包含`QtSQL`模块。

2. QSqlDatabase类用于建立与数据库的连接，一个QSqlDatabase对象就表示一个数据库连接。其他操作数据库的对象都需要用到数据库连接。

3. QSqlDatabase类的功能主要分为3部分：利用静态函数QSalDatabase::addDatabase()创建数据库连接，利用静态函数QSalDatabase::open()打开数据库【需要提前设置好数据库参数，如用户名和密码】，数据库信息的获取和事务操作。

4. QDataWidgetMapper类对象要设置一个QSqlTableModel模型，然后将数据表的某个字段与界面上的某个组件建立映射，界面组件就可以自动显示这个字段的数据，成为数据感知组件。

5. QSqlTableModel::setEditStrategy(QSqlTableModel::EditStrategy strategy)通过传入参数设置数据保存策略。当传入参数是QSqlTableModel::OnManualSubmit时，表示暂时缓存修改，待调用函数submitAll()后，保存所有修改到数据库中。

6. 在使用setTable()设置数据表之后，还需要运行函数select()才能将数据刷新到模型中。

7. QSqlRecord类记录了数据表的字段信息和一条记录的数据内容。

8. QDataWidgetMapper没有选择模型，所以在数据表格上点击单元格，使数据模型的当前记录发生变化时，dataMapper的当前行并不会自动变化，需要使用如下语句进行手动更新dataMapper的当前行。**这样可以使窗口上的数据感知组件刷新并显示当前记录的内容**：

    ```c++
    dataMapper->setCurrentIndex(current.row()); // 更新数据映射的行号
    ```

## samp9_2

1. QSqlQueryModel可以查询数据并作为数据模型，但是它获得的数据集是只读的，即使在界面上修改了QSqlQueryModel模型的数据，也不能将所做的修改提交到数据库。

2. 通过toFirst()，toLast()等函数去移动QDataWidgetMapper对象的当前行时，数据模型的当前记录不会自动变化，需要根据根据QDataWidgetMapper对象的当前行设置选择模型的当前行，这样才能使QTableView组件和数据感知组件的当前行是同步的，代码如下：

    ```c++
    void MainWindow::refreshTableView()
    {
        // 刷新tableView的当前行
        int index = dataMapper->currentIndex(); // dataMapper的当前行号
        QModelIndex curIndex = qryModel->index(index, 1);   // 为当前行创建模型索引
        selModel->clearSelection();
        selModel->setCurrentIndex(curIndex, QItemSelectionModel::Select);   // 设置当前行
    }
    ```

## samp9_3

1. 为了动态生成SQL语句，可以使用函数prepare()设置带有参数的SQL语句，然后用函数bindValue()为语句中的参数绑定数值，再用exec()运行SQL语句，具体代码如下：

    ```c++
    QSqlQuery query;    // 根据EmpNO查询Memo和Photo字段的数据
    query.prepare("SELECT EmpNo, Memo, Photo FROM employee WHERE EmpNo = :ID");
    query.bindValue(":ID", empNo);
    query.exec();
    ```

2. 若exec()不带任何参数执行，则运行有prepare()和bindValue()设置的SQL语句；若设置的参数是一条SQL语句，则直接运行这条SQL语句。

3. 使用QSqlQuery对象更新数据库后，需要重新设置QSqlQueryModel的SQL语句，并查询数据，以更新数据集和tableView的显示内容。
