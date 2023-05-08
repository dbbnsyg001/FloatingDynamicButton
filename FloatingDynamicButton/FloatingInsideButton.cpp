#include "FloatingInsideButton.h"
#include <QToolTip>

FloatingInsideButton::FloatingInsideButton(QWidget* parent) : QPushButton(parent)
{
    //扁平化
    this->setFlat(true);
}

FloatingInsideButton::~FloatingInsideButton()
{
}

void FloatingInsideButton::SetText(const QString& text)
{
    m_text = text;
    this->setText(text);
}

void FloatingInsideButton::SetImage(const QString& image, const QString& tip, bool hover)
{
    QString imageStyle = QString("QPushButton{background-image: url(%1); \
									background-position:center; \
									background-repeat:no-repeat;}").arg(image);

    if (hover)
    {
        imageStyle = QString("QPushButton::hover{background-image: url(%1); \
								background-position:center; \
								background-repeat:no-repeat;}") .arg(image);
    }

    this->setStyleSheet(imageStyle);
    m_tip = tip;
    m_imageShowInHover = hover;
    SetToolTipVisable(true);
}

void FloatingInsideButton::SetToolTipVisable(bool isVisible)
{
    if (isVisible && this->toolTip().isEmpty())
    {
        this->setToolTip(m_tip);
    }
    else if (isVisible == false && this->toolTip().size() > 0)
    {
        this->setToolTip("");
        QToolTip::showText(QPoint(0, 0), QString(), this, this->rect());
    }
}

void FloatingInsideButton::enterEvent(QEvent* event)
{
    this->setText("");
}

void FloatingInsideButton::leaveEvent(QEvent* event)
{
    this->setText(m_text);
}

