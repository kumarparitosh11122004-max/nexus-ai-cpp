#include <algorithm>
#include <chrono>
#include <cctype>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
const std::string RESET = "\033[0m";
const std::string CYAN = "\033[96m";
const std::string BLUE = "\033[94m";
const std::string GREEN = "\033[92m";
const std::string YELLOW = "\033[93m";
const std::string RED = "\033[91m";
const std::string DIM = "\033[90m";

std::string trim(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return value;
}

bool contains(const std::string& text, const std::string& phrase) {
    return text.find(phrase) != std::string::npos;
}

std::string currentDateTime(const char* format) {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif
    std::ostringstream output;
    output << std::put_time(&localTime, format);
    return output.str();
}

void printLine(char ch = '-', int width = 62) {
    std::cout << DIM << std::string(width, ch) << RESET << '\n';
}

void printBanner() {
    std::cout << CYAN;
    printLine('=');
    std::cout << "   _   _ _____  __  __ _   _ ____  \n";
    std::cout << "  | \\ | | ____||  \\/  | | | / ___| \n";
    std::cout << "  |  \\| |  _|  | |\\/| | | | \\___ \\ \n";
    std::cout << "  | |\\  | |___ | |  | | |_| |___) |\n";
    std::cout << "  |_| \\_|_____||_|  |_|\\___/|____/ \n";
    std::cout << RESET;
    std::cout << GREEN << "              NEXUS AI  |  C++ CHATBOT" << RESET << '\n';
    std::cout << DIM << "          A lightweight conversational assistant" << RESET << '\n';
    printLine('=');
}

class Chatbot {
public:
    explicit Chatbot(const std::string& knowledgeFile,
                     const std::string& historyFile)
        : knowledgeFile_(knowledgeFile), historyFile_(historyFile) {
        loadKnowledge();
    }

    std::string reply(const std::string& input) {
        const std::string message = lower(trim(input));
        if (message.empty()) return "Please type a message so I can help.";

        if (message == "help" || message == "/help") {
            return "Available commands: /help, /time, /date, /history, "
                   "/teach QUESTION | ANSWER, /clear, /exit.";
        }
        if (message == "/time" || message == "time") {
            return "Current local time: " + currentDateTime("%I:%M:%S %p");
        }
        if (message == "/date" || message == "date") {
            return "Today's date: " + currentDateTime("%d %B %Y");
        }
        if (message == "/history") {
            return historySummary();
        }
        if (message.rfind("/teach ", 0) == 0) {
            return teach(input.substr(7));
        }
        if (message == "/clear") {
            return "CLEAR_SCREEN";
        }

        for (const auto& item : knowledge_) {
            if (contains(message, item.first)) return item.second;
        }

        if (contains(message, "hello") || contains(message, "hi") ||
            contains(message, "hey")) {
            return "Hello! I'm Nexus AI. What would you like to talk about?";
        }
        if (contains(message, "your name") || contains(message, "who are you")) {
            return "I'm Nexus AI, a C++ chatbot project built with C++17.";
        }
        if (contains(message, "how are you")) {
            return "I'm running smoothly! Thanks for asking. How can I help?";
        }
        if (contains(message, "thank")) {
            return "You're welcome! Feel free to ask me something else.";
        }
        if (contains(message, "bye") || contains(message, "goodbye")) {
            return "Goodbye! Thanks for chatting with Nexus AI.";
        }
        if (contains(message, "c++") || contains(message, "cpp")) {
            return "C++ is a general-purpose programming language known for "
                   "performance, control and object-oriented programming.";
        }
        if (contains(message, "github")) {
            return "GitHub is a platform for hosting code, tracking changes "
                   "with Git and collaborating on software projects.";
        }
        if (contains(message, "computer")) {
            return "A computer accepts input, processes data, stores information "
                   "and produces output.";
        }
        return "I don't know that one yet. You can teach me using:\n"
               "/teach question | answer\n"
               "For example: /teach capital of japan | Tokyo";
    }

    void saveMessage(const std::string& role, const std::string& message) const {
        std::ofstream file(historyFile_, std::ios::app);
        if (file) file << currentDateTime("%Y-%m-%d %H:%M:%S")
                       << " [" << role << "] " << message << '\n';
    }

    void clearHistory() const {
        std::ofstream file(historyFile_, std::ios::trunc);
    }

private:
    std::string knowledgeFile_;
    std::string historyFile_;
    std::map<std::string, std::string> knowledge_;

    void loadKnowledge() {
        std::ifstream file(knowledgeFile_);
        std::string line;
        while (std::getline(file, line)) {
            const auto separator = line.find('|');
            if (separator == std::string::npos) continue;
            const std::string question = lower(trim(line.substr(0, separator)));
            const std::string answer = trim(line.substr(separator + 1));
            if (!question.empty() && !answer.empty()) knowledge_[question] = answer;
        }
    }

    std::string teach(const std::string& data) {
        const auto separator = data.find('|');
        if (separator == std::string::npos) {
            return "Format: /teach QUESTION | ANSWER";
        }
        const std::string question = lower(trim(data.substr(0, separator)));
        const std::string answer = trim(data.substr(separator + 1));
        if (question.empty() || answer.empty()) {
            return "Both the question and answer are required.";
        }
        knowledge_[question] = answer;
        std::ofstream file(knowledgeFile_, std::ios::app);
        if (!file) return "I couldn't save the knowledge file. Check folder permissions.";
        file << question << '|' << answer << '\n';
        return "Knowledge saved! Ask me \"" + question + "\" next time.";
    }

    std::string historySummary() const {
        std::ifstream file(historyFile_);
        if (!file) return "No chat history found yet.";
        std::string line;
        int count = 0;
        while (std::getline(file, line)) ++count;
        return "Your history contains " + std::to_string(count) +
               " saved messages. Open data/chat_history.txt to read them.";
    }
};

void showHelp() {
    std::cout << YELLOW << "\n  QUICK COMMANDS\n" << RESET;
    std::cout << "  /help                         Show commands\n";
    std::cout << "  /time                         Show local time\n";
    std::cout << "  /date                         Show current date\n";
    std::cout << "  /history                      Count saved messages\n";
    std::cout << "  /teach question | answer      Teach a new response\n";
    std::cout << "  /clear                        Clear the terminal screen\n";
    std::cout << "  /exit                         Close Nexus AI\n\n";
}

void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
    printBanner();
}

} // namespace

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    Chatbot bot("data/knowledge.txt", "data/chat_history.txt");
    printBanner();
    std::cout << "  Type " << GREEN << "/help" << RESET
              << " to see commands, or " << GREEN << "/exit" << RESET
              << " to quit.\n";
    printLine();

    std::string input;
    while (true) {
        std::cout << BLUE << "\n  You > " << RESET;
        if (!std::getline(std::cin, input)) break;
        input = trim(input);
        if (input.empty()) continue;

        if (lower(input) == "/exit") {
            const std::string farewell = "Session closed. Have a great day!";
            std::cout << GREEN << "\n  Nexus > " << farewell << RESET << '\n';
            bot.saveMessage("You", input);
            bot.saveMessage("Nexus", farewell);
            break;
        }

        bot.saveMessage("You", input);
        if (lower(input) == "/clear") {
            clearScreen();
            continue;
        }

        const std::string answer = bot.reply(input);
        if (answer == "CLEAR_SCREEN") {
            clearScreen();
            continue;
        }

        std::cout << GREEN << "  Nexus > " << RESET << answer << '\n';
        bot.saveMessage("Nexus", answer);
    }

    std::cout << DIM << "\n  Thank you for using Nexus AI.\n" << RESET;
    return 0;
}
