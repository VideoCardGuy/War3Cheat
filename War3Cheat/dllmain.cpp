// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "ShareMemory/GameShareMemory.h"
#include "Feature/CheatFeature.h"
#include <ProcessLib/Process/Process.h>
#include <LogLib/Log.h>

#pragma comment(lib,"ProcessLib.lib")
#pragma comment(lib,"LogLib.lib")

#define _SELF L"dllmain.cpp"

static HANDLE hWorkThread = NULL;
static BOOL   bRunWorkThread = FALSE;

DWORD WINAPI _WorkThread(LPVOID )
{
	CCheatFeature CheatFeautre;
	CGameShareMemory::CheatActionContent ActionContent;


	CheatFeautre.Initialize();
	while (bRunWorkThread)
	{
		if (!CGameShareMemory::GetInstance().ExistAction(ActionContent))
		{
			::Sleep(100);
			continue;
		}


		DWORD dwSelectedGameObject = CheatFeautre.GetSelectedGameObject();
		if (ActionContent.emActionType != em_Action_Type::PrintItem && dwSelectedGameObject == NULL)
		{
			LOG_C_E(L"Please Select Your Operation Hero...");
			continue;
		}

		
		switch (ActionContent.emActionType)
		{
		case em_Action_Type::ChangeItem:
			CheatFeautre.ChangeItem(dwSelectedGameObject, ActionContent.GetChangeItemParamToItemIndex(), ActionContent.GetChangeItemParamToItemId());
			break;
		case em_Action_Type::PrintItem:
			CheatFeautre.PrintItem();
			break;
		case em_Action_Type::SetSelectedHeroSkillCool:
			CheatFeautre.SetSelectedHeroSkillCool(dwSelectedGameObject);
			break;
		case em_Action_Type::SetSelectedObjectAttackType:
			CheatFeautre.SetSelectedObjectAttackType(dwSelectedGameObject);
			break;
		case em_Action_Type::SetSelectedObjectInvincible:
			CheatFeautre.SetSelectedObjectInvincible(dwSelectedGameObject);
			break;
		default:
			break;
		}
	}

	
	return 0;
}

VOID Release()
{
	if (hWorkThread != NULL)
	{
		bRunWorkThread = FALSE;
		::WaitForSingleObject(hWorkThread, INFINITE);
		::CloseHandle(hWorkThread);
		hWorkThread = NULL;

		CGameShareMemory::GetInstance().Release();
		libTools::CLog::GetInstance().Release();
	}
}

BOOL APIENTRY DllMain(HMODULE , DWORD  ul_reason_for_call, LPVOID )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (hWorkThread == NULL && CGameShareMemory::GetInstance().Run())
		{
			bRunWorkThread = TRUE;
			libTools::CLog::GetInstance().SetClientName(L"War3", std::wstring(CGameShareMemory::GetInstance().GetConsolePath()) + L"\\");
			hWorkThread = ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_WorkThread, NULL, NULL, NULL);
		}
		return TRUE;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		Release();
		return FALSE;
	}
	return TRUE;
}

