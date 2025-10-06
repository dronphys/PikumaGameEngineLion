#include "Logger.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <windows.h>

std::vector<LogEntry> Logger::messages;

std::string CurrentDateTimeToString() {
    // Get current time
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    // Create a tm struct to hold the local time
    std::tm timeInfo{};
    localtime_s(&timeInfo, &now); // Safe version for MSVC

    // Format time into string
    std::string output(30, '\0');
    std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", &timeInfo);

    // Remove trailing null characters
    output.resize(std::strlen(output.c_str()));

    return output;
}


void Logger::Log(const std::string& message) {

    // LogEntry logEntry;
    // logEntry.type = LogType::LOG_INFO;
    // logEntry.message = "LOG: [" + CurrentDateTimeToString() + "] - " + message;
    // messages.push_back({ LogType::LOG_INFO, message });
    //
    // // ANSI CODES
    // std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << '\n';
}

void Logger::Err(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LogType::LOG_ERROR;
    logEntry.message = "ERR: [" + CurrentDateTimeToString() + "] - " + message;
    messages.push_back({ LogType::LOG_INFO, message });
    std::cout << "\x1B[91m" << logEntry.message << "\033[0m" << '\n';

}

void Logger::EnableANSIColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
