#include "keypressfilter.h"

KeyPressFilter *KeyPressFilter::instance = 0;

LRESULT CALLBACK KeyPressFilter::MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if(wParam == WM_KEYDOWN || wParam == WM_KEYUP)
	{
		GUITHREADINFO threadInfo;
		threadInfo.cbSize = sizeof(GUITHREADINFO);
		DWORD tid = GetWindowThreadProcessId(GetActiveWindow(),0);
		GetGUIThreadInfo(tid,&threadInfo);
		HWND focus = threadInfo.hwndFocus;

		if (wParam == WM_KEYDOWN)
			qDebug() << "Key Pressed!";
		else
			qDebug() << "Key Unpressed!";

		//Get the key information
		KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);

		wchar_t buffer[5];

		BYTE keyboard_state[256];
		GetKeyboardState(keyboard_state);
		UpdateKeyState(keyboard_state, VK_SHIFT);
		UpdateKeyState(keyboard_state, VK_CAPITAL);
		UpdateKeyState(keyboard_state, VK_CONTROL);
		UpdateKeyState(keyboard_state, VK_MENU);

		HKL keyboard_layout = GetKeyboardLayout(0);

		char lpszName[0x100] = {0};

		DWORD dwMsg = 1;
		dwMsg += cKey.scanCode << 16;
		dwMsg += cKey.flags << 24;

		int i = GetKeyNameText(dwMsg, (LPTSTR)lpszName, 255);

		int result = ToUnicodeEx(cKey.vkCode, cKey.scanCode, keyboard_state, buffer, 4, 0, keyboard_layout);
		buffer[4] = L'\0';

		QString keyName = QString::fromUtf16((ushort*)lpszName);

		qDebug() << "key:" << cKey.vkCode << " " << keyName;
		if (wParam == WM_KEYDOWN) {
			getInstance()->pressedKeys->insert(keyName);
			getInstance()->emitThrow(static_cast<char>(cKey.vkCode));
		} else {
			getInstance()->pressedKeys->remove(keyName);
		}
	}

	return CallNextHookEx(getInstance()->hHook, nCode, wParam, lParam);
}
/*
void KeyPressFilter::setupEventHook(FREContext iCtx)
{
    ctx = iCtx;
    CoInitialize(NULL);

    evHook = SetWinEventHook(EVENT_OBJECT_FOCUS, EVENT_OBJECT_END, NULL,
    handleEventObjectFocus, 0, 0,
    WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

}
*/
/*
BOOL handleEventObjectFocus(DWORD thread)
 {
    GUITHREADINFO threadInfo;
    threadInfo.cbSize = sizeof(GUITHREADINFO);

    BOOL result = GetGUIThreadInfo(thread, &threadInfo);


    if(threadInfo.flags & GUI_CARETBLINKING)
    {
        qDebug() << "YEEEES";
    }

    return result;
}
*/
