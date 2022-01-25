#include "cmakeprojectfile.h"

#include "cmakelistsedit/CMakeListsFile.h"
#include "cmakelistsedit/StandardFileBuffer.h"

namespace CMakeProjectManager {
namespace Internal {

CMakeProjectFile::CMakeProjectFile(const QString& targetName,
                                   const Utils::FilePath& basePath,
                                   const Utils::FilePath& listsFilePath) :
    targetName_(targetName),
    basePath_(basePath),
    listsFilePath_(listsFilePath)
{
}

CMakeProjectFile::~CMakeProjectFile()
{
    save();
}

bool CMakeProjectFile::load()
{
    auto fileBuffer = std::make_unique<cmle::StandardFileBuffer>(listsFilePath_.absoluteFilePath().toString());
    if (!fileBuffer->load())
        return false;

    auto listsFile = std::make_unique<cmle::CMakeListsFile>(fileBuffer.get());
    if (!listsFile->isLoaded())
        return false;

    // TODO make this configureable
    listsFile->setInsertBlockPolicy(cmle::InsertBlockPolicy::First);
    listsFile->setDefaultSectionType(cmle::SectionType::Private);
    listsFile->setSortSectionPolicy(cmle::SortSectionPolicy::Sort);
    listsFile->setBlockCreationPolicy(cmle::BlockCreationPolicy::NoCreate);

    fileBuffer_ = std::move(fileBuffer);
    listsFile_ = std::move(listsFile);
    return true;
}

bool CMakeProjectFile::save()
{
    if (fileBuffer_ && fileBuffer_->isDirty())
        return fileBuffer_->save();
    return false;
}

bool CMakeProjectFile::addFiles(const Utils::FilePaths& filePaths, Utils::FilePaths* notAdded)
{
    bool allSuccess = true;
    for (const auto& f : filePaths)
    {
        if (!listsFile_->addSourceFile(targetName_, makeRelative(f)))
        {
            allSuccess = false;
            if (notAdded)
                (*notAdded) << f;
        }
    }

    listsFile_->save();
    return allSuccess;
}

bool CMakeProjectFile::renameFile(const Utils::FilePath& oldFilePath, const Utils::FilePath& newFilePath)
{
    if (!listsFile_->renameSourceFile(targetName_, makeRelative(oldFilePath), makeRelative(newFilePath)))
        return false;

    listsFile_->save();
    return true;
}

bool CMakeProjectFile::removeFiles(const Utils::FilePaths& filePaths, Utils::FilePaths* notRemoved)
{
    bool allSuccess = true;
    for (const auto& f : filePaths)
    {
        if (!listsFile_->removeSourceFile(targetName_, makeRelative(f)))
        {
            allSuccess = false;
            if (notRemoved)
                (*notRemoved) << f;
        }
    }

    listsFile_->save();
    return allSuccess;
}

bool CMakeProjectFile::deleteFiles(const Utils::FilePaths& filePaths)
{
    return removeFiles(filePaths, nullptr);
}

QString CMakeProjectFile::makeRelative(const Utils::FilePath& filePath)
{
    return filePath.relativeChildPath(basePath_).toString();
}

} // namespace Internal
} // namespace CMakeProjectManager
