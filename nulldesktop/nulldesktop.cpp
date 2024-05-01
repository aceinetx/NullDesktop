#include "include.h"
#include "volumeControl.hpp"
#define CLOSE_EXPLORER_EXE 1

#pragma comment(lib, "user32")

HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO info;

void ConsoleThread() {
    system("cmd");
    exit(0);
}

void SetCursorPosition(int x, int y) {
    GetConsoleScreenBufferInfo(hStd, &info);

    COORD newCursor;
    newCursor.X = x;
    newCursor.Y = y;

    SetConsoleCursorPosition(hStd, newCursor);
}

void SetPreviousCursorPosition() {
    COORD newCursor;
    newCursor.X = info.dwCursorPosition.X;
    newCursor.Y = info.dwCursorPosition.Y;

    SetConsoleCursorPosition(hStd, newCursor);
}

std::string GetConsoleCharacters(int x, int y, int length) {
    std::string result;
    for (int i = x; i < x + length; i++) {
        wchar_t x;
        COORD currentCoord;
        currentCoord.X = i;
        currentCoord.Y = y;
        DWORD dwChars;
        ReadConsoleOutputCharacterW(hStd, &x, 1, currentCoord, &dwChars);
        result.push_back(x);
    }
    return result;
}

int main()
{
    volumeControl::InitDevice();
    if ((int)volumeControl::GetCurrentVolume() % 2 != 0) {
        volumeControl::SetCurrentVolume(20);
    }

    SendMessage(::GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000); // Make the console fullscreened
    std::cout << "Null desktop v1.0\n";

    if (CLOSE_EXPLORER_EXE == 1) {
        system("taskkill /f /im explorer.exe");
    }
    
    std::thread console_thread(ConsoleThread); // Start console
    console_thread.detach();

    while(1){
        if (GetAsyncKeyState(VK_VOLUME_UP) & 1) {
            float current_volume;
            current_volume = volumeControl::GetCurrentVolume();
            
            if ((int)current_volume <= 98) {
                volumeControl::SetCurrentVolume((float)((int)current_volume+2));
            }
            
            SetCursorPosition(0, 0);

            SetConsoleTextAttribute(hStd, 0x70);

            current_volume = volumeControl::GetCurrentVolume();
            std::string volume_string;
            if (current_volume < 100) volume_string.push_back(' ');
            if (current_volume < 10) volume_string.push_back(' ');
            volume_string.append(std::format("{}", (int)current_volume));
            std::cout << "Volume up   (" << volume_string << ")";
            // Volume up   (  3)
            // 17 chars


            SetConsoleTextAttribute(hStd, info.wAttributes);

            SetPreviousCursorPosition();

        }
        if (GetAsyncKeyState(VK_VOLUME_DOWN) & 1) {
            float current_volume;
            current_volume = volumeControl::GetCurrentVolume();

            if ((int)current_volume >= 4) {
                volumeControl::SetCurrentVolume((float)((int)current_volume - 2));
            }

            SetCursorPosition(0, 0);

            SetConsoleTextAttribute(hStd, 0x70);

            current_volume = volumeControl::GetCurrentVolume();
            std::string volume_string;
            if (current_volume < 100) volume_string.push_back(' ');
            if (current_volume < 10) volume_string.push_back(' ');
            volume_string.append(std::format("{}", (int)current_volume));
            std::cout << "Volume down (" << volume_string << ")";

            SetPreviousCursorPosition();

            SetConsoleTextAttribute(hStd, info.wAttributes);
        }
        if (GetAsyncKeyState(VK_VOLUME_MUTE) & 1) {
            SetCursorPosition(0, 0);

            SetConsoleTextAttribute(hStd, 0x70);

            volumeControl::Mute(!volumeControl::IsMuted());
            if (volumeControl::IsMuted()) {
                std::cout << "Volume muted     ";
            }
            else {
                std::cout << "Volume unmuted   ";
            }

            SetPreviousCursorPosition();

            SetConsoleTextAttribute(hStd, info.wAttributes);
        }
    }
}