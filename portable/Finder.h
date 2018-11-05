// Finder.h
// Copyright 2018/11/04 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef Filer_h
#define Filer_h

#ifdef _WIN32

#include <unistd.h>
#include <dirent.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <string>
#pragma comment(lib, "User32.lib")

namespace portable {

class Finder
:	public DIR
{public:
	WIN32_FIND_DATA data;
	HANDLE h;
	DWORD error;
	char path[MAX_PATH];
	long tell;
	void Reset()
	{	memset(this,0,sizeof(*this));
		h = INVALID_HANDLE_VALUE;
		error = ERROR_NO_MORE_FILES;
	}
	Finder()
	{	Reset();
	}
	void SetPath(const char* path)
	{	strcpy(this->path,path);
		strcat(this->path,"\\*");
	}
	bool Open()
	{	h = FindFirstFile(path,&data);
		if(INVALID_HANDLE_VALUE == h) 
		{	return false;
		}
		return true;
	}
	bool Open(const char* path)
	{	SetPath(path);
		return Open();
	}
	bool IsDir() const
	{	if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{	return true;
		}
		return false;
	}
	long long GetFileSize() const
	{	LARGE_INTEGER filesize;
		filesize.LowPart = data.nFileSizeLow;
		filesize.HighPart = data.nFileSizeHigh;
		return filesize.QuadPart;
	}
	bool operator!() const
	{	return h == INVALID_HANDLE_VALUE || error != ERROR_NO_MORE_FILES;
	}
	bool IsEof() const
	{	return error == ERROR_NO_MORE_FILES;
	}
	bool Read()
	{	if(!*this)
		{	return false;
		}
		if(!tell)
		{	tell = 1;
			return true;
		}
		if(!FindNextFile(h,&data))
		{	error = GetLastError();
			return false;
		}
		tell++;
		return true;
	}
	bool Seek(long pos)
	{	while(Tell() < pos)
		{	if(!Read())
			{	if(Tell() == pos)
				{	return true;
				}			
				return false;
		}	}
		return true;
	}
	void Close()
	{	if(h != INVALID_HANDLE_VALUE)
		{	FindClose(h);
			h = INVALID_HANDLE_VALUE;
	}	}
	void Rewind()
	{	Close();
		Open();
	}
	long Tell() const
	{	return tell;
	}
	void Set(dirent* entry)
	{	if(!entry)
		{	return;
		}
		memset(&entry,0,sizeof(entry));	
		entry->d_type=DT_UNKNOWN;
		entry->d_name = data.cFileName;
		entry->d_namlen = (int) strlen(entry->d_name);
		if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{	entry->d_type = DT_DIR;
		}
		else
		{	entry->d_type = DT_REG;
	}	}
};

#else

#endif

}
#endif

