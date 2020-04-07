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

#include "vkbinputcontext_p.h"
#include "vkbinputintegration.h"
#include "vkbinputlayout.h"
#include "vkbinputpopup.h"

VkbInputPopup *VkbInputContextPrivate::createInputPopup(const VkbInputKey &key)
{
    QObject *button = inputPanel.button(key);
    if (!button)
        return nullptr;

    QObject *inputPopup = VkbInputIntegration::instance()->createInputPopup(button);
    if (inputPopup) {
        QObject::connect(inputPopup, SIGNAL(keyPressed(VkbInputKey)), &inputEngine, SLOT(handleKeyPress(VkbInputKey)));
        QObject::connect(inputPopup, SIGNAL(keyReleased(VkbInputKey)), &inputEngine, SLOT(handleKeyRelease(VkbInputKey)));
        QObject::connect(inputPopup, SIGNAL(keyCanceled(VkbInputKey)), &inputEngine, SLOT(handleKeyCancel(VkbInputKey)));
    }
    return qobject_cast<VkbInputPopup *>(inputPopup);
}

static QString resolveInputLayout(VkbInputEngine::InputMode inputMode)
{
    static const QString LayoutPath = qEnvironmentVariable("CVKB_LAYOUT_PATH", QStringLiteral(":/cvkb/layouts"));

    const QMetaEnum metaEnum = QMetaEnum::fromType<VkbInputEngine::InputMode>();
    const QString layoutName = QString::fromLatin1(metaEnum.key(inputMode)).toLower();

    return QDir(LayoutPath).filePath(layoutName + QStringLiteral(".json"));
}

bool VkbInputContextPrivate::loadInputLayout()
{
    VkbInputLayout layout;
    if (!layout.load(resolveInputLayout(inputEngine.inputMode())))
        return false;

    inputPanel.setLayout(layout);
    return true;
}

void VkbInputContextPrivate::_q_showInputPopup(const VkbInputKey &key)
{
    if (key.alt.isEmpty())
        return;

    VkbInputPopup *popup = createInputPopup(key);
    if (!popup)
        return;

    popup->setAlt(key.alt);
    popup->show();
}
