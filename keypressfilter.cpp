#include "keypressfilter.h"
#include <QTextStream>

#define MAX_X 10000
#define EPS 10

KeyPressFilter *KeyPressFilter::instance = 0;

KeyPressFilter::KeyPressFilter()
{
	pressedKeys = new QSet<QString>;
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

KeyPressFilter::~KeyPressFilter()
{
	delete pressedKeys;
	instance = nullptr;
}

int KeyPressFilter::levenshteinDistance(const QVector<QPair<int, int> > &src, const QVector<QPair<int, int> > &dst)
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

void KeyPressFilter::addPoint(float x, float y)
{
	minX = std::min(x, minX);
	maxX = std::max(x, maxX);

	minY = std::min(y, minY);
	maxY = std::max(y, maxY);

	allPoints.append(qMakePair(x, y));
}

void KeyPressFilter::clearMatrix()
{
	allPoints.clear();
	minX = MAX_X, maxX = -MAX_X, minY = MAX_X, maxY = -MAX_X;
	for(int j = 0; j < MAP_SIZE; j++)
	{
		for(int i = 0; i < MAP_SIZE; i++)
		{
			mapOfGestures[i][j] = 0;
		}
	}
}

bool KeyPressFilter::vectorEquals(const QVector < QPair<int, int> > & v1, const QVector < QPair<int, int> > & v2)
{
	return(min(v1.size(), v2.size()) >= 1 * levenshteinDistance(v1, v2));
}

QVector < QPair<int, int> > reversed(QVector < QPair<int, int> > & in ) {
	QVector < QPair<int, int> > result;
	result.reserve( in.size() ); // reserve is new in Qt 4.7
	std::reverse_copy( in.begin(), in.end(), std::back_inserter( result ) );
	return result;
}

void KeyPressFilter::findMacros()
{

	minX -= EPS;
	minY -= EPS;

	maxX += EPS;
	maxY += EPS;

	float dx = (maxX - minX)/MAP_SIZE;
	float dy = (maxY - minY)/MAP_SIZE;

	int indexOfBox = 0;

	keyVector.clear();

	for (int j = 0; j < allPoints.size(); ++j)
	{
		int idX = -1;
		int idY = -1;
		//qDebug() << allPoints.at(j).first << " " << allPoints.at(j).second;
		for(int i = 0; i < MAP_SIZE; i++)
			if(minX + i * dx <= allPoints.at(j).first && minX + (i + 1) * dx >= allPoints.at(j).first)
			{
				idX = i;
			}
		for(int i = 0; i < MAP_SIZE; i++)
			if(minY + i * dy <= allPoints.at(j).second && allPoints.at(j).second <= minY + (i + 1) * dy)
			{
				idY = i;
			}
		if(idX != -1 && idY != -1)
		{
			if(mapOfGestures[idX][idY] == 0)
			{
				indexOfBox++;
				keyVector.append(qMakePair(idX, idY));
				std::cerr << idX << " " << idY << std::endl;
			}
			mapOfGestures[idX][idY] = indexOfBox;
		}
		//qDebug() << idX << " " << idY;
	}

	//$HORIZONTALLINE
	tempVector.clear();
	tempVector.append(qMakePair(0, 5));
	tempVector.append(qMakePair(1, 5));
	tempVector.append(qMakePair(2, 5));
	tempVector.append(qMakePair(3, 5));
	tempVector.append(qMakePair(4, 5));
	tempVector.append(qMakePair(5, 5));
	tempVector.append(qMakePair(6, 5));
	tempVector.append(qMakePair(7, 5));
	tempVector.append(qMakePair(8, 5));
	tempVector.append(qMakePair(9, 5));

	if(vectorEquals(tempVector, keyVector))
		emitMouseThrow("$HORIZONTALLINE");
	if(vectorEquals(reversed(tempVector), keyVector))
		emitMouseThrow("$HORIZONTALLINE");
	//$VERTICALLINE
	tempVector.clear();
	tempVector.append(qMakePair(5, 0));
	tempVector.append(qMakePair(5, 1));
	tempVector.append(qMakePair(5, 2));
	tempVector.append(qMakePair(5, 3));
	tempVector.append(qMakePair(5, 4));
	tempVector.append(qMakePair(5, 5));
	tempVector.append(qMakePair(5, 6));
	tempVector.append(qMakePair(5, 7));
	tempVector.append(qMakePair(5, 8));
	tempVector.append(qMakePair(5, 9));

	if(vectorEquals(tempVector, keyVector))
		emitMouseThrow("$VERTICALLINE");
	if(vectorEquals(reversed(tempVector), keyVector))
		emitMouseThrow("$VERTICALLINE");

	//$LEFTDOWNCORNER
	tempVector.append(qMakePair( 0 ,  0 ));
	tempVector.append(qMakePair( 0 ,  1 ));
	tempVector.append(qMakePair( 0 ,  2 ));
	tempVector.append(qMakePair( 0 ,  3 ));
	tempVector.append(qMakePair( 0 ,  4 ));
	tempVector.append(qMakePair( 0 ,  5 ));
	tempVector.append(qMakePair( 0 ,  6 ));
	tempVector.append(qMakePair( 0 ,  7 ));
	tempVector.append(qMakePair( 0 ,  8 ));
	tempVector.append(qMakePair( 0 ,  9 ));
	tempVector.append(qMakePair( 1 ,  9 ));
	tempVector.append(qMakePair( 2 ,  9 ));
	tempVector.append(qMakePair( 3 ,  9 ));
	tempVector.append(qMakePair( 4 ,  9 ));
	tempVector.append(qMakePair( 5 ,  9 ));
	tempVector.append(qMakePair( 6 ,  9 ));
	tempVector.append(qMakePair( 7 ,  9 ));
	tempVector.append(qMakePair( 8 ,  9 ));
	tempVector.append(qMakePair( 9 ,  9 ));
	/*
	1 0 0 0 0 0 0 0 0 0
	1 0 0 0 0 0 0 0 0 0
	1 0 0 0 0 0 0 0 0 0
	1 0 0 0 0 0 0 0 0 0
	1 0 0 0 0 0 0 0 0 0
	1 0 0 0 0 0 0 0 0 0
	1 0 0 0 0 0 0 0 0 0
	1 0 0 0 0 0 0 0 0 0
	1 0 0 0 0 0 0 0 0 0
	1 1 1 1 1 1 1 1 1 1
	*/
	if(vectorEquals(tempVector, keyVector))
		emitMouseThrow("$LEFTLOWERCORNER");
}

LRESULT CALLBACK KeyPressFilter::MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (wParam == WM_KEYDOWN || wParam == WM_KEYUP)
	{
		GUITHREADINFO threadInfo;
		threadInfo.cbSize = sizeof(GUITHREADINFO);
		DWORD tid = GetWindowThreadProcessId(GetActiveWindow(),0);
		GetGUIThreadInfo(tid,&threadInfo);

		if (wParam == WM_KEYDOWN)
			qDebug() << "Key Pressed!";
		else
			qDebug() << "Key Unpressed!";

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
			getInstance()->emitKeyThrow(static_cast<char>(cKey.vkCode));
		} else {
			getInstance()->pressedKeys->remove(keyName.toUpper());
		}
	}
	std::cerr << "OK" << std::endl;
	return CallNextHookEx(getInstance()->hHook, nCode, wParam, lParam);
}

LRESULT CALLBACK KeyPressFilter::MyLowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(wParam == WM_LBUTTONDOWN)
	{
		getInstance()->clearMatrix();
		qDebug() << "Mouse Pressed!";

		getInstance()->mouseDown();

		tagMSLLHOOKSTRUCT cMouse = *((tagMSLLHOOKSTRUCT*)lParam);
		qDebug() << cMouse.pt.x << " " << cMouse.pt.y;
		getInstance()->addPoint(cMouse.pt.x, cMouse.pt.y);
	}

	if(wParam == WM_LBUTTONUP)
	{
		qDebug() << "Mouse UP!";

		getInstance()->mouseUp();

		tagMSLLHOOKSTRUCT cMouse = *((tagMSLLHOOKSTRUCT*)lParam);
		qDebug() << cMouse.pt.x << " " << cMouse.pt.y;

		getInstance()->findMacros();
	}

	if(wParam == WM_MOUSEMOVE && getInstance()->mousePressed)
	{
		qDebug() << "Mouse Moved!";

		tagMSLLHOOKSTRUCT cMouse = *((tagMSLLHOOKSTRUCT*)lParam);
		qDebug() << cMouse.pt.x << " " << cMouse.pt.y;

		getInstance()->addPoint(cMouse.pt.x, cMouse.pt.y);
	}

	return CallNextHookEx(getInstance()->hHookMouse, nCode, wParam, lParam);
}
