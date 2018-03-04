// InjectorWar3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <ProcessLib/Process/Process.h>
#include <CharacterLib/Character.h>
#include <InjectorLib/DllInjector/DllInjector.h>
#include <FileLib/File.h>
#include "ShareMemory/GameShareMemory.h"

#pragma comment(lib,"ProcessLib.lib")
#pragma comment(lib,"CharacterLib.lib")
#pragma comment(lib,"FileLib.lib")
#pragma comment(lib,"InjectorLib.lib")


BOOL InjectorDLLToGame(_In_ DWORD dwPid)
{
	std::wcout << L"Injectoring 'War3Cheat.dll' to 'War3.exe'" << std::endl;
	return libTools::CDllInjector::RemoteThreadInjectorDLL(dwPid, libTools::CCharacter::MakeCurrentPath(L"\\War3Cheat.dll"));
}


VOID PrintHelp()
{
	std::wcout << L"------------------------------------------------------" << std::endl;
	std::wcout << L"SetSelectedObjectInvincible" << std::endl;
	std::wcout << L"SetSelectedObjectAttackType" << std::endl;
	std::wcout << L"SetSelectedHeroSkillCool" << std::endl;
	std::wcout << L"PrintItem" << std::endl;
	std::wcout << L"ChangeItem -Index -ItemId" << std::endl;
	std::wcout << L"------------------------------------------------------" << std::endl;
}

BOOL CreateMemoryShare()
{
	if (!CGameShareMemory::GetInstance().Create())
	{
		std::wcout << CGameShareMemory::GetInstance().GetErrorText().c_str() << std::endl;
		return FALSE;
	}
	return TRUE;
}

int main()
{
	if (!libTools::CFile::FileExist(libTools::CCharacter::MakeCurrentPath(L"\\War3Cheat.dll")))
	{
		std::wcout << L"UnExist 'War3Cheat.dll'" << std::endl;
		system("pause");
		return 0;
	}
	else if (!CreateMemoryShare())
	{
		system("pause");
		return 0;
	}


	DWORD dwPid = libTools::CProcess::FindPidByProcName(L"War3.exe");
	if (dwPid == 0)
	{
		std::wcout << L"UnExist Process 'War3.exe'" << std::endl;
		system("pause");
		return 0;
	}

	
	if (!InjectorDLLToGame(dwPid))
	{
		std::wcout << L"Injector DLL Faild! please run as administrator" << std::endl;
		system("pause");
		return 0;
	}


	PrintHelp();
	for (;;)
	{
		std::wcout << L">";


		std::wstring wsCommand;
		std::getline(std::wcin, wsCommand);
		libTools::CCharacter::MakeTextToLower(wsCommand);


		if (wsCommand == L"help")
		{
			PrintHelp();
		}
		else if (wsCommand == libTools::CCharacter::MakeTextToLower(std::wstring(L"SetSelectedObjectInvincible")))
		{
			CGameShareMemory::GetInstance().SetNormalAction(em_Action_Type::SetSelectedObjectInvincible);
		}
		else if (wsCommand == libTools::CCharacter::MakeTextToLower(std::wstring(L"SetSelectedObjectAttackType")))
		{
			CGameShareMemory::GetInstance().SetNormalAction(em_Action_Type::SetSelectedObjectAttackType);
		}
		else if (wsCommand == libTools::CCharacter::MakeTextToLower(std::wstring(L"SetSelectedHeroSkillCool")))
		{
			CGameShareMemory::GetInstance().SetNormalAction(em_Action_Type::SetSelectedHeroSkillCool);
		}
		else if (wsCommand == libTools::CCharacter::MakeTextToLower(std::wstring(L"PrintItem")))
		{
			CGameShareMemory::GetInstance().SetNormalAction(em_Action_Type::PrintItem);
		}
		else if (wsCommand.find(libTools::CCharacter::MakeTextToLower(std::wstring(L"ChangeItem"))) != -1)
		{
			libTools::CCharacter::GetRemoveRight(wsCommand, L"ChangeItem", wsCommand);
			CGameShareMemory::GetInstance().SetActionWithParam(em_Action_Type::ChangeItem, wsCommand);
		}
		else
		{
			std::wcout << L"Invalid Command" << std::endl;
			PrintHelp();
		}
	}
    return 0;
}

