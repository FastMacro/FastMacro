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
#include <vector>
#include <algorithm>

using namespace std;

#define MAP_SIZE 20

class KeyPressFilter : public QObject
{
		Q_OBJECT

public:

	static KeyPressFilter *getInstance() {
		if (!instance)
			instance = new KeyPressFilter;
		return instance;
	}

	QSet<QString> *getPressedKeys() {
		return pressedKeys;
	}

	void enable() {
		enabled = true;
	}

	void disable() {
		enabled = false;
	}

	void mouseUp() {
		mousePressed = false;
	}

	void mouseDown() {
		mousePressed = true;
	}

	static LRESULT CALLBACK MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MyLowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

	~KeyPressFilter();

private:
	KeyPressFilter()
	{
		pressedKeys = new QSet<QString>;
		enabled = true;
		mousePressed = false;

		hHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyLowLevelKeyBoardProc, NULL, 0);
		hHookMouse = SetWindowsHookEx(WH_MOUSE_LL, MyLowLevelMouseProc, NULL, 0);

		if(hHook == NULL)
		{
			qDebug() << "Hook failed";
		}
		if(hHookMouse == NULL)
		{
			qDebug() << "Hook failed";
		}
	}

	static void UpdateKeyState(BYTE *keystate, int keycode)
	{
		keystate[keycode] = GetKeyState(keycode);
	}

	int levenshteinDistance(const QVector < QPair<int, int> > & src, const QVector < QPair<int, int> > & dst)
	{
		int m = src.size();
		int n = dst.size();

		if (m == 0)
		{
			return n;
		}
		if (n == 0)
		{
			return m;
		}

		std::vector< std::vector<int> > matrixForDynamic(m + 1);

		for (int i = 0; i <= m; ++i)
		{
			matrixForDynamic[i].resize(n + 1);
			matrixForDynamic[i][0] = i;
		}
		for (int i = 0; i <= n; ++i)
		{
			matrixForDynamic[0][i] = i;
		}

		int above_cell, left_cell, diagonal_cell, cost;

		for (int i = 1; i <= m; ++i)
		{
			for(int j = 1; j <= n; ++j)
			{
				cost = src[i - 1] == dst[j - 1] ? 0 : 1;
				above_cell = matrixForDynamic[i - 1][j];
				left_cell = matrixForDynamic[i][j - 1];
				diagonal_cell = matrixForDynamic[i - 1][j - 1];
				matrixForDynamic[i][j] = std::min(std::min(above_cell + 1, left_cell + 1), diagonal_cell + cost);
			}
		}

		return matrixForDynamic[m][n];
	}

	void emitKeyThrow(QChar symbol) {
		if (enabled)
			emit throwChar(symbol);
	}

	void emitMouseThrow() {
		if (enabled)
			emit throwMouseEvent();
	}

	HHOOK hHookMouse = NULL;
	HHOOK hHook = NULL;

	void addPoint(float pointX, float pointY);
	void clearMatrix();
	void findMacros();

	bool vectorEquals(const QVector<QPair<int, int> > &v1, const QVector<QPair<int, int> > &v2);

	HHOOK hHookFocus;
	static KeyPressFilter *instance;
	bool enabled;
	bool mousePressed;
	int mapOfGestures[MAP_SIZE][MAP_SIZE];

	float minX;
	float minY;
	float maxX;
	float maxY;

	QSet<QString> *pressedKeys;
	QVector < QPair<float, float> > allPoints;
	QVector < QPair<int, int> > keyVector;
	QVector < QPair<int, int> > tempVector;

signals:
	void throwChar(QChar key);
	void throwMouseEvent();
};
