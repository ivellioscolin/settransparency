// SetTransparency.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SetTransparency.h"
#include <string>

using namespace std;

HWND hWnd_main = NULL;
DWORD g_pid = 0;
BYTE g_alpha = 0;
wstring g_app_title = wstring(L"");
wregex g_pattern_title;

BOOL CALLBACK EnumWindowsProc(
	_In_  HWND hwnd,
	_In_  LPARAM lParam
	)
{
	DWORD pid;
	DWORD result;
	result = GetWindowThreadProcessId(hwnd, &pid);

	// Match PID
	if (pid == g_pid)
	{
		hWnd_main = hwnd;
		wchar_t buffer[MAXBYTE] = {0};
		GetWindowTextW(hwnd, buffer, MAXBYTE);
		wstring titleStr = wstring(buffer);

		// Match main window title
		if (regex_match(titleStr, g_pattern_title))
		{
			//MessageBox(NULL, _T("OK"), _T("OK"), MB_OK);
			if (IsWindow(hwnd))
			{
				LONG winLong = GetWindowLong(hwnd, GWL_EXSTYLE);
				winLong |= WS_EX_LAYERED;
				SetWindowLong(hwnd, GWL_EXSTYLE, winLong);
				SetLayeredWindowAttributes(hwnd, 0, g_alpha, LWA_ALPHA);
			}
		}
	}

	return TRUE;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	wstring parametersStr = wstring(lpCmdLine);

	wstring parPid = wstring(L"");
	wstring parAlpha = wstring(L"");
	wstring parAppTitle = wstring(L"");

	// Parser the input as 3 parameters
	wregex par_pattern(L"(\\S+)\\s+(\\S+)\\s+(.*)", regex_constants::ECMAScript);

	if (regex_match(parametersStr, par_pattern))
	{
		parPid = regex_replace(parametersStr, par_pattern, wstring(L"$1"));
		parAlpha = regex_replace(parametersStr, par_pattern, wstring(L"$2"));
		parAppTitle = regex_replace(parametersStr, par_pattern, wstring(L"$3"));
	}

	if ( (!parPid.empty()) && (!parAlpha.empty()) && (!parAppTitle.empty()) )
	{
		//MessageBox(NULL, _T("OK"), _T("OK"), MB_OK);
		g_pid = stoul(parPid, nullptr);
		g_alpha = static_cast<BYTE>(stoul(parAlpha, nullptr));
		g_app_title = parAppTitle;
		g_pattern_title = wregex(wstring(L".*") + g_app_title + wstring(L".*"), regex_constants::ECMAScript);

		// Validate the process
		if (OpenProcess(PROCESS_QUERY_INFORMATION, 0, g_pid) != NULL)
		{
			EnumWindows(EnumWindowsProc, NULL);
		}
	}
	
	return 0;
}
