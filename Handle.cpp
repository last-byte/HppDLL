#include "pch.h"
#include "hppdll.h"

namespace RAII
{
	Handle::Handle(HANDLE input)
	{
		_handle = input;
	}

	Handle::~Handle()
	{
		::CloseHandle(_handle);
	}

	HANDLE Handle::GetHandle()
	{
		return _handle;
	}
}