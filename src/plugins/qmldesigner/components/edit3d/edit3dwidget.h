/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
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

#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QVector3D>
#include <QWidget>

#include <coreplugin/icontext.h>
#include "itemlibraryinfo.h"
#include <modelnode.h>

namespace QmlDesigner {

class Edit3DView;
class Edit3DCanvas;
class ToolBox;

class Edit3DWidget : public QWidget
{
    Q_OBJECT

public:
    Edit3DWidget(Edit3DView *view);

    Edit3DCanvas *canvas() const;
    Edit3DView *view() const;
    void contextHelp(const Core::IContext::HelpCallback &callback) const;

    void showCanvas(bool show);
    QMenu *visibilityTogglesMenu() const;
    void showVisibilityTogglesMenu(bool show, const QPoint &pos);

    QMenu *backgroundColorMenu() const;
    void showBackgroundColorMenu(bool show, const QPoint &pos);

    void showContextMenu(const QPoint &pos, const ModelNode &modelNode, const QVector3D &pos3d);
    void updateCreateSubMenu(const QStringList &keys,
                             const QHash<QString, QList<ItemLibraryEntry>> &entriesMap);

private slots:
    void onCreateAction();

protected:
    void dragEnterEvent(QDragEnterEvent *dragEnterEvent) override;
    void dropEvent(QDropEvent *dropEvent) override;

private:
    void linkActivated(const QString &link);
    void createContextMenu();

    QPointer<Edit3DView> m_edit3DView;
    QPointer<Edit3DView> m_view;
    QPointer<Edit3DCanvas> m_canvas;
    QPointer<QLabel> m_onboardingLabel;
    QPointer<ToolBox> m_toolBox;
    Core::IContext *m_context = nullptr;
    QPointer<QMenu> m_visibilityTogglesMenu;
    QPointer<QMenu> m_backgroundColorMenu;
    QPointer<QMenu> m_contextMenu;
    QPointer<QAction> m_editMaterialAction;
    QPointer<QAction> m_deleteAction;
    QPointer<QMenu> m_createSubMenu;
    ModelNode m_contextMenuTarget;
    QVector3D m_contextMenuPos3d;
    QHash<QString, ItemLibraryEntry> m_nameToEntry;
};

} // namespace QmlDesigner
