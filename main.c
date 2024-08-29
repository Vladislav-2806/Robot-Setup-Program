#undef UNICODE
#pragma warning(disable: 4996)
#pragma comment(lib, "ucrtbased.dll")
#pragma comment(lib, "vcruntime140d.dll")

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <tchar.h>

HFONT helloTextFont;
HFONT wifiSetupTextFont;
HFONT wifiSetupTextFont2;
HFONT wifiSetupTextFont3;
HFONT wifiSetupTextFont4;

HWND nameWifiEdit = 0;
HWND passWifiEdit = 0;
HWND mainHwnd;

wchar_t portNames[70][20] = { L"\\\\.\\COM0", L"\\\\.\\COM1", L"\\\\.\\COM2", L"\\\\.\\COM3", L"\\\\.\\COM4", L"\\\\.\\COM5", L"\\\\.\\COM6", L"\\\\.\\COM7",
                            L"\\\\.\\COM8", L"\\\\.\\COM9", L"\\\\.\\COM10", L"\\\\.\\COM11", L"\\\\.\\COM12", L"\\\\.\\COM13", L"\\\\.\\COM14", L"\\\\.\\COM15",
                            L"\\\\.\\COM16", L"\\\\.\\COM17", L"\\\\.\\COM18", L"\\\\.\\COM19", L"\\\\.\\COM20", L"\\\\.\\COM21", L"\\\\.\\COM22", L"\\\\.\\COM23",
                            L"\\\\.\\COM24", L"\\\\.\\COM25", L"\\\\.\\COM26", L"\\\\.\\COM27", L"\\\\.\\COM28", L"\\\\.\\COM29", L"\\\\.\\COM30", L"\\\\.\\COM31",
                            L"\\\\.\\COM32", L"\\\\.\\COM33", L"\\\\.\\COM34", L"\\\\.\\COM35", L"\\\\.\\COM36", L"\\\\.\\COM37", L"\\\\.\\COM38", L"\\\\.\\COM39",
                            L"\\\\.\\COM40", L"\\\\.\\COM41", L"\\\\.\\COM42", L"\\\\.\\COM43", L"\\\\.\\COM44", L"\\\\.\\COM45", L"\\\\.\\COM46", L"\\\\.\\COM47",
                            L"\\\\.\\COM48", L"\\\\.\\COM49", L"\\\\.\\COM50", L"\\\\.\\COM51", L"\\\\.\\COM52", L"\\\\.\\COM53", L"\\\\.\\COM54", L"\\\\.\\COM55",
                            L"\\\\.\\COM56", L"\\\\.\\COM57", L"\\\\.\\COM58", L"\\\\.\\COM59", L"\\\\.\\COM60" };

int windowSize[2] = { 600, 400 };
int mouseCoords[2] = { 0, 0 };
int click = 0;
int connectComOk = -1;
int nameWifiEditId = 0;
int passWifiEditId = 1;
int cableConnected = 0;
char activeComPort[20] = "";

char wifiInformation[2][200];

LRESULT WndProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp) {
    if (message == WM_DESTROY) PostQuitMessage(0);
    else if (message == WM_MOUSEMOVE) {
        mouseCoords[0] = LOWORD(lp);
        mouseCoords[1] = HIWORD(lp);
    }
    else if (message == WM_KEYDOWN) {
    }
    else if (message == WM_LBUTTONDOWN) {
        click = 1;
    }
    else if (message == WM_COMMAND) {
        if (LOWORD(wp) == nameWifiEditId) GetWindowText(lp, wifiInformation[0], 200);
        else if (LOWORD(wp) == passWifiEditId) GetWindowText(lp, wifiInformation[1], 200);
    }
    else if (message == WM_LBUTTONUP) {
        click = 0;
    }
    else if (message == WM_KEYUP) {
    }
    else DefWindowProcA(hwnd, message, wp, lp);
}

int intLimit(int number, int maxNumber) {
    if (number > maxNumber) return maxNumber;
    return number;
}

void WinInit() {
    helloTextFont = CreateFontW(70, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 1, 0, 0, 0, 0, "Calibri");
    wifiSetupTextFont = CreateFontW(22, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 1, 0, 0, 0, 0, "Calibri");
    wifiSetupTextFont2 = CreateFontW(40, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 1, 0, 0, 0, 0, "Calibri");
    wifiSetupTextFont3 = CreateFontW(30, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 1, 0, 0, 0, 0, "Calibri");
    wifiSetupTextFont4 = CreateFontW(35, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 1, 0, 0, 0, 0, "Calibri");

}

int HelloTextAnimation(HDC dc) {
    static float textColor = -1;
    static int textColorVector = 1;

    textColor += 0.05*textColorVector;

    if (textColor > 3.5) textColorVector = -1;

    if (textColor > 1) SetTextColor(dc, RGB(intLimit(250 * textColor, 255), intLimit(192 * textColor, 255), intLimit(255 * (textColor - 1), 255)));
    else if (textColorVector == 1) SetTextColor(dc, RGB(250, 192, 0));
    else {
        textColor = -1;
        textColorVector = 1;
        return 1;
    }


    SetBkColor(dc, RGB(250, 192, 0));
    SelectObject(dc, helloTextFont);

    char helloText[2][100] = { "Давайте настроим", "вашего робота." };

    TextOut(dc, 30, 70, helloText[0], strlen(helloText[0]));
    TextOut(dc, 70, 150, helloText[1], strlen(helloText[1]));

    return 0;
}

int SetupWifiAnimation(HDC dc) {
    static float textColor = 1;
    static int textColorVector = 1;

    if (textColor < 2 && textColorVector == 1) textColor += 0.05;
    else if (textColorVector == 0 && textColor > 1.1) textColor -= 0.1;
    else if (textColorVector == 0) {
        DestroyWindow(nameWifiEdit);
        DestroyWindow(passWifiEdit);

        nameWifiEdit = 0;
        textColor = 1;
        textColorVector = 1;
        return 1;
    }

    char wifiText[5][110] = { "Подключите вашего робота, указав данные вашей Wi-fi сети.", "Имя сети", "Пароль", "Продолжить",
                            "Внимание! Использование русских букв преведёт к ошибке!"};

    SelectObject(dc, wifiSetupTextFont);
    SetBkColor(dc, RGB(250, 192, 0));

    if (textColor > 1.9) {
        SetTextColor(dc, RGB(230, 50, 50));
        TextOut(dc, 20, 35, wifiText[4], strlen(wifiText[4]));
    }

    SetTextColor(dc, RGB(intLimit(250 * textColor, 255), intLimit(192 * textColor, 255), intLimit(255 * (textColor - 1), 255)));

    TextOut(dc, 20, 10, wifiText[0], strlen(wifiText[0]));

    SelectObject(dc, wifiSetupTextFont2);
    TextOut(dc, 20, 70, wifiText[1], strlen(wifiText[1]));

    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(intLimit(250 * textColor, 255), intLimit(192 * textColor, 255), intLimit(255 * (textColor - 1), 255)));

    RoundRect(dc, 20, 120, 450, 160, 20, 20);

    if (textColor >= 2 && !nameWifiEdit) {
        nameWifiEdit = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE, 30, 122, 410, 36, mainHwnd, nameWifiEditId, NULL, NULL);
        SendMessage(nameWifiEdit, WM_SETFONT, wifiSetupTextFont4, 0);

        passWifiEdit = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE, 30, 232, 410, 36, mainHwnd, passWifiEditId, NULL, NULL);
        SendMessage(passWifiEdit, WM_SETFONT, wifiSetupTextFont4, 0);
    }

    TextOut(dc, 20, 180, wifiText[2], strlen(wifiText[2]));

    RoundRect(dc, 20, 230, 450, 270, 20, 20);

    SetTextColor(dc, RGB(250, 192, 0));
    SetBkColor(dc, RGB(intLimit(250 * textColor, 255), intLimit(192 * textColor, 255), intLimit(255 * (textColor - 1), 255)));
    SelectObject(dc, wifiSetupTextFont3);

    if (mouseCoords[0] > 400 && mouseCoords[1] > 310 && mouseCoords[0] < 570 && mouseCoords[1] < 350) {
        RoundRect(dc, 400, 305, 570, 345, 20, 20);
        TextOut(dc, 413, 310, wifiText[3], strlen(wifiText[3]));

        if (click) {
            if (strlen(wifiInformation[0]) == 0) MessageBox(NULL, "Похоже, что вы не указали имя вашей сети.", "Внимание!", MB_OK);
            else if(MessageBox(NULL, "Вы уверены, что ввели данные верно?", "Внимание!", MB_YESNO)==6) textColorVector = 0;
            click = 0;
        }
    }
    else {
        RoundRect(dc, 400, 310, 570, 350, 20, 20);
        TextOut(dc, 413, 315, wifiText[3], strlen(wifiText[3]));
    }
    return 0;
}

char* ShowCOMPorts() {
    static char comPorts[100][10];
    static char pastComPorts[100][10];
    static int countComPorts = -1;

    int r = 0;

    for (int i = 0; i < 100; i++) {
        strcpy(comPorts[i], "");
    }

    HKEY hkey = NULL;
    r = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM\\"), 0, KEY_READ, &hkey);
    if (r != ERROR_SUCCESS) {
        MessageBox(NULL, "Произошла непредвиденная ошибка, пожалуйста, попробуйте еще раз.", "Ошибка!", MB_OK);
        exit(1);
        return "";
    }

    unsigned long CountValues = 0, MaxValueNameLen = 0, MaxValueLen = 0;
    RegQueryInfoKey(hkey, NULL, NULL, NULL, NULL, NULL, NULL, &CountValues, &MaxValueNameLen, &MaxValueLen, NULL, NULL);
    ++MaxValueNameLen;
    TCHAR* bufferName = NULL, * bufferData = NULL;
    bufferName = (TCHAR*)malloc(MaxValueNameLen * sizeof(TCHAR));
    if (!bufferName)
    {
        RegCloseKey(hkey);
        MessageBox(NULL, "Произошла непредвиденная ошибка, пожалуйста, попробуйте еще раз.", "Ошибка!", MB_OK);
        exit(1);
        return "";
    }
    bufferData = (TCHAR*)malloc((MaxValueLen + 1) * sizeof(TCHAR));
    if (!bufferData)
    {
        free(bufferName);
        RegCloseKey(hkey);
        MessageBox(NULL, "Произошла непредвиденная ошибка, пожалуйста, попробуйте еще раз.", "Ошибка!", MB_OK);
        exit(1);
        return "";
    }

    unsigned long NameLen, type, DataLen;
    for (unsigned int i = 0; i < CountValues; i++)
    {
        NameLen = MaxValueNameLen;
        DataLen = MaxValueLen;
        r = RegEnumValue(hkey, i, bufferName, &NameLen, NULL, &type, (LPBYTE)bufferData, &DataLen);
        if ((r != ERROR_SUCCESS) || (type != REG_SZ))
            continue;

        //wcstombs(comPorts[i], bufferData, wcslen(bufferData) + 1);
        strcpy(comPorts[i], bufferData);
    }
    free(bufferName);
    free(bufferData);
    RegCloseKey(hkey);

    if (countComPorts != -1 && abs(countComPorts - CountValues) == 1) {
        if (countComPorts < CountValues) {
            for (int i = 0; i < 100; i++) {
                int comIn = 0;
                for (int j = 0; j < 100; j++) {
                    if (strcmp(comPorts[i], pastComPorts[j]) == 0) {
                        comIn = 1;
                        break;
                    }
                }
                if (!comIn) {
                    for (int i = 0; i < 100; i++) {
                        strcpy(pastComPorts[i], "");
                    }
                    countComPorts = -1;
                    return comPorts[i];
                    break;
                }
            }
        }
        else {
            for (int i = 0; i < 100; i++) {
                int comIn = 0;
                for (int j = 0; j < 100; j++) {
                    if (strcmp(pastComPorts[j], comPorts[i]) == 0) {
                        comIn = 1;
                        break;
                    }
                }
                if (!comIn) {
                    for (int i = 0; i < 100; i++) {
                        strcpy(pastComPorts[i], "");
                    }
                    countComPorts = -1;
                    return pastComPorts[i];
                    break;
                }
            }
        }
    }

    countComPorts = CountValues;

    for (int i = 0; i < 100; i++) {
        strcpy(pastComPorts[i], comPorts[i]);
    }

    Sleep(10);

    return "";
}

int SetComPortAnimation(HDC dc) {
    static float textColor = 1;
    static int textColorVector = 1;
    static int countPoints = 0;
    static char comText[5][100] = { "Подключите один конец Micro-USB кабеля к роботу,", "а второй к вашему компьютеру.",
                            "Если устройство уже подключено, то отключите его,", "а потом подсоедените обратно.",
                            "Ожидание" };

    if (textColor < 2 && textColorVector == 1) textColor += 0.05;
    else if (textColorVector == 0 && textColor > 1.1) textColor -= 0.1;
    else if (textColorVector == 0) {
        textColor = 1;
        textColorVector = 1;
        strcpy(comText[4], "Ожидание");
        countPoints = 0;
        return 1;
    }


    SetTextColor(dc, RGB(intLimit(250 * textColor, 255), intLimit(192 * textColor, 255), intLimit(255 * (textColor - 1), 255)));

    SelectObject(dc, wifiSetupTextFont);
    SetBkColor(dc, RGB(250, 192, 0));

    TextOut(dc, 60, 20, comText[0], strlen(comText[0]));
    TextOut(dc, 150, 50, comText[1], strlen(comText[1]));
    TextOut(dc, 60, 80, comText[2], strlen(comText[2]));
    TextOut(dc, 150, 110, comText[3], strlen(comText[3]));

    //printf("%s\n", activeComPort);

    if (strlen(activeComPort)==0) strcpy(activeComPort, ShowCOMPorts());
    else cableConnected = 1;

    countPoints += 4;

    if(countPoints%100==0 && countPoints<400) strcat(comText[4], ".");
    else if (countPoints > 400) {
        strcpy(comText[4], "Ожидание");
        countPoints = 0;
    }

    SelectObject(dc, helloTextFont);
    TextOut(dc, 120, 200, comText[4], strlen(comText[4]));

    if (cableConnected && textColor>=2) {
        textColorVector = 0;
    }

    return 0;
}

int ShowNoComPortAni(HDC dc) {
    static float textColor = 1;
    static int textColorVector = 1;

    if (textColor < 2 && textColorVector == 1) textColor += 0.05;
    else if (textColorVector == 0 && textColor > 1.1) textColor -= 0.1;
    else if (textColorVector == 0) {
        textColor = 1;
        textColorVector = 1;

        connectComOk = -1;
        cableConnected = 0;
        strcpy(activeComPort, "");
        strcpy(wifiInformation[0], "");
        strcpy(wifiInformation[1], "");

        return 1;
    }


    SetTextColor(dc, RGB(intLimit(250 * textColor, 255), intLimit(192 * textColor, 255), intLimit(255 * (textColor - 1), 255)));

    static char noComText[5][100] = {"Что-то пошло не так,", "убедитесь, что вы используете", "нужное устройство.", "Заново"};

    SelectObject(dc, wifiSetupTextFont2);
    SetBkColor(dc, RGB(250, 192, 0));

    TextOut(dc, 120, 20, noComText[0], strlen(noComText[0]));
    TextOut(dc, 40, 60, noComText[1], strlen(noComText[1]));
    TextOut(dc, 130, 100, noComText[2], strlen(noComText[2]));

    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(intLimit(250 * textColor, 255), intLimit(192 * textColor, 255), intLimit(255 * (textColor - 1), 255)));

    SetTextColor(dc, RGB(250, 192, 0));
    SetBkColor(dc, RGB(intLimit(250 * textColor, 255), intLimit(192 * textColor, 255), intLimit(255 * (textColor - 1), 255)));
    SelectObject(dc, wifiSetupTextFont3);

    if (mouseCoords[0] > 200 && mouseCoords[1] > 205 && mouseCoords[0] < 370 && mouseCoords[1] < 245) {
        RoundRect(dc, 200, 205, 370, 245, 20, 20);
        TextOut(dc, 240, 210, noComText[3], strlen(noComText[3]));

        if (click) {
            textColorVector = 0;
            click = 0;
        }
    }
    else {
        RoundRect(dc, 200, 210, 370, 250, 20, 20);
        TextOut(dc, 240, 215, noComText[3], strlen(noComText[3]));
    }

    return 0;
}

int ShowCompleteAni(HDC dc) {
    static float textColor = 1;
    static int textColorVector = 1;

    if (textColor < 2 && textColorVector == 1) textColor += 0.05;
    else if (textColorVector == 0 && textColor > 1.1) textColor -= 0.1;
    else if (textColorVector == 0) return 1;


    SetTextColor(dc, RGB(intLimit(250 * textColor, 255), intLimit(192 * textColor, 255), intLimit(255 * (textColor - 1), 255)));

    static char CompleteText[5][100] = { "Настрока завершена!", "Вы можете отключить робота.", "Выход" };

    SelectObject(dc, wifiSetupTextFont2);
    SetBkColor(dc, RGB(250, 192, 0));

    TextOut(dc, 120, 60, CompleteText[0], strlen(CompleteText[0]));
    TextOut(dc, 50, 100, CompleteText[1], strlen(CompleteText[1]));

    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(intLimit(250 * textColor, 255), intLimit(192 * textColor, 255), intLimit(255 * (textColor - 1), 255)));

    SetTextColor(dc, RGB(250, 192, 0));
    SetBkColor(dc, RGB(intLimit(250 * textColor, 255), intLimit(192 * textColor, 255), intLimit(255 * (textColor - 1), 255)));
    SelectObject(dc, wifiSetupTextFont3);

    if (mouseCoords[0] > 200 && mouseCoords[1] > 205 && mouseCoords[0] < 370 && mouseCoords[1] < 245) {
        RoundRect(dc, 200, 205, 370, 245, 20, 20);
        TextOut(dc, 245, 210, CompleteText[2], strlen(CompleteText[2]));

        if (click) {
            textColorVector = 0;
            click = 0;
        }
    }
    else {
        RoundRect(dc, 200, 210, 370, 250, 20, 20);
        TextOut(dc, 245, 215, CompleteText[2], strlen(CompleteText[2]));
    }

    return 0;
}

int ShowRobotSetting(HDC dc) {
    static float textColor = 1, settingBarParam = 0;
    static int textColorVector = 1, connected = -1;

    if (settingBarParam < 1) settingBarParam += 0.01;

    if (textColor < 2 && textColorVector == 1) textColor += 0.05;
    else if (textColorVector == 0 && textColor > 1.1) textColor -= 0.1;
    else if (textColorVector == 0) {
        textColor = 1;
        settingBarParam = 0;
        textColorVector = 1;
        int pastConnected = connected;
        connected = -1;

        return pastConnected + 1;
    }

    SetTextColor(dc, RGB(intLimit(250 * textColor, 255), intLimit(192 * textColor, 255), intLimit(255 * (textColor - 1), 255)));

    static char settingText[5][100] = { "Робот подключён!", "Идет настройка, она продлится", "не дольше минуты." };

    SelectObject(dc, wifiSetupTextFont2);
    SetBkColor(dc, RGB(250, 192, 0));

    TextOut(dc, 140, 60, settingText[0], strlen(settingText[0]));
    TextOut(dc, 40, 100, settingText[1], strlen(settingText[1]));
    TextOut(dc, 135, 140, settingText[2], strlen(settingText[2]));

    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(20, 170, 50));

    Rectangle(dc, -1, 345, windowSize[0]*settingBarParam, 365);

    if (textColor >= 2 && connected == -1) {
        connected = espConnectMenager();
    }
    else if (connected != -1) {
        textColorVector = 0;
        settingBarParam = 1;
    }

    return 0;
}

int espConnectMenager() {
    char numPort[10] = "", sym[5];
    int portNumber = 0, integerNum = 0;

    for (int i = 0; i < 20; i++) {
        if (activeComPort[i] == 77) {
            integerNum = 1;
        }
        else if (integerNum) {
            sprintf(sym, "%c", activeComPort[i]);
            strcat(numPort, sym);
        }
    }

    portNumber = atoi(numPort);

    Sleep(200);

    HANDLE Port;
    Port = CreateFileW(portNames[portNumber], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (Port == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, "Произошла непредвиденная ошибка, пожалуйста, попробуйте еще раз.", "Ошибка!", MB_OK);
        exit(1);
    }
    DCB dcb;
    GetCommState(Port, &dcb);
    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(Port, &dcb);
    GetCommState(Port, &dcb);

    char activationKey[100] = "12KA9DS42349D92S";
    char message[200] = "";

    strcat(message, wifiInformation[1]);
    strcat(message, "#");
    strcat(message, wifiInformation[0]);
    strcat(message, "#");
    strcat(message, activationKey);

    unsigned char dst[5] = "";
    int size;

    for (int i = 0; i < strlen(message); i++) {
        sprintf(dst, "%c", message[i]);

        WriteFile(Port, dst, 1, &size, NULL);
    }

    unsigned char dst2[1024] = { 0 };

    unsigned long size2 = sizeof(dst2);
    ReadFile(Port, dst2, 1, &size2, 0);

    CloseHandle(Port);

    return strcmp(dst2, "1")==0;
}

void ClearDisplay(HDC dc) {
    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(250, 192, 0));
    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCPenColor(dc, RGB(250, 192, 0));

    Rectangle(dc, 0, 0, windowSize[0], windowSize[1]);
}

int WinShow(HDC dc) {
    HDC memDC = CreateCompatibleDC(dc);
    HBITMAP memBM = CreateCompatibleBitmap(dc, windowSize[0], windowSize[1]);
    SelectObject(memDC, memBM);

    ClearDisplay(memDC);

    static int partOfSetup = 0;

    if (partOfSetup == 0 && HelloTextAnimation(memDC)) partOfSetup = 1;
    else if (partOfSetup == 1 && SetupWifiAnimation(memDC)) partOfSetup = 2;
    else if (partOfSetup == 2 && SetComPortAnimation(memDC)) partOfSetup = 3;
    else if (partOfSetup == 3) partOfSetup += ShowRobotSetting(memDC);
    else if (partOfSetup == 4 && ShowNoComPortAni(memDC)) partOfSetup = 1;
    else if (partOfSetup == 5 && ShowCompleteAni(memDC)) exit(0);

    BitBlt(dc, 0, 300 * (nameWifiEdit!=0), windowSize[0], windowSize[1], memDC, 0, 300 * (nameWifiEdit != 0), SRCCOPY);
    DeleteDC(memDC);
    DeleteObject(memBM);
}

int WinMain() {
    WNDCLASSA wcl;
    memset(&wcl, 0, sizeof(WNDCLASSA));

    wcl.lpszClassName = "my Window";
    wcl.lpfnWndProc = WndProc;

    RegisterClassA(&wcl);

    mainHwnd = CreateWindow("my Window", "Robot Setup Program", WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX),
        (GetSystemMetrics(SM_CXSCREEN) >> 1) - (windowSize[0] >> 1), (GetSystemMetrics(SM_CYSCREEN) >> 1) - (windowSize[1] >> 1), windowSize[0], windowSize[1], NULL, NULL, NULL, NULL);

    ShowWindow(mainHwnd, SW_SHOWNORMAL);

    WinInit();

    HDC dc = GetDC(mainHwnd);

    MSG msg;

    while (1) {
        if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
            DispatchMessage(&msg);
            TranslateMessage(&msg);
            if (msg.message == WM_QUIT) break;
        }
        else {
            WinShow(dc);
            Sleep(1);
        }
    }
}