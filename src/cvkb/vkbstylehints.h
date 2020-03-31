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

#ifndef VKBSTYLEHINTS_H
#define VKBSTYLEHINTS_H

#include <QtCVkb/vkbglobal.h>
#include <QtCore/qobject.h>

class VkbStyleHintsPrivate;

class Q_CVKB_EXPORT VkbStyleHints : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int pressAndHoldInterval READ pressAndHoldInterval WRITE setPressAndHoldInterval NOTIFY pressAndHoldIntervalChanged)

public:
    explicit VkbStyleHints(QObject *parent = nullptr);
    ~VkbStyleHints();

    static VkbStyleHints *instance();

    static int pressAndHoldInterval();
    static void setPressAndHoldInterval(int pressAndHoldInterval);

signals:
    void pressAndHoldIntervalChanged(int pressAndHoldInterval);

private:
    Q_DECLARE_PRIVATE(VkbStyleHints)
};

#endif // VKBSTYLEHINTS_H