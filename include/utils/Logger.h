#pragma once
#include <cmath>
#include <iostream>
#include <string>

namespace ncompiler {
    struct Logger {
        bool isDebug = false;

        explicit Logger(bool debug) : isDebug(debug) {}

        void info(const std::string &message) {
            log(message);
        }

        void debug(const std::string &message) {
            if (isDebug)
                log(message);
        }

        void error(const std::string &message) {
            std::cout << "error: " << std::endl;
            log(message);
        } 
        
    private:
        void log(const std::string &message) { // NOLINT(*-convert-member-functions-to-static)
            std::cout << message << std::endl;
        }
    };
}
