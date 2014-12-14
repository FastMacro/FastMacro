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

#define MAP_SIZE 10

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
	KeyPressFilter();

	static void UpdateKeyState(BYTE *keystate, int keycode)
	{
		keystate[keycode] = GetKeyState(keycode);
	}

	int levenshteinDistance(const QVector < QPair<int, int> > & src, const QVector < QPair<int, int> > & dst);

	void emitKeyThrow(QChar symbol) {
		emit throwChar(symbol);
	}

	void emitMouseThrow(QString type) {
		emit throwMouseEvent(type);
	}

	HHOOK hHookMouse = NULL;
	HHOOK hHook = NULL;

	void addPoint(float pointX, float pointY);
	void clearMatrix();
	void findMacros();

	bool vectorEquals(const QVector<QPair<int, int> > &v1, const QVector<QPair<int, int> > &v2);

	HHOOK hHookFocus;
	static KeyPressFilter *instance;
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
	void throwMouseEvent(QString type);
};
