// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CFileList.h"
#include "IrrCompileConfig.h"
#include "irrArray.h"
#include "coreutil.h"

namespace irr
{
namespace io
{

static const core::string<c16> emptyFileListEntry;

CFileList::CFileList(const core::string<c16>& path, bool ignoreCase, bool ignorePaths)
 : IgnorePaths(ignorePaths), IgnoreCase(ignoreCase), Path(path)
{
	#ifdef _DEBUG
	setDebugName("CFileList");
	#endif

	Path.replace('\\', '/');
}

CFileList::~CFileList()
{
	Files.clear();
}

u32 CFileList::getFileCount() const
{
	return Files.size();
}

void CFileList::sort()
{
	Files.sort();
}

const core::string<c16>& CFileList::getFileName(u32 index) const
{
	if (index >= Files.size())
		return emptyFileListEntry;

	return Files[index].Name;
}


//! Gets the full name of a file in the list, path included, based on an index.
const core::string<c16>& CFileList::getFullFileName(u32 index) const
{
	if (index >= Files.size())
		return emptyFileListEntry;

	return Files[index].FullName;
}

//! adds a file or folder
u32 CFileList::addItem(const core::string<c16>& fullPath, u32 size, bool isDirectory, u32 id)
{
	SFileListEntry entry;
	entry.Size = size;
	entry.ID   = id;
	entry.Name = fullPath;
	entry.Name.replace('\\', '/');
	entry.IsDirectory = isDirectory;

	// remove trailing slash
	if (core::lastChar(entry.Name) == '/')
	{
		entry.IsDirectory = true;
		entry.Name[entry.Name.size()-1] = 0;
		entry.Name.validate();
	}

	if (IgnoreCase)
		entry.Name.make_lower();

	entry.FullName = entry.Name;

	core::deletePathFromFilename(entry.Name);

	if (IgnorePaths)
		entry.FullName = entry.Name;

	Files.push_back(entry);

	return Files.size() - 1;
}

bool CFileList::isDirectory(u32 index) const
{
	bool ret = false;
	if (index < Files.size())
		ret = Files[index].IsDirectory;

	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return ret;
}

//! Returns the size of a file
u32 CFileList::getFileSize(u32 index) const
{
	return index < Files.size() ? Files[index].IsDirectory : 0;
}


//! Searches for a file or folder within the list, returns the index
s32 CFileList::findFile(const core::string<c16>& filename, bool isDirectory = false) const
{
	SFileListEntry entry;
	entry.FullName = filename;
	entry.IsDirectory = isDirectory;

	// swap
	entry.FullName.replace('\\', '/');

	// remove trailing slash
	if (core::lastChar(entry.Name) == '/')
	{
		entry.IsDirectory = true;
		entry.Name[ entry.Name.size()-1] = 0;
		entry.Name.validate();
	}

	if (IgnoreCase)
		entry.FullName.make_lower();

	if (IgnorePaths)
		core::deletePathFromFilename(entry.FullName);

	return Files.binary_search(entry);
}

//! Returns the base path of the file list
const core::string<c16>& CFileList::getPath() const
{
	return Path;
}

} // end namespace irr
} // end namespace io

