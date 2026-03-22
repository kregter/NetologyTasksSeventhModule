#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

class Observer {
public:
    virtual ~Observer() = default;

    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class Observable {
public:
    void addObserver(Observer* obs) {
        observers.push_back(obs);
    }
    void removeObserver(Observer* obs) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), obs),
            observers.end()
        );
    }

    void warning(const std::string& message) const {
        for (auto* obs : observers) {
            if (obs) obs->onWarning(message);
        }
    }
    void error(const std::string& message) const {
        for (auto* obs : observers) {
            if (obs) obs->onError(message);
        }
    }
    void fatalError(const std::string& message) const {
        for (auto* obs : observers) {
            if (obs) obs->onFatalError(message);
        }
    }

private:
    mutable std::vector<Observer*> observers;
};

class ObservableObserver : public Observer {
public:
    explicit ObservableObserver(Observable& obs)
        : observable(obs) {
        observable.addObserver(this);
    }

    virtual ~ObservableObserver() {
        observable.removeObserver(this);
    }

protected:
    Observable& observable;
};

class WarningObserver : public ObservableObserver {
public:
    using ObservableObserver::ObservableObserver;

    void onWarning(const std::string& message) override {
        std::cout << "WARNING: " << message << std::endl;
    }
};

class ErrorObserver : public ObservableObserver {
public:
    ErrorObserver(Observable& obs, const std::string& path)
        : ObservableObserver(obs), filePath(path) {}

    void onError(const std::string& message) override {
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) {
            file << "ERROR: " << message << std::endl;
        }
    }

private:
    std::string filePath;
};

class FatalErrorObserver : public ObservableObserver {
public:
    FatalErrorObserver(Observable& obs, const std::string& path)
        : ObservableObserver(obs), filePath(path) {}

    void onFatalError(const std::string& message) override {
        std::cout << "FATAL: " << message << std::endl;
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) {
            file << "FATAL: " << message << std::endl;
        }
    }

private:
    std::string filePath;
};

int main() {
    Observable logger;

    WarningObserver w(logger);
    ErrorObserver e(logger, "errors.txt");
    FatalErrorObserver f(logger, "fatal.txt");

    logger.warning("Warning message test");
    logger.error("Error message test");
    logger.fatalError("Fatal message test");

    return 0;
}