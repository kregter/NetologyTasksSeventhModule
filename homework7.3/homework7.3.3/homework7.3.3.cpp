#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

enum class Type {
    Warning,
    Error,
    FatalError,
    Unknown
};

class LogMessage {
public:
    LogMessage(Type t, std::string msg)
        : type_(t), message_(std::move(msg)) {}

    Type type() const {
        return type_;
    }

    const std::string& message() const {
        return message_;
    }

private:
    Type type_;
    std::string message_;
};

class Handler {
public:
    virtual ~Handler() = default;

    void setNext(std::shared_ptr<Handler> next) {
        next_ = next;
    }

    void handle(const LogMessage& msg) {
        if (!process(msg) && next_) {
            next_->handle(msg);
        }
    }

protected:
    virtual bool process(const LogMessage& msg) = 0;

private:
    std::shared_ptr<Handler> next_;
};

class WarningHandler : public Handler {
protected:
    bool process(const LogMessage& msg) override {
        if (msg.type() == Type::Warning) {
            std::cout << "WARNING: " << msg.message() << std::endl;
            return true;
        }
        return false;
    }
};

class ErrorHandler : public Handler {
public:
    explicit ErrorHandler(const std::string& path)
        : filePath(path) {}

protected:
    bool process(const LogMessage& msg) override {
        if (msg.type() == Type::Error) {
            std::ofstream file(filePath, std::ios::app);
            if (file.is_open()) {
                file << "ERROR: " << msg.message() << std::endl;
            }
            return true;
        }
        return false;
    }

private:
    std::string filePath;
};

class FatalErrorHandler : public Handler {
protected:
    bool process(const LogMessage& msg) override {
        if (msg.type() == Type::FatalError) {
            throw std::runtime_error("FATAL: " + msg.message());
        }
        return false;
    }
};

class UnknownHandler : public Handler {
protected:
    bool process(const LogMessage& msg) override {
        if (msg.type() == Type::Unknown) {
            throw std::runtime_error(
                "Unknown message: " + msg.message()
            );
        }
        return false;
    }
};

int main() {
    auto fatal = std::make_shared<FatalErrorHandler>();
    auto error = std::make_shared<ErrorHandler>("errors.txt");
    auto warning = std::make_shared<WarningHandler>();
    auto unknown = std::make_shared<UnknownHandler>();

    fatal->setNext(error);
    error->setNext(warning);
    warning->setNext(unknown);

    try {
        fatal->handle(LogMessage(Type::Warning, "Warning message test"));
        fatal->handle(LogMessage(Type::Error, "Error message test"));
        fatal->handle(LogMessage(Type::FatalError, "Fatal message test"));
        fatal->handle(LogMessage(Type::Unknown, "Unknown message test"));
    }
    catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}