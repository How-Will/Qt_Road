#ifndef TMYLABEL_H
#define TMYLABEL_H

#include <QObject>
#include <QLabel>
#include <QMouseEvent>

class TMyLabel : public QLabel
{
     Q_OBJECT
public:
    TMyLabel(QWidget *parent = nullptr);    // 构造函数需要按此参数改写
    bool event(QEvent *e);  // 重写event()函数
protected:
    void mouseDoubleClickEvent(QMouseEvent *event); // 重写鼠标双击事件的默认处理函数
signals:
    void doubleClicked();   // 自定义信号
};

#endif // TMYLABEL_H
