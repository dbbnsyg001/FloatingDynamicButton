#include "FloatingDynamicButton.h"
#include "FloatingInsideButton.h"
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#pragma execution_character_set("utf-8")

struct FloatingDynamicButton_private
{
    QTimer* timer = nullptr;            //定时器
    int     interval;                   //定时器间隔
    FloatingInsideButton* button = nullptr;
    int     radius;                     //悬浮按钮半径宽度
    bool    isRefreshing;               //当前是否处于绘制光晕阴影状态
    int     shadowWidth;                //光晕最大宽度
    int     curShadowWidth;             //光晕当前宽度
    bool    shadowIsExpanging;          //当前是否处于增加光晕的模式
    QColor  shadowColor;                //光晕颜色
    int     shadowColorAlpha;           //光晕初始透明度（0-255）
    QColor  centerShadowColor;          //边框阴影颜色
    int     centerShadowWidth;          //边框阴影宽度

    FloatingDynamicButton_private()
    {
        interval = 120;
        timer = new QTimer();
        radius = 40;
        shadowWidth = 10;
        curShadowWidth = 0;
        shadowIsExpanging = true;
        shadowColor = QColor(250, 128, 114);
        shadowColorAlpha =  100;
        isRefreshing = false;
        centerShadowColor = QColor(0, 0, 0, 70);
        centerShadowWidth = 2;
    }
    ~FloatingDynamicButton_private()
    {
        if (timer)
        {
            timer->stop();
            delete timer;
            timer = nullptr;
        }

        if (button)
        {
            delete button;
            button = nullptr;
        }
    }
};


FloatingDynamicButton::FloatingDynamicButton(QWidget* parent) : BaseDragLimitWidget(parent)
{
    m_private = new FloatingDynamicButton_private;
    m_private->button = new FloatingInsideButton(this);
    m_private->button->installEventFilter(this);
    SetRadius(m_private->radius);
    //定时器,防止界面自己刷新造成呼吸不固定
    connect(m_private->timer, &QTimer::timeout, this, &FloatingDynamicButton::slotBeginDraw);
    //开启动画
    this->SetBegin(true);
}

FloatingDynamicButton::~FloatingDynamicButton()
{
    delete m_private;
    m_private = nullptr;
}

void FloatingDynamicButton::SetBegin(bool isBegin)
{
    if (isBegin)
    {
        m_private->timer->start(m_private->interval);
    }
    else
    {
        m_private->timer->stop();
    }
}

void FloatingDynamicButton::slotBeginDraw()
{
    m_private->isRefreshing = true;
    update();
}


void FloatingDynamicButton::SetText(const QString& text)
{
    m_private->button->SetText(text);
}

void FloatingDynamicButton::SetImage(const QString& image, const QString&  tip, bool hover)
{
    m_private->button->SetImage(image, tip, hover);
}

void FloatingDynamicButton::SetRadius(int radius)
{
    m_private->radius = radius;
    //重新设置大小
    int diameterRect = (radius + m_private->shadowWidth) * 2;
    this->setFixedSize(diameterRect, diameterRect);
    int diameter = sqrt((radius * radius) * 2);
    m_private->button->setFixedSize(diameter, diameter);
    //中心点同步
    m_private->button->move(diameterRect / 2 - diameter / 2, diameterRect / 2 - diameter / 2);
}

void FloatingDynamicButton::paintEvent(QPaintEvent* event)
{
    //画光晕
    if (m_private->isRefreshing)
    {
        //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
        DrawShadowCircle(false);
        m_private->isRefreshing = false;
    }
    else
    {
        DrawShadowCircle(true);
    }

    //画阴影
    DrawCenterCircleShadow();
    //画中心的按钮
    DrawCenterCircleFill();
    QWidget::paintEvent(event);
}

bool FloatingDynamicButton::eventFilter(QObject* watched, QEvent* event)
{
    if (QEvent::MouseButtonPress == event->type() && watched == m_private->button)
    {
        //传递按下事件到该类
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);

        if (mouseEvent && mouseEvent->button() == Qt::LeftButton)
        {
            QWidget::mousePressEvent(mouseEvent);
            return true;
        }
    }

    if (QEvent::MouseButtonRelease == event->type() && watched == m_private->button)
    {
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
        //如果移动过不算点击
        bool isMoved = this->JudgeLastPressedMoved();
        m_private->button->SetToolTipVisable(true);

        if (mouseEvent && mouseEvent->button() == Qt::LeftButton && isMoved == false)
        {
            emit widgetClicked();
            QWidget::mouseReleaseEvent(mouseEvent);
            return true;
        }
    }

    if (QEvent::MouseMove == event->type() && watched == m_private->button)
    {
        m_private->button->SetToolTipVisable(false);
    }

    return false;
}

void FloatingDynamicButton::DrawCenterCircleShadow()
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.translate(this->width() / 2, this->height() / 2);
    painter.setBrush(QBrush(m_private->centerShadowColor));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    int radius = m_private->radius + m_private->centerShadowWidth;
    painter.drawEllipse(-radius, -radius, radius * 2, radius * 2);
}

void FloatingDynamicButton::DrawCenterCircleFill()
{
    //计算当前绘制外圈光晕的半径
    int radius = m_private->radius;
    m_private->shadowColor.setAlpha(255);
    //绘制光晕
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(this->width() / 2, this->height() / 2);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_private->shadowColor);
    painter.drawEllipse(-radius, -radius, radius * 2, radius * 2);
}

void FloatingDynamicButton::DrawShadowCircle(bool isStatic)
{
    //计算外圈光晕的半径
    int radius = m_private->radius;

    if (m_private->curShadowWidth >= m_private->shadowWidth)
    {
        m_private->shadowIsExpanging = false;
    }
    else if (m_private->curShadowWidth < 3)
    {
        m_private->shadowIsExpanging = true;
    }

    if (isStatic)
    {
        m_private->shadowIsExpanging == true ? m_private->curShadowWidth : m_private->curShadowWidth;
    }
    else
    {
        m_private->shadowIsExpanging == true ? m_private->curShadowWidth++ : m_private->curShadowWidth--;
    }

    radius += m_private->curShadowWidth;
    //光晕透明度动态
    float shadowPercent = m_private->curShadowWidth * 1.0 / m_private->shadowWidth;
    int shadowAlpha = m_private->shadowColorAlpha * shadowPercent;
    shadowAlpha = shadowAlpha >= 0 ? shadowAlpha : 0;
    m_private->shadowColor.setAlpha(shadowAlpha);
    //绘制光晕
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(this->width() / 2, this->height() / 2);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_private->shadowColor);
    painter.drawEllipse(-radius, -radius, radius * 2, radius * 2);
}
