#pragma once
#include <QPushButton>
#include <QEvent>

//自定义button
class FloatingInsideButton : public QPushButton
{
    Q_OBJECT
public:
    FloatingInsideButton(QWidget* parent = Q_NULLPTR);
    ~FloatingInsideButton();
public:
    /// <summary>
    /// 设置文字
    /// </summary>
    /// <param name="text">文字</param>
    void SetText(const QString& text);

    /// <summary>
    /// 设置图片
    /// </summary>
    /// <param name="image">图片资源</param>
    /// <param name="bHover">是否需要移动上去才显示</param>
    void SetImage(const QString& image, const QString& tip, bool hover);

    /// <summary>
    /// 设置按钮的tip是否可见
    /// </summary>
    /// <param name="isVisible"></param>
    void SetToolTipVisable(bool isVisible);
protected:
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
private:
    bool   m_imageShowInHover = true;
    QString m_text;
    QString m_tip;
};
