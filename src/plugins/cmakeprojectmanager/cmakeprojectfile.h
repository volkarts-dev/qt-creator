#pragma once

#include "utils/filepath.h"

namespace cmle {

class CMakeListsFile;
class StandardFileBuffer;

} // namespace cmle

namespace CMakeProjectManager {
namespace Internal {

class CMakeProjectFile
{
public:
    CMakeProjectFile(const QString& targetName,
                     const Utils::FilePath& basePath,
                     const Utils::FilePath& listsFilePath);
    ~CMakeProjectFile();

    bool load();
    bool save();

    bool addFiles(const Utils::FilePaths& filePaths, Utils::FilePaths* notAdded);
    bool renameFile(const Utils::FilePath& oldFilePath, const Utils::FilePath& newFilePath);
    bool removeFiles(const Utils::FilePaths& filePaths, Utils::FilePaths* notRemoved);
    bool deleteFiles(const Utils::FilePaths& filePaths);

private:
    QString makeRelative(const Utils::FilePath& filePath);

    QString targetName_;
    Utils::FilePath basePath_;
    Utils::FilePath listsFilePath_;
    std::unique_ptr<cmle::StandardFileBuffer> fileBuffer_;
    std::unique_ptr<cmle::CMakeListsFile> listsFile_;
};

} // namespace Internal
} // namespace CMakeProjectManager
