#include "ProcessList.h"
#include <psapi.h>

#include <QtWinExtras/QtWin>

ProcessList::ProcessList(QTreeWidgetItem* root)
{
	m_Root = root;
	m_NullIcon = QPixmap(16, 16);
}
ProcessList::~ProcessList()
{
	clear();
}
void ProcessList::clear()
{
	QList<QTreeWidgetItem*> children;

	children = m_Root->takeChildren();
	for(int i = 0; i < children.size(); i++)
	{
		delete children.at(i);
	}
	children.clear();
}
void ProcessList::refresh()
{
	clear();
	DWORD aProcesses[1024], cbNeeded, cProcesses;

	if(!EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
	{
		return;
	}
	cProcesses = cbNeeded / sizeof(DWORD);

	for(int i = 0; i < cProcesses; i++)
	{
		if(aProcesses[i] != 0)
		{
			DWORD dwProcessId = aProcesses[i];
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);

			if(hProcess != NULL)
			{
				HMODULE hMod;
				SHFILEINFO shfi;
				ProcessList::Node* node = new ProcessList::Node();
				TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
				TCHAR szProcessPath[MAX_PATH] = TEXT("");

				if(EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded) )
				{
					GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR));
					GetModuleFileNameEx(hProcess, hMod, szProcessPath, sizeof(szProcessPath)/sizeof(TCHAR));

					// Obtener el icono del proceso
					memset(&shfi, 0, sizeof(shfi));
					SHGetFileInfo(szProcessPath, 0, &shfi, sizeof(shfi), SHGFI_SMALLICON | SHGFI_ICON);
					if(!shfi.hIcon)
					{
						node->m_Icon = m_NullIcon;
					}
					else
					{
			#if (QT_VERSION_MAJOR >= 5)
						node->m_Icon = QtWin::fromHICON(shfi.hIcon);
			#else
						node->m_Icon = QPixmap::fromWinHICON(shfi.hIcon);
			#endif
						DestroyIcon(shfi.hIcon);
					}
				}
				node->m_ProcessId = dwProcessId;
				node->m_BaseName = QString::fromWCharArray(szProcessName);
				node->m_FileName = QString::fromWCharArray(szProcessPath);
				m_Root->addChild(node);
			}
			CloseHandle(hProcess);
		}
	}
}


