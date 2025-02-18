#pragma once
#include <exception>
#include <string>

namespace gst {
    namespace excp {
        class OutOfRange: public std::exception {
        public:
            OutOfRange(const char* msg) : message{msg} {}
            const char* what() const throw() {
                return ("CAL: Out of range error! " + message).c_str();
            }
        private:
            std::string message{};
        };
    }
}