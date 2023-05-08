#pragma once
#include <QWidget>
#include <QMouseEvent>

class BaseDragLimitWidget_private;
//继承该类即可获得相对父窗体的拖拽功能,可以设置事件穿透，把拖拽传递给父窗口
class BaseDragLimitWidget : public QWidget
{
    Q_OBJECT
public:
    BaseDragLimitWidget(QWidget* parent = Q_NULLPTR);
    ~BaseDragLimitWidget();
    /// <summary>
    /// 设置鼠标穿透
    /// </summary>
    /// <param name="isTrans"></param>
    void SetEventsTransparent(bool isSet);

    /// <summary>
    /// 判断最后一次按下是否是拖拽
    /// </summary>
    /// <returns>true:超过2像素为拖拽 false:为点击</returns>
    bool JudgeLastPressedMoved();
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    /// <summary>
    /// 如果设置了鼠标穿透，发送穿透事件，该界面不响应
    /// </summary>
    /// <param name="event"></param>
    void TransparentParentEvent(QMouseEvent* event);
private:
    BaseDragLimitWidget_private* m_private = nullptr;
};

