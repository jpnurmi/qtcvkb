/*
 * The MIT License (MIT)
 *
 * Copyright (C) 2020 CELLINK AB <info@cellink.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "vkbinputpanel.h"

VkbInputPanel::VkbInputPanel(QObject *parent) : QQuickPopup(parent)
{
    setClosePolicy(NoAutoClose);

    connect(this, &QQuickPopup::visibleChanged, this, &VkbInputPanel::visibleChanged);
    connect(this, &QQuickPopup::openedChanged, this, &VkbInputPanel::animatingChanged);
    connect(this, &QQuickPopup::visibleChanged, this, &VkbInputPanel::animatingChanged);
    connect(this, &QQuickPopup::localeChanged, this, &VkbInputPanel::localeChanged);
}

bool VkbInputPanel::isVisible() const
{
    return QQuickPopup::isVisible();
}

void VkbInputPanel::setVisible(bool visible)
{
    QQuickPopup::setVisible(visible);
}

bool VkbInputPanel::isAnimating() const
{
    return isVisible() && !isOpened();
}

QRectF VkbInputPanel::rect() const
{
    return m_rect;
}

QLocale VkbInputPanel::locale() const
{
    return QQuickPopup::locale();
}

Qt::LayoutDirection VkbInputPanel::inputDirection() const
{
    return Qt::LeftToRight;
}

void VkbInputPanel::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickPopup::geometryChanged(newGeometry, oldGeometry);
    updateRect(newGeometry);
}

void VkbInputPanel::updateRect(const QRectF &rect)
{
    if (m_rect == rect)
        return;

    m_rect = rect;
    emit rectChanged();
}