// NppSharp - C#/.NET Scripting Plugin for Notepad++
// Copyright (C) 2012  Chris Mrazek
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "StdAfx.h"
#include "Main.h"
#include "NppInterface.h"
#include "ClrUtil.h"

namespace NppSharp
{
	struct Globals
	{
	public:
		bool					initialized;
		gcroot<NppInterface^>	npp;
		FuncItem*				funcList;
		wstring					strConfigDir;
		list<ShortcutKey>		shortcuts;

		Globals()
			: initialized(false)
			, funcList(NULL)
		{ }
	};
	Globals g;

	

	#pragma unmanaged

	extern "C" __declspec(dllexport) void setInfo(NppData nppData)
	{
		SetPluginInfo(nppData);
	}

	extern "C" __declspec(dllexport) const TCHAR * getName()
	{
		return PLUGIN_NAME;
	}

	extern "C" __declspec(dllexport) FuncItem* getFuncsArray(int *pNumFuncsOut)
	{
		return GetFuncList(pNumFuncsOut);
	}

	extern "C" __declspec(dllexport) void beNotified(SCNotification *pNotify)
	{
		OnNotify(pNotify);
	}

	extern "C" __declspec(dllexport) LRESULT messageProc(UINT uiMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uiMsg)
		{
		case WM_COMMAND:
			OnCommand(wParam);
			break;
		}
		return TRUE;
	}

	extern "C" __declspec(dllexport) BOOL isUnicode()
	{
		return TRUE;
	}

	#pragma managed

#include "CmdList.h"

	//void CreateConfigDir(TCHAR *pszNppConfigDir)
	//{
	//	wstring path = pszNppConfigDir;
	//	if (path[path.length() - 1] != L'\\') path += L'\\';
	//	path += CONFIG_DIR_NAME;

	//	DWORD dwAttr = ::GetFileAttributes(path.c_str());
	//	if (dwAttr == INVALID_FILE_ATTRIBUTES || (dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0)
	//	{
	//		if (!::CreateDirectory(path.c_str(), NULL))
	//		{
	//			// Can't create for some reason, so just use the npp config dir.
	//			g.strConfigDir = pszNppConfigDir;
	//		}
	//		else
	//		{
	//			g.strConfigDir = path;
	//		}
	//	}
	//	else
	//	{
	//		g.strConfigDir = path;
	//	}
	//}

	void SetPluginInfo(NppData nppData)
	{
		try
		{
			//// Create the config directory.
			//TCHAR	szDir[MAX_PATH] = TEXT("");
			//SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)szDir);
			//CreateConfigDir(szDir);
			//logOpen(g.strConfigDir.c_str());
			//logWrite(L"NppSharp started.");

			// Initialize the main plugin object.
			g.npp = gcnew NppInterface(nppData._nppHandle, nppData._scintillaMainHandle, nppData._scintillaSecondHandle);
			Plugin::Initialize(g.npp);
			g.initialized = true;
		}
		catch (Exception^ ex)
		{
			wstring str = ClrStringToWString(String::Concat("CLR Exception in SetPluginInfo:\n", ex));
			::MessageBox(nppData._nppHandle, str.c_str(), L"Error", MB_OK | MB_ICONERROR);
		}
		catch (std::exception ex)
		{
			wstring str = ClrStringToWString(String::Concat("STD Exception in SetPluginInfo:\n", gcnew String(ex.what())));
			::MessageBox(nppData._nppHandle, str.c_str(), L"Error", MB_OK | MB_ICONERROR);
		}
		catch (...)
		{
			::MessageBox(nppData._nppHandle, L"Unknown exception in SetPluginInfo.", L"Error", MB_OK | MB_ICONERROR);
		}
	}

	FuncItem* GetFuncList(int *pNumFuncsOut)
	{
		try
		{
			List<PluginCommand^>^ funcList = g.npp->GetCommandList();

			g.funcList = new FuncItem[funcList->Count];
			memset(g.funcList, 0, sizeof(FuncItem) * funcList->Count);

			int i = 0;
			for each (PluginCommand ^f in funcList)
			{
				FuncItem *pItem = g.funcList + i;

				String ^funcName = f->Name;
				if (funcName->Length >= nbChar) funcName = funcName->Substring(0, nbChar - 1);
				pin_ptr<const wchar_t> cstr = PtrToStringChars(funcName);

				lstrcpy(pItem->_itemName, cstr);
				if (lstrcmp(cstr, L"-") != 0) pItem->_pFunc = GetCommand(i);
				else pItem->_pFunc = NULL;

				NppShortcut^ nppShortcut = f->Shortcut;
				if (nppShortcut != nullptr)
				{
					ShortcutKey sc;
					sc._isCtrl = nppShortcut->Control;
					sc._isAlt = nppShortcut->Alt;
					sc._isShift = nppShortcut->Shift;
					sc._key = (UCHAR)nppShortcut->KeyCode;
					g.shortcuts.push_back(sc);
					pItem->_pShKey = &(*g.shortcuts.rbegin());
				}

				f->Index = i;
				i++;
			}

			g.npp->SetFuncItems(g.funcList, funcList->Count);
			*pNumFuncsOut = i;
			return g.funcList;
		}
		catch (Exception^ ex)
		{
			wstring str = ClrStringToWString(String::Concat("CLR Exception in GetFuncList:\n", ex));
			::MessageBox(g.npp->NppHandle, str.c_str(), L"Error", MB_OK | MB_ICONERROR);
		}
		catch (std::exception ex)
		{
			wstring str = ClrStringToWString(String::Concat("STD Exception in GetFuncList:\n", gcnew String(ex.what())));
			::MessageBox(g.npp->NppHandle, str.c_str(), L"Error", MB_OK | MB_ICONERROR);
		}
		catch (...)
		{
			::MessageBox(g.npp->NppHandle, L"Unknown exception in GetFuncList.", L"Error", MB_OK | MB_ICONERROR);
		}

		*pNumFuncsOut = 0;
		return NULL;
	}

	void OnNotify(SCNotification *pNotify)
	{
		try
		{
			if (!g.initialized) return;

			switch (pNotify->nmhdr.code)
			{
			case NPPN_READY:
				g.npp->OnReady();
				break;

			case NPPN_SHUTDOWN:
				g.npp->OnShutdown();
				//logWrite(L"Received shutdown notification, closing log.");
				//logClose();
				break;

			case NPPN_TBMODIFICATION:
				g.npp->OnTbModification();
				break;

			case NPPN_FILEBEFORECLOSE:
				g.npp->OnFileClosing(pNotify->nmhdr.idFrom);
				break;

			case NPPN_FILECLOSED:
				g.npp->OnFileClosed(pNotify->nmhdr.idFrom);
				break;

			case NPPN_FILEBEFOREOPEN:
				g.npp->OnFileOpening(pNotify->nmhdr.idFrom);
				break;

			case NPPN_FILEOPENED:
				g.npp->OnFileOpened(pNotify->nmhdr.idFrom);
				break;

			case NPPN_FILEBEFORESAVE:
				g.npp->OnFileSaving(pNotify->nmhdr.idFrom);
				break;

			case NPPN_FILESAVED:
				g.npp->OnFileSaved(pNotify->nmhdr.idFrom);
				break;

			case NPPN_BUFFERACTIVATED:
				g.npp->OnFileActivated(pNotify->nmhdr.idFrom);
				break;

			case NPPN_LANGCHANGED:
				g.npp->OnLanguageChanged(pNotify->nmhdr.idFrom);
				break;

			case NPPN_WORDSTYLESUPDATED:
				g.npp->OnStyleUpdate(pNotify->nmhdr.idFrom);
				break;

			case NPPN_FILEBEFORELOAD:
				g.npp->OnFileLoading();
				break;

			case NPPN_FILELOADFAILED:
				g.npp->OnFileLoadFailed();
				break;

			case NPPN_DOCORDERCHANGED:
				g.npp->OnFileOrderChanged(pNotify->nmhdr.idFrom);
				break;
			}
		}
		catch (Exception^ ex)
		{
			wstring str = ClrStringToWString(String::Concat("CLR Exception in OnNotify:\n", ex));
			::MessageBox(g.npp->NppHandle, str.c_str(), L"Error", MB_OK | MB_ICONERROR);
		}
		catch (std::exception ex)
		{
			wstring str = ClrStringToWString(String::Concat("STD Exception in OnNotify:\n", gcnew String(ex.what())));
			::MessageBox(g.npp->NppHandle, str.c_str(), L"Error", MB_OK | MB_ICONERROR);
		}
		catch (...)
		{
			::MessageBox(g.npp->NppHandle, L"Unknown exception in OnNotify.", L"Error", MB_OK | MB_ICONERROR);
		}
	}

	void OnCommand(int cmdId)
	{
		try
		{
			if (!g.initialized) return;
			g.npp->ExecuteCommandById(cmdId);
		}
		catch (Exception^ ex)
		{
			wstring str = ClrStringToWString(String::Concat("CLR Exception in OnCommand:\n", ex));
			::MessageBox(g.npp->NppHandle, str.c_str(), L"Error", MB_OK | MB_ICONERROR);
		}
		catch (std::exception ex)
		{
			wstring str = ClrStringToWString(String::Concat("STD Exception in OnCommand:\n", gcnew String(ex.what())));
			::MessageBox(g.npp->NppHandle, str.c_str(), L"Error", MB_OK | MB_ICONERROR);
		}
		catch (...)
		{
			::MessageBox(g.npp->NppHandle, L"Unknown exception in OnCommand.", L"Error", MB_OK | MB_ICONERROR);
		}
	}

}
