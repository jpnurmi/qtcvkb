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

#ifndef VKBINPUTENGINE_H
#define VKBINPUTENGINE_H

#include <QtCVkb/vkbinputglobal.h>
#include <QtCore/qobject.h>
#include <QtCore/qscopedpointer.h>

class VkbInputKey;
class VkbInputLayout;
class VkbInputEnginePrivate;

class Q_CVKB_EXPORT VkbInputEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(InputMode inputMode READ inputMode NOTIFY inputModeChanged)
    Q_PROPERTY(Qt::InputMethodHints inputMethodHints READ inputMethodHints NOTIFY inputMethodHintsChanged)
    Q_PROPERTY(Qt::KeyboardModifiers keyboardModifiers READ keyboardModifiers NOTIFY keyboardModifiersChanged)

public:
    explicit VkbInputEngine(QObject *parent = nullptr);

    enum InputMode { Letters, Capitals, Symbols, Digits };
    Q_ENUM(InputMode)

    InputMode inputMode() const;
    void setInputMode(InputMode inputMode);

    Qt::InputMethodHints inputMethodHints() const;
    void setInputMethodHints(Qt::InputMethodHints inputMethodHints);

    Qt::KeyboardModifiers keyboardModifiers() const;
    void setKeyboardModifiers(Qt::KeyboardModifiers keyboardModifiers);

    VkbInputLayout layout() const;
    void setLayout(const VkbInputLayout &layout);

public slots:
    void reset();
    void commit();
    void update(Qt::InputMethodQueries queries);
    void setFocusObject(QObject *focusObject);

    void sendKeyPress(const VkbInputKey &key);
    void sendKeyRelease(const VkbInputKey &key);

signals:
    void inputModeChanged(InputMode inputMode);
    void inputMethodHintsChanged(Qt::InputMethodHints inputMethodHints);
    void keyboardModifiersChanged(Qt::KeyboardModifiers keyboardModifiers);
    void layoutChanged(const VkbInputLayout &layout);

    void keyPressed(const VkbInputKey &key);
    void keyReleased(const VkbInputKey &key);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    Q_DISABLE_COPY(VkbInputEngine)
    Q_DECLARE_PRIVATE(VkbInputEngine)
};

#endif // VKBINPUTENGINE_H
