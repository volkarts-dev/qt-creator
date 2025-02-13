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

#include "qprocessuniqueptr.h"

#include <utils/id.h>

#include <QString>
#include <QProcessEnvironment>

#include <designersettings.h>

namespace ProjectExplorer {
class Target;
} // namespace ProjectExplorer

namespace QmlDesigner {

class PuppetBuildProgressDialog;
class Model;

class PuppetCreator
{
public:
    enum PuppetType { FallbackPuppet, UserSpacePuppet, BinPathPuppet };

    PuppetCreator(ProjectExplorer::Target *target, const Model *model);

    void createQml2PuppetExecutableIfMissing();

    QProcessUniquePointer createPuppetProcess(
        const QString &puppetMode,
        const QString &socketToken,
        std::function<void()> processOutputCallback,
        std::function<void(int, QProcess::ExitStatus)> processFinishCallback,
        const QStringList &customOptions = {}) const;

    void setQrcMappingString(const QString qrcMapping);

    static QString defaultPuppetToplevelBuildDirectory();
    static QString defaultPuppetFallbackDirectory();
    static QString qmlPuppetFallbackDirectory(const DesignerSettings &settings);
protected:
    bool build(const QString &qmlPuppetProjectFilePath) const;


    QString qmlPuppetToplevelBuildDirectory() const;
    QString qmlPuppetDirectory(PuppetType puppetPathType) const;
    QString qml2PuppetPath(PuppetType puppetType) const;

    bool startBuildProcess(const QString &buildDirectoryPath,
                           const QString &command,
                           const QStringList &processArguments = QStringList(),
                           PuppetBuildProgressDialog *progressDialog = nullptr) const;
    static QString puppetSourceDirectoryPath();
    static QString qml2PuppetProjectFile();

    bool checkPuppetIsReady(const QString &puppetPath) const;
    bool qtIsSupported() const;
    QProcessUniquePointer puppetProcess(const QString &puppetPath,
                                        const QString &workingDirectory,
                                        const QString &puppetMode,
                                        const QString &socketToken,
                                        std::function<void()> processOutputCallback,
                                        std::function<void(int, QProcess::ExitStatus)> processFinishCallback,
                                        const QStringList &customOptions) const;

    QProcessEnvironment processEnvironment() const;

    QString buildCommand() const;
    QString qmakeCommand() const;

    QByteArray qtHash() const;
    QDateTime qtLastModified() const;
    QDateTime puppetSourceLastModified() const;

    bool useOnlyFallbackPuppet() const;
    QString getStyleConfigFileName() const;
    bool usesVirtualKeyboard() const;

private:
    mutable QString m_compileLog;
    ProjectExplorer::Target *m_target = nullptr;
    PuppetType m_availablePuppetType;
    static QHash<Utils::Id, PuppetType> m_qml2PuppetForKitPuppetHash;
    const Model *m_model = nullptr;
    QString m_qrcMapping;
};

} // namespace QmlDesigner
