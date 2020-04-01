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

#include "vkbinputlayoutitem.h"
#include "vkbinputlayoutattached.h"
#include "vkbinputdelegate.h"
#include "vkbinputpopup.h"

#include <QtQml/qqmlcomponent.h>
#include <QtQml/qqmlcontext.h>
#include <QtQml/qqmlengine.h>
#include <QtQuickTemplates2/private/qquickabstractbutton_p.h>

VkbInputLayoutItem::VkbInputLayoutItem(QQuickItem *parent) : QQuickItem(parent)
{
}

qreal VkbInputLayoutItem::spacing() const
{
    return m_spacing;
}

void VkbInputLayoutItem::setSpacing(qreal spacing)
{
    if (qFuzzyCompare(m_spacing, spacing))
        return;

    m_spacing = spacing;
    polish();
    emit spacingChanged();
}

VkbInputLayout VkbInputLayoutItem::layout() const
{
    return m_layout;
}

void VkbInputLayoutItem::setLayout(const VkbInputLayout &layout)
{
    if (m_layout == layout)
        return;

    QHash<VkbInputKey, QQuickAbstractButton *> oldButtons = m_buttons;
    m_buttons.clear();

    for (int r = 0; r < layout.rowCount(); ++r) {
        const QVector<VkbInputKey> row = layout.rowAt(r);
        for (int c = 0; c < row.count(); ++c) {
            const VkbInputKey &key = row.at(c);
            QQuickAbstractButton *button = oldButtons.take(key);
            if (!button) {
                button = createButton(key, this);
                connect(button, &QQuickAbstractButton::clicked, this, &VkbInputLayoutItem::handleKeyClick);
                if (!key.alt.isEmpty())
                    connect(button, &QQuickAbstractButton::pressAndHold, this, &VkbInputLayoutItem::handleKeyPressAndHold);
            }
            m_buttons.insert(key, button);
        }
    }

    for (QQuickAbstractButton *button : qAsConst(oldButtons))
        button->deleteLater();

    m_layout = layout;
    polish();
    emit layoutChanged();
}

QQmlListProperty<VkbInputDelegate> VkbInputLayoutItem::delegates()
{
    return QQmlListProperty<VkbInputDelegate>(this, nullptr, delegates_append, delegates_count, delegates_at, delegates_clear);
}

void VkbInputLayoutItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    if (newGeometry.size() != oldGeometry.size())
        polish();
}

void VkbInputLayoutItem::updatePolish()
{
    QQuickItem::updatePolish();

    const qreal cw = width();
    const qreal ch = height();
    const qreal sp = spacing();
    const int rc = m_layout.rowCount();
    const int mc = m_layout.columnCount();
    const qreal kh = (ch - (rc - 1) * sp) / rc;
    const qreal kw = std::min((cw - (mc - 1) * sp) / mc, kh * 1.5);

    qreal y = 0;
    for (int r = 0; r < rc; ++r) {
        qreal x = 0;
        QList<QQuickAbstractButton *> buttons;
        const QVector<VkbInputKey> row = m_layout.rowAt(r);
        const int cc = row.count();
        for (int c = 0; c < cc; ++c) {
            const VkbInputKey &key = row.at(c);
            QQuickAbstractButton *button = m_buttons.value(key);
            if (!button)
                continue;
            const qreal w = kw * key.span + sp * (key.span - 1.0);
            button->setPosition(QPointF(x, y));
            button->setSize(QSize(w, kh));
            x += w + sp;
            buttons += button;
        }

        const qreal rw = x - sp;
        const qreal dx = (cw - rw) / 2.0;
        for (QQuickItem *item : qAsConst(buttons))
            item->setX(item->x() + dx);

        y += kh + sp;
    }
}

void VkbInputLayoutItem::handleKeyClick()
{
    VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(sender());
    if (!attached)
        return;

    emit keyClicked(attached->inputKey());
}

void VkbInputLayoutItem::handleKeyPressAndHold()
{
    QQuickAbstractButton *button = qobject_cast<QQuickAbstractButton *>(sender());
    VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(button);
    if (!attached)
        return;

    VkbInputPopup *popup = createPopup(attached->inputKey(), button);
    if (!popup)
        return;

    popup->open();
    connect(popup, &QQuickPopup::closed, popup, &QObject::deleteLater);
    connect(popup, &VkbInputPopup::keySelected, this, &VkbInputLayoutItem::keyClicked);
    connect(button, &QQuickAbstractButton::released, popup, &QQuickPopup::close);

    emit keyPressAndHold(attached->inputKey());
}

VkbInputDelegate *VkbInputLayoutItem::findDelegate(Qt::Key key) const
{
    auto it = std::find_if(m_delegates.cbegin(), m_delegates.cend(), [&key](VkbInputDelegate *delegate) { return delegate->key() == key; });
    if (it != m_delegates.cend())
        return *it;
    if (key != Qt::Key_unknown)
        return findDelegate(Qt::Key_unknown);
    return nullptr;
}

template <typename T>
static T *beginCreate(const VkbInputKey &key, QQmlComponent *component, QObject *parent)
{
    if (!component)
        return nullptr;

    QQmlContext *creationContext = component->creationContext();
    if (!creationContext)
        creationContext = qmlContext(parent);
    QQmlContext *context = new QQmlContext(creationContext, parent);

    QObject *instance = component->beginCreate(context);
    T *object = qobject_cast<T *>(instance);
    if (!object) {
        delete instance;
        return nullptr;
    }

    VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(instance);
    if (attached)
        attached->setInputKey(key);

    return object;
}

QQuickAbstractButton *VkbInputLayoutItem::createButton(const VkbInputKey &key, QQuickItem *parent) const
{
    VkbInputDelegate *delegate = findDelegate(key.key);
    if (!delegate)
        return nullptr;

    QQmlComponent *component = delegate->button();
    QQuickAbstractButton *button = beginCreate<QQuickAbstractButton>(key, component, parent);
    if (!button) {
        qWarning() << "VkbInputLayoutItem::createButton: button delegate for" << key.key << "is not a Button";
        return nullptr;
    }

    button->setParentItem(parent);
    button->setFocusPolicy(Qt::NoFocus);
    button->setAutoRepeat(key.autoRepeat);
    button->setCheckable(key.checkable);
    if (key.checked)
        button->setChecked(true);
    component->completeCreate();
    return button;
}

VkbInputPopup *VkbInputLayoutItem::createPopup(const VkbInputKey &key, QQuickAbstractButton *button) const
{
    VkbInputDelegate *delegate = findDelegate(key.key);
    if (!delegate)
        return nullptr;

    QQmlComponent *component = delegate->popup();
    VkbInputPopup *popup = beginCreate<VkbInputPopup>(key, component, button);
    if (!popup) {
        qWarning() << "VkbInputLayout::createPopup: popup delegate for" << key.key << "is not an InputPopup";
        return nullptr;
    }

    popup->setParentItem(button);
    component->completeCreate();
    return popup;
}

void VkbInputLayoutItem::delegates_append(QQmlListProperty<VkbInputDelegate> *property, VkbInputDelegate *delegate)
{
    VkbInputLayoutItem *that = static_cast<VkbInputLayoutItem *>(property->object);
    that->m_delegates.append(delegate);
    that->polish();
}

int VkbInputLayoutItem::delegates_count(QQmlListProperty<VkbInputDelegate> *property)
{
    VkbInputLayoutItem *that = static_cast<VkbInputLayoutItem *>(property->object);
    return that->m_delegates.count();
}

VkbInputDelegate *VkbInputLayoutItem::delegates_at(QQmlListProperty<VkbInputDelegate> *property, int index)
{
    VkbInputLayoutItem *that = static_cast<VkbInputLayoutItem *>(property->object);
    return that->m_delegates.value(index);
}

void VkbInputLayoutItem::delegates_clear(QQmlListProperty<VkbInputDelegate> *property)
{
    VkbInputLayoutItem *that = static_cast<VkbInputLayoutItem *>(property->object);
    that->m_delegates.clear();
    that->polish();
}
