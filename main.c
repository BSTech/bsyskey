/*

Fake Syskey with REAL resources, by BSTech.
https://software.bstechcomputing.net/syskey/
18.11.2019

Resource IDs taken by using Resource Hacker program.
And, yes, I am not a professional programmer, I just make them as a hobby project.
I hope this explains the dirty code below.

*/


#include <stdio.h>
#include <Windows.h>

static HMODULE g_hOrig, g_hMUI;
static wchar_t passfile[MAX_PATH];

INT_PTR __stdcall D1(HWND, UINT, WPARAM, LPARAM);
INT_PTR __stdcall D2(HWND, UINT, WPARAM, LPARAM);
INT_PTR __stdcall D3(HWND, UINT, WPARAM, LPARAM);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	wchar_t lang[6];
	
	GetLocaleInfoW(LOCALE_CUSTOM_UI_DEFAULT, LOCALE_SISO639LANGNAME, lang, 4);
	lang[2] = L'-';
	lang[5] = 0;
	GetLocaleInfoW(LOCALE_CUSTOM_UI_DEFAULT, LOCALE_SISO3166CTRYNAME, lang + 3, 4);
	
	wchar_t appdata[MAX_PATH];
	wchar_t winpath[MAX_PATH];
	wchar_t origpath[MAX_PATH];
	wchar_t muipath[MAX_PATH];
	
	GetEnvironmentVariableW(L"APPDATA", appdata, MAX_PATH);
	GetEnvironmentVariableW(L"WINDIR", winpath, MAX_PATH);
	wsprintfW(passfile, L"%s\\syskey.txt", appdata);
	wsprintfW(origpath, L"%s\\System32\\syskey.exe", winpath);
	wsprintfW(muipath, L"%s\\System32\\%s\\syskey.exe.mui", winpath, lang);
	
	g_hOrig = LoadLibraryW(origpath);
	g_hMUI = LoadLibraryW(muipath);
	
	DialogBoxParamW(g_hMUI, MAKEINTRESOURCEW(100), 0, D1, 0);
	
	return S_OK;
}

INT_PTR __stdcall D1(HWND h, UINT m, WPARAM w, LPARAM l)
{
	if (m == WM_INITDIALOG)
	{
		HANDLE hBmp = LoadImageW(g_hOrig, MAKEINTRESOURCEW(1), IMAGE_ICON, 64, 64, 0);
		HWND image = GetDlgItem(h, 101);
		
		SendMessageW(image, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hBmp);

		HWND radio1 = GetDlgItem(h, 104);
		EnableWindow(radio1, FALSE);

		HWND radio2 = GetDlgItem(h, 105);
		SendMessageW(radio2, BM_SETCHECK, BST_CHECKED, 0);
	}
	
	if (m == WM_COMMAND)
	{
		switch (LOWORD(w))
		{
			case 1:
			case 2:
				PostQuitMessage(0);
				break;
			case 106: DialogBoxParamW(g_hMUI, MAKEINTRESOURCEW(200), h, D2, 0); break;
		}
	}
	return FALSE;
}

INT_PTR __stdcall D2(HWND h, UINT m, WPARAM w, LPARAM l)
{
	if (m == WM_INITDIALOG)
	{
		HWND radio1 = GetDlgItem(h, 209);
		SendMessage(radio1, BM_SETCHECK, BST_CHECKED, 0);

		HWND radio2 = GetDlgItem(h, 211);
		SendMessage(radio2, BM_SETCHECK, BST_CHECKED, 0);

		EnableWindow(GetDlgItem(h, 203), FALSE);
		EnableWindow(GetDlgItem(h, 204), FALSE);
		EnableWindow(GetDlgItem(h, 205), FALSE);
		EnableWindow(GetDlgItem(h, 206), FALSE);
		EnableWindow(GetDlgItem(h, 207), FALSE);
		EnableWindow(GetDlgItem(h, 210), FALSE);
		EnableWindow(GetDlgItem(h, 212), FALSE);
	}

	if (m == WM_COMMAND)
	{
		switch (LOWORD(w))
		{
			case 1:
			{
				wchar_t t1[260], t2[260];
				GetDlgItemTextW(h, 205, t1, 260);
				GetDlgItemTextW(h, 207, t2, 260);
				
				FILE* hPassFile = 0;
				_wfopen_s(&hPassFile, passfile, L"w+");
				fwprintf_s(hPassFile, L"pass: %s\r\nconfirm: %s\r\n\r\n", t1, t2);
				fclose(hPassFile);
				
				if (wcscmp(t1, t2))
				{
					wchar_t title[64], text[256];
					LoadStringW(g_hMUI, 1501, title, 64);
					LoadStringW(g_hMUI, 1504, text, 256);

					MessageBoxW(h, text, title, MB_ICONERROR);
					return FALSE;
				}
				
				DialogBoxParamW(g_hMUI, MAKEINTRESOURCEW(900), h, D3, 0);
			}
			break;
			case 2: EndDialog(h, 0); break;
			case 202:
				if (IsDlgButtonChecked(h, 202))
				{
					EnableWindow(GetDlgItem(h, 203), TRUE);
					EnableWindow(GetDlgItem(h, 204), TRUE);
					EnableWindow(GetDlgItem(h, 205), TRUE);
					EnableWindow(GetDlgItem(h, 206), TRUE);
					EnableWindow(GetDlgItem(h, 207), TRUE);
					EnableWindow(GetDlgItem(h, 211), FALSE);
					EnableWindow(GetDlgItem(h, 213), FALSE);
				}
				break;
			case 209:
				if (IsDlgButtonChecked(h, 209))
				{
					EnableWindow(GetDlgItem(h, 203), FALSE);
					EnableWindow(GetDlgItem(h, 204), FALSE);
					EnableWindow(GetDlgItem(h, 205), FALSE);
					EnableWindow(GetDlgItem(h, 206), FALSE);
					EnableWindow(GetDlgItem(h, 207), FALSE);
					EnableWindow(GetDlgItem(h, 211), TRUE);
					EnableWindow(GetDlgItem(h, 213), TRUE);
				}
			break;
		}
	}
	return FALSE;
}

INT_PTR __stdcall D3(HWND h, UINT m, WPARAM w, LPARAM l)
{
	if (m == WM_INITDIALOG)
	{
		HANDLE hBmp = LoadImageW(g_hOrig, MAKEINTRESOURCEW(920), IMAGE_ICON, 64, 72, 0);
		HWND image = GetDlgItem(h, 904);
		
		SendMessageW(image, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hBmp);
	}

	if (m == WM_COMMAND)
	{
		if (LOWORD(w) == 2) EndDialog(h, 0);
		if (LOWORD(w) == 1)
		{
			wchar_t t1[260];
			GetDlgItemTextW(h, 903, t1, 260);
			
			EndDialog(h, 0);
			
			wchar_t title[64], text[256];
			
			if (wcslen(t1) > 0)
			{
				LoadStringW(g_hMUI, 1501, title, 64);
				FormatMessageW(4096, 0, 86, 0, text, 256, 0);
				
				MessageBoxW(h, text, title, MB_ICONERROR);
				return FALSE;
			}
			
			LoadStringW(g_hMUI, 1508, title, 64);
			LoadStringW(g_hMUI, 1507, text, 256);

			MessageBoxW(h, text, title, MB_ICONINFORMATION);
			PostQuitMessage(0);
		}
	}
	return FALSE;
}
