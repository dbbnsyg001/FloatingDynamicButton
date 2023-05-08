#pragma once
#include <QPaintEvent>
#include "BaseDragLimitWidget.h"

class FloatingDynamicButton_private;

class FloatingDynamicButton: public BaseDragLimitWidget
{
    Q_OBJECT

public:
    FloatingDynamicButton(QWidget* parent = Q_NULLPTR);
    ~FloatingDynamicButton();
    /// <summary>
    /// 开始
    /// </summary>
    void SetBegin(bool isBegin);

    /// <summary>
    /// 设置显示文字
    /// </summary>
    /// <param name="text">文字</param>
    void SetText(const QString& text);

    /// <summary>
    /// 设置图片
    /// </summary>
    /// <param name="image">图片资源</param>
    /// <param name="bHover">是否需要移动上去才显示</param>
    void SetImage(const QString& image, const QString& tip, bool hover = true);

    /// <summary>
    /// 设置半径
    /// </summary>
    void SetRadius(int radius);
signals:
    void widgetClicked();
public slots:
    /// <summary>
    /// 定时绘制动画
    /// </summary>
    void slotBeginDraw();
protected:
    void paintEvent(QPaintEvent* event);

    /// <summary>
    /// 事件过滤器，自身点击事件往下传递
    /// </summary>
    /// <param name="watched"></param>
    /// <param name="event"></param>
    /// <returns></returns>
    bool eventFilter(QObject* watched, QEvent* event);

    /// <summary>
    /// 绘制圆形填充外部的阴影
    /// </summary>
    void DrawCenterCircleShadow();

    /// <summary>
    /// 绘制圆形填充
    /// </summary>
    void DrawCenterCircleFill();

    /// <summary>
    /// 绘制阴影
    /// </summary>
    /// <param name="isStatic">防止刷新事件影响</param>
    void DrawShadowCircle(bool isStatic = false);
private:
    FloatingDynamicButton_private* m_private;
};

