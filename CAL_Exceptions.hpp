#pragma once
#include <exception>
#include <string>

namespace gst {
    namespace excp {
        class OutOfRange: public std::exception {
        public:
            OutOfRange(const char* msg) : message{msg} {}
            const char* what() const throw() {
                return (std::string{"CAL: Out of range error! "} + message).c_str();
            }
        private:
            std::string message{};
        };
        class InvalidRule: public std::exception {
        public:
            InvalidRule(const char* msg) : message{msg} {}
                const char* what() const throw() {
                    return (std::string{"CAL: Current step's rule is invalid! "} + message).c_str();
                }
        private:
            std::string message{};
        };
        class InvalidRuleFunction: public std::exception {
        public:
            InvalidRuleFunction(const char* msg) : message{msg} {}
                const char* what() const throw() {
                    return (std::string{"CAL: Your rule-function is invalid! "} + message).c_str();
                }
        private:
            std::string message{};
        };
    }
}