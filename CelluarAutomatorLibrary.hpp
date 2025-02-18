#pragma once
#include <CAL_Exceptions.hpp>
#include <array>

namespace gtd {
    template <class T, size_t x_max, size_t y_max>
    class CelluarAutomator {
    public:
        CelluarAutomator() {
            if(x_max < 3) throw gst::excp::OutOfRange("x-axis value is too low for creating celluar");
            if(y_max < 3) throw gst::excp::OutOfRange("y-axis value is too low for creating celluar");
        }
        CellRaw operator[](size_t indx) {
            if(indx < 0) throw gst::excp::OutOfRange("Value is too low for this celluar");
            if(indx >= y_max) throw gst::excp::OutOfRange("Value is too big for this celluar");
            return array[indx];
        }
        ~CelluarAutomator() {}
    private:
        class CellRaw {
        public:
            CellRaw() {}
            T& operator[](size_t indx) {
                if(indx < 0) throw gst::excp::OutOfRange("Value is too low for this celluar raw");
                if(indx >= x_max) throw gst::excp::OutOfRange("Value is too big for this celluar raw");
                return array[indx];
            }
        private:
            std::array<T,x_max> raw{};
        };
        std::array<std::array<CellRaw,y_max> map{};
    };
}