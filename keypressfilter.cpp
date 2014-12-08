#include "keypressfilter.h"

KeyPressFilter *KeyPressFilter::instance = 0;

LRESULT CALLBACK KeyPressFilter::MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (((wParam == WM_KEYDOWN && getInstance()->enabled) || wParam == WM_KEYUP))
	{
		GUITHREADINFO threadInfo;
		threadInfo.cbSize = sizeof(GUITHREADINFO);
		DWORD tid = GetWindowThreadProcessId(GetActiveWindow(),0);
		GetGUIThreadInfo(tid,&threadInfo);

		if (wParam == WM_KEYDOWN)
			qDebug() << "Key Pressed!";
		else
			qDebug() << "Ksey Unpressed!";

		//Get the key information
		KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);

		BYTE keyboard_state[256];
		GetKeyboardState(keyboard_state);
		UpdateKeyState(keyboard_state, VK_SHIFT);
		UpdateKeyState(keyboard_state, VK_CAPITAL);
		UpdateKeyState(keyboard_state, VK_CONTROL);
		UpdateKeyState(keyboard_state, VK_MENU);

		char lpszName[0x100] = {0};
		DWORD dwMsg = 1;
		dwMsg += cKey.scanCode << 16;
		dwMsg += cKey.flags << 24;
		GetKeyNameText(dwMsg, (LPTSTR)lpszName, 255);
		QString keyName = QString::fromUtf16((ushort*)lpszName);

		qDebug() << "key:" << cKey.vkCode << " " << keyName;
		if (wParam == WM_KEYDOWN) {
			getInstance()->pressedKeys->insert(keyName.toUpper());
			getInstance()->emitThrow(static_cast<char>(cKey.vkCode));
		} else {
			getInstance()->pressedKeys->remove(keyName.toUpper());
		}
	}
	return CallNextHookEx(getInstance()->hHook, nCode, wParam, lParam);
}
