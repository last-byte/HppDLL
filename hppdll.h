#pragma once
#define SECURITY_WIN32
#define WIN32_LEAN_AND_MEAN

// uncomment the following definitions to enable debug logging to
// c:\debug.txt
#define DEBUG_BUILD

#include <windows.h>
#include <SubAuth.h>
#include <iostream>
#include <fstream>
#include <string>
#include "detours.h"

// if this is a debug build declare the PrintDebug() function
// and define the DEBUG macro in order to call it
// else make the DEBUG macro do nothing
#ifdef DEBUG_BUILD
void PrintDebug(std::string input);
#define DEBUG(x) PrintDebug(x)
#else
#define DEBUG(x) do {} while (0)
#endif

// namespace containing RAII types to make sure handles are always closed before detaching our DLL
namespace RAII
{
	class Library
	{
	public:
		Library(std::wstring input);
		~Library();
		HMODULE GetHandle();

	private:
		HMODULE _libraryHandle;
	};

	class Handle
	{
	public:
		Handle(HANDLE input);
		~Handle();
		HANDLE GetHandle();

	private:
		HANDLE _handle;
	};
}

//functions used to install and remove the hook
bool InstallHook();
bool RemoveHook();

// define the pMsvpPasswordValidate type to point to MsvpPasswordValidate
typedef BOOLEAN(WINAPI* pMsvpPasswordValidate)(BOOLEAN, NETLOGON_LOGON_INFO_CLASS, PVOID, void*, PULONG, PUSER_SESSION_KEY, PVOID);
extern pMsvpPasswordValidate MsvpPasswordValidate;

// define our hook function with the same parameters as the hooked function
// this allows us to directly access the hooked function parameters
BOOLEAN HookMSVPPValidate
(
	BOOLEAN UasCompatibilityRequired,
	NETLOGON_LOGON_INFO_CLASS LogonLevel,
	PVOID LogonInformation,
	void* Passwords,
	PULONG UserFlags,
	PUSER_SESSION_KEY UserSessionKey,
	PVOID LmSessionKey
);