#include "pch.h"
#include "hppdll.h"

bool RemoveHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)MsvpPasswordValidate, HookMSVPPValidate);
	auto error = DetourTransactionCommit();
	if (error != NO_ERROR)
	{
		DEBUG("Failed to unhook MsvpPasswordValidate");
		return false;
	}
	else
	{
		DEBUG("Hook removed!");
		return true;
	}
}