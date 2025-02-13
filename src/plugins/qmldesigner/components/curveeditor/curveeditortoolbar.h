/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Design Tooling
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

#include <QSpinBox>
#include <QSlider>
#include <QToolBar>
#include <QValidator>
#include <QWidget>

#include "keyframe.h"

namespace QmlDesigner {

class CurveEditorModel;

class ValidatableSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    ValidatableSpinBox(std::function<bool(int)> validator, QWidget* parent=nullptr);
protected:
    QValidator::State validate(QString &text, int &pos) const override;
private:
    std::function<bool(int)> m_validator;
};


class CurveEditorToolBar : public QToolBar
{
    Q_OBJECT

signals:
    void unifyClicked();

    void interpolationClicked(Keyframe::Interpolation interpol);

    void startFrameChanged(int start);

    void endFrameChanged(int end);

    void currentFrameChanged(int current);

    void zoomChanged(double zoom);

public:
    CurveEditorToolBar(CurveEditorModel *model, QWidget* parent = nullptr);

    void setZoom(double zoom);

    void setCurrentFrame(int current, bool notify);

    void updateBoundsSilent(int start, int end);

private:
    ValidatableSpinBox *m_startSpin;
    ValidatableSpinBox *m_endSpin;
    QSpinBox *m_currentSpin;
    QSlider *m_zoomSlider;
};

} // End namespace QmlDesigner.
