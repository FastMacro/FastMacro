#pragma once
#include <QEvent>
#include <QKeyEvent>
#include <QtGui>
#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QTime>
#include <QChar>
#include <iostream>
#include <Windows.h>

using namespace std;

class KeyPressFilter : public QObject
{
		Q_OBJECT

public:

	static KeyPressFilter *getInstance() {
		if (!instance)
			instance = new KeyPressFilter;
		return instance;
	}

	static void UpdateKeyState(BYTE *keystate, int keycode)
	{
		keystate[keycode] = GetKeyState(keycode);
	}

	void enable() {
		enabled = true;
	}

	void disable() {
		enabled = false;
	}

	static LRESULT CALLBACK MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);

    ~KeyPressFilter() {}

private:
	KeyPressFilter()
	{
		pressedKeys = new QSet<QString>;
		hHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyLowLevelKeyBoardProc, NULL, 0);
		if(hHook == NULL)
		{
			qDebug() << "Hook failed";
		}
	}
	void emitThrow(QChar symbol) {
		if (enabled)
			emit throwChar(symbol);
	}

	HHOOK hHook;
    HHOOK hHookFocus;
	static KeyPressFilter *instance;
	bool enabled;
	QSet<QString> *pressedKeys;

signals:
	void throwChar(QChar key);
};
