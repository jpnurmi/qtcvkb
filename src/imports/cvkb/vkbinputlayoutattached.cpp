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

#include "vkbinputlayoutattached.h"

VkbInputLayoutAttached::VkbInputLayoutAttached(QObject *parent) : QObject(parent)
{
}

QString VkbInputLayoutAttached::key() const
{
    return m_key.key;
}

QStringList VkbInputLayoutAttached::alt() const
{
    return m_key.alt;
}

bool VkbInputLayoutAttached::autoRepeat() const
{
    return m_key.autoRepeat;
}

bool VkbInputLayoutAttached::isCheckable() const
{
    return m_key.checkable;
}

bool VkbInputLayoutAttached::isChecked() const
{
    return m_key.checked;
}

VkbInputKey VkbInputLayoutAttached::inputKey() const
{
    return m_key;
}

void VkbInputLayoutAttached::setInputKey(const VkbInputKey &key)
{
    m_key = key;
}

VkbInputLayoutAttached *VkbInputLayoutAttached::qmlAttachedProperties(QObject *object)
{
    return new VkbInputLayoutAttached(object);
}

VkbInputLayoutAttached *VkbInputLayoutAttached::qmlAttachedPropertiesObject(QObject *object)
{
    return qobject_cast<VkbInputLayoutAttached *>(::qmlAttachedPropertiesObject<VkbInputLayoutAttached>(object));
}
