#include "keypressfilter.h"
#include <QTextStream>

KeyPressFilter *KeyPressFilter::instance = 0;

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
    minX = 10000, maxX = -10000, minY = 10000, maxY = -10000;
    for(int j = 0; j < 10; j++)
    {
        for(int i = 0; i < 10; i++)
        {
            mapOfGestures[i][j] = 0;
        }
    }
}

bool KeyPressFilter::vectorEquals(const QVector < QPair<int, int> > & v1, const QVector < QPair<int, int> > & v2)
{
    return(min(v1.size(), v2.size()) >= 10 * levenshtein_distance(v1, v2));
}

void KeyPressFilter::findMacros()
{

    minX -= 10;
    minY -= 10;

    maxX += 10;
    maxY += 10;

    float dx = (maxX - minX)/10;
    float dy = (maxY - minY)/10;

    int indexOfBox = 0;

    keyVector.clear();

    for (int j = 0; j < allPoints.size(); ++j)
    {
        int idX = -1;
        int idY = -1;
        //qDebug() << allPoints.at(j).first << " " << allPoints.at(j).second;
        for(int i = 0; i < 10; i++)
            if(minX + i * dx <= allPoints.at(j).first && minX + (i + 1) * dx >= allPoints.at(j).first)
            {
                idX = i;
            }
        for(int i = 0; i < 10; i++)
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

    //FOREACH MACROS
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
    {
        std::cerr << "Macros" << std::endl;
    }

    //--------------

    for(int j = 0; j < 10; j++)
    {
        for(int i = 0; i < 10; i++)
        {
            std::cerr << mapOfGestures[i][j] << " ";
        }
        qDebug();
    }
}

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
