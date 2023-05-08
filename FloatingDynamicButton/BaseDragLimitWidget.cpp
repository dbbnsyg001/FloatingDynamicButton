#include "BaseDragLimitWidget.h"
#include <QApplication>
#include <QDesktopWidget>

struct BaseDragLimitWidget_private
{
    int         offSetLeft = 0;                 //距离左侧偏移
    int         offSetRight = 0;                //距离右侧偏移
    int         offSetTop = 0;                  //距离顶部偏移
    int         offSetBottom = 0;               //距离底部偏移
    QWidget*    parentWidget = nullptr;         //父控件的指针
    bool        isTransparent = false;          //鼠标穿透
    QPoint      pressStartPos;                  //按下开始拖动时的位置
    QPoint      pressMovePos;                   //开始移动时的位置
    bool        isPressed = false;              //界面是否被鼠标按下
    bool        lastPressisMoved = false;       //最后一次按下是否造成了拖动
};

BaseDragLimitWidget::BaseDragLimitWidget(QWidget* parent)
    : QWidget(parent)
{
    m_private = new BaseDragLimitWidget_private;
    m_private->parentWidget = parent;
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

BaseDragLimitWidget::~BaseDragLimitWidget()
{
    delete m_private;
    m_private = nullptr;
}


bool BaseDragLimitWidget::JudgeLastPressedMoved()
{
    return m_private->lastPressisMoved;
}

void BaseDragLimitWidget::SetEventsTransparent(bool isSet)
{
    m_private->isTransparent = isSet;
}

void BaseDragLimitWidget::mousePressEvent(QMouseEvent* event)
{
    m_private->isPressed = true;
    m_private->lastPressisMoved = false;

    //穿透事件
    if (m_private->isTransparent)
    {
        TransparentParentEvent(event);
        return;
    }

    m_private->pressMovePos = event->globalPos() - this->frameGeometry().topLeft();
    //开始拖拽时点击控件的什么位置，和父类的大小偏移，就是说左侧要对左侧，其他侧同理
    m_private->pressStartPos = this->mapFromGlobal(QCursor::pos());
    m_private->offSetLeft = m_private->pressStartPos.rx();
    m_private->offSetTop = m_private->pressStartPos.ry();

    //如果设置了父类，就用父类的大小限制，如果没有，整个桌面都是可移动范围
    if (nullptr != m_private->parentWidget)
    {
        m_private->offSetRight = m_private->parentWidget->width() - (this->width());
        m_private->offSetBottom = m_private->parentWidget->height() - (this->height());
    }
    else
    {
        // 获取桌面对象
        QDesktopWidget* desktop = QApplication::desktop();
        // 获取可用桌面大小
        QRect availableGeometry = desktop->availableGeometry();
        m_private->offSetRight = availableGeometry.width() - (this->width());
        m_private->offSetBottom = availableGeometry.height() - (this->height());
    }
}

void BaseDragLimitWidget::mouseMoveEvent(QMouseEvent* event)
{
    //穿透事件
    if (m_private->isTransparent)
    {
        m_private->lastPressisMoved = true;
        TransparentParentEvent(event);
        return;
    }

    if (!m_private->isPressed)
    {
        return;
    }

    m_private->lastPressisMoved = true;
    QPoint pos = this->mapToGlobal(event->pos() - m_private->pressMovePos);
    int moveX = pos.rx();
    int moveY = pos.ry();

    if (moveX < 0)
    {
        moveX = 0;
    }

    if (moveY < 0)
    {
        moveY = 0;
    }

    if (moveX > m_private->offSetRight)
    {
        moveX = m_private->offSetRight;
    }

    if (moveY > m_private->offSetBottom)
    {
        moveY = m_private->offSetBottom;
    }

    move(QPoint(moveX, moveY));
}

void BaseDragLimitWidget::mouseReleaseEvent(QMouseEvent* event)
{
    TransparentParentEvent(event);
    m_private->isPressed = false;
}

void BaseDragLimitWidget::TransparentParentEvent(QMouseEvent* event)
{
    //没有父窗口不做事件投递
    if (nullptr == m_private->parentWidget)
    {
        return;
    }

    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    event->setLocalPos(this->mapTo(m_private->parentWidget, event->pos()));
    QApplication::postEvent(m_private->parentWidget, event);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
}
