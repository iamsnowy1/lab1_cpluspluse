#include <windows.h>
#include <iostream>
#include <string>

void StartProcess(const std::wstring& applicationName, const std::wstring& arguments) {
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };

    std::wstring commandLine = applicationName + L" " + arguments;

    // Створення процесу
    if (!CreateProcessW(
        nullptr,                   // Ім'я програми
        &commandLine[0],           // Аргументи командного рядка
        nullptr,                   // Атрибути безпеки процесу
        nullptr,                   // Атрибути безпеки потоку
        FALSE,                     // Успадкування дескрипторів
        0,                         // Прапори створення
        nullptr,                   // Середовище
        nullptr,                   // Поточний каталог
        &si,                       // Інформація про запуск
        &pi                        // Інформація про процес
    )) {
        std::cerr << "Не вдалося створити процес. Код помилки: " << GetLastError() << std::endl;
        return;
    }

    std::cout << "Процес успішно запущений. PID: " << pi.dwProcessId << std::endl;

    // Очікування завершення процесу або тайм-ауту
    DWORD waitResult = WaitForSingleObject(pi.hProcess, 10000); // Очікування до 10 секунд

    if (waitResult == WAIT_TIMEOUT) {
        std::cout << "Процес виконується занадто довго. Завершення..." << std::endl;
        if (!TerminateProcess(pi.hProcess, 1)) {
            std::cerr << "Не вдалося завершити процес. Код помилки: " << GetLastError() << std::endl;
        }
        else {
            std::cout << "Процес успішно завершений." << std::endl;
        }
    }
    else if (waitResult == WAIT_OBJECT_0) {
        DWORD exitCode;
        if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
            std::cout << "Процес завершено. Код завершення: " << exitCode << std::endl;
        }
        else {
            std::cerr << "Не вдалося отримати код завершення. Код помилки: " << GetLastError() << std::endl;
        }
    }
    else {
        std::cerr << "Несподіваний результат очікування. Код помилки: " << GetLastError() << std::endl;
    }

    // Закриття дескрипторів
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    std::wstring applicationName = L"notepad.exe"; // Замініть на іншу програму за необхідності
    std::wstring arguments = L""; // Додайте аргументи за потреби

    StartProcess(applicationName, arguments);

    return 0;
}
