/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
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

#include <qmldesignercorelib_global.h>
#include "qmlmodelnodefacade.h"
#include "qmlstate.h"
#include "qmltimeline.h"
#include "qmlchangeset.h"

#include <nodeinstance.h>

namespace QmlDesigner {

class QmlItemNode;
class QmlPropertyChanges;
class MoveManipulator;
class QmlVisualNode;

class QMLDESIGNERCORE_EXPORT QmlObjectNode : public QmlModelNodeFacade
{
    friend QmlItemNode;
    friend MoveManipulator;

public:
    QmlObjectNode();
    QmlObjectNode(const ModelNode &modelNode);

    static bool isValidQmlObjectNode(const ModelNode &modelNode);
    bool isValid() const override;

    bool hasError() const;
    QString error() const;
    bool hasNodeParent() const;
    bool hasInstanceParent() const;
    bool hasInstanceParentItem() const;
    void setParentProperty(const NodeAbstractProperty &parentProeprty);
    QmlObjectNode instanceParent() const;
    QmlItemNode instanceParentItem() const;

    QmlItemNode modelParentItem() const;

    void setId(const QString &id);
    QString id() const;
    QString validId();

    QmlModelState currentState() const;
    QmlTimeline currentTimeline() const;
    virtual void setVariantProperty(const PropertyName &name, const QVariant &value);
    virtual void setBindingProperty(const PropertyName &name, const QString &expression);
    NodeAbstractProperty nodeAbstractProperty(const PropertyName &name) const;
    NodeAbstractProperty defaultNodeAbstractProperty() const;
    NodeProperty nodeProperty(const PropertyName &name) const;
    NodeListProperty nodeListProperty(const PropertyName &name) const;

    bool instanceHasValue(const PropertyName &name) const;
    QVariant instanceValue(const PropertyName &name) const;
    TypeName instanceType(const PropertyName &name) const;

    bool hasProperty(const PropertyName &name) const;
    bool hasBindingProperty(const PropertyName &name) const;
    bool instanceHasBinding(const PropertyName &name) const;
    bool propertyAffectedByCurrentState(const PropertyName &name) const;
    QVariant modelValue(const PropertyName &name) const;
    bool isTranslatableText(const PropertyName &name) const;
    QString stripedTranslatableText(const PropertyName &name) const;
    QString expression(const PropertyName &name) const;
    bool isInBaseState() const;
    bool timelineIsActive() const;
    QmlPropertyChanges propertyChangeForCurrentState() const;

    virtual bool instanceCanReparent() const;

    bool isRootModelNode() const;

    void destroy();

    void ensureAliasExport();
    bool isAliasExported() const;

    QList<QmlModelState> allAffectingStates() const;
    QList<QmlModelStateOperation> allAffectingStatesOperations() const;

    void removeProperty(const PropertyName &name);

    void setParent(const QmlObjectNode &newParent);

    QmlItemNode toQmlItemNode() const;
    QmlVisualNode toQmlVisualNode() const;

    bool isAncestorOf(const QmlObjectNode &objectNode) const;

    bool hasDefaultPropertyName() const;
    PropertyName defaultPropertyName() const;

    static  QVariant instanceValue(const ModelNode &modelNode, const PropertyName &name);

    static QString generateTranslatableText(const QString& text);

    static QString stripedTranslatableTextFunction(const QString &text);

    static QString convertToCorrectTranslatableFunction(const QString &text);

    QString simplifiedTypeName() const;

    QStringList allStateNames() const;

    static QmlObjectNode *getQmlObjectNodeOfCorrectType(const ModelNode &modelNode);

    virtual bool isBlocked(const PropertyName &propName) const;

    friend auto qHash(const QmlObjectNode &node) { return qHash(node.modelNode()); }
    QList<QmlModelState> allDefinedStates() const;
    QList<QmlModelStateOperation> allInvalidStateOperations() const;

    QmlModelStateGroup states() const;

    QList<ModelNode> allTimelines() const;

    QList<ModelNode> getAllConnections() const;

protected:
    NodeInstance nodeInstance() const;
    QmlObjectNode nodeForInstance(const NodeInstance &instance) const;
    QmlItemNode itemForInstance(const NodeInstance &instance) const;
};

QMLDESIGNERCORE_EXPORT QList<ModelNode> toModelNodeList(const QList<QmlObjectNode> &fxObjectNodeList);
QMLDESIGNERCORE_EXPORT QList<QmlObjectNode> toQmlObjectNodeList(const QList<ModelNode> &modelNodeList);
}// QmlDesigner
