#include <string>
#include <iostream>
#include <fstream>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class LogCommandCons : public LogCommand {
public:
    void print(const std::string& message) override {
        std::cout << message << " in console" << std::endl;
    }
};

class LogCommandFile : public LogCommand {
private:
    std::string filePath;

public:
    explicit LogCommandFile(const std::string& path)
        : filePath(path) {}

    void print(const std::string& message) override {
        std::ofstream file(filePath, std::ios::app); // добавление в файл
        if (file.is_open()) {
            file << message << " in file" << std::endl;
        }
    }
};

void print(LogCommand& command) {
    command.print("Test log message");
}

int main() {
    LogCommandCons consoleCmd;
    LogCommandFile fileCmd("log.txt");

    print(consoleCmd);
    print(fileCmd);

    return 0;
}