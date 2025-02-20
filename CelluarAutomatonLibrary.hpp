#pragma once
#include "CAL_Exceptions.hpp"
#include <stdexcept>
#include <string>
#include <vector>
#include <array>

#define GTD_CA_RULE_ARG(T) std::pair<T*,std::array<T*,8>>

namespace gtd {
    template <class T, size_t y_max, size_t x_max>
    class CelluarAutomaton {
    public:
        class HCell {
        public:
            HCell(size_t _y, size_t _x, T _value) : y{_y}, x{_x}, value{_value} {}
            size_t y{}; size_t x{};
            T value{};
        };
        class CellRaw {
        public:
            CellRaw() {}
            T& operator[](size_t indx) {
                if(indx < 0) throw gst::excp::OutOfRange("Value is too low for this celluar raw");
                if(indx >= x_max) throw gst::excp::OutOfRange("Value is too big for this celluar raw");
                return raw[indx];
            }
        private:
            std::array<T,x_max> raw{};
        };
        CelluarAutomaton() {
            if(x_max < 3) throw gst::excp::OutOfRange("x-axis value is too low for creating celluar");
            if(y_max < 3) throw gst::excp::OutOfRange("y-axis value is too low for creating celluar");
        }
        CelluarAutomaton(T(*r_func)(std::pair<T*,std::array<T*,8>>)) : rule_func{r_func} {
            if(x_max < 3) throw gst::excp::OutOfRange("x-axis value is too low for creating celluar");
            if(y_max < 3) throw gst::excp::OutOfRange("y-axis value is too low for creating celluar");
        }
        void step() {
            if(rule_func == nullptr) return;
            std::vector<HCell> h_map{};
            for(size_t y{}; y < y_max; ++y) {
                for(size_t x{}; x < x_max; ++x) {
                    T new_value{};
                    if(try_catch_rule) {
                        try {
                            new_value = rule_func(prepare_dots(y,x));
                        } catch(const std::exception& e) {
                            //throw gst::excp::InvalidRule(e.what());
                        }
                    } else {
                        new_value = rule_func(prepare_dots(y,x));
                    }
                    if(new_value != map[y][x]) h_map.emplace_back(y,x,new_value);
                }
            }
            for(auto h_cell : h_map) map[h_cell.y][h_cell.x] = h_cell.value;
        }
        void step(T(*r_func)(std::pair<T*,std::array<T*,8>>)) {
            std::vector<HCell> h_map{};
            for(size_t y{}; y < y_max; ++y) {
                for(size_t x{}; x < x_max; ++x) {
                    T new_value{};
                    if(try_catch_rule) {
                        try {
                            new_value = r_func(prepare_dots(y,x));
                        } catch(const std::exception& e) {
                            //throw gtd::excp::InvalidRule(e.what());
                        }
                    } else {
                        new_value = r_func(prepare_dots(y,x));
                    }
                    if(new_value != map[y][x]) h_map.emplace_back(y,x,new_value);
                }
            }
            for(auto h_cell : h_map) map[h_cell.y][h_cell.x] = h_cell.value;
        }
        void step(std::string str) {}
        CellRaw& operator[](size_t indx) {
            if(indx < 0) throw gst::excp::OutOfRange("Value is too low for this celluar");
            if(indx >= y_max) throw gst::excp::OutOfRange("Value is too big for this celluar");
            return map[indx];
        }
        void draw() {}
        std::string draw(std::string(*r_func)(T)) {
            std::string output{};
            for(size_t y{}; y < y_max; ++y) {
                for(size_t x{}; x < x_max; ++x) {
                    output += r_func(map[y][x]);
                }
                output += '\n';
            }
            return output;
        }
        bool operator==(gtd::CelluarAutomaton<T,x_max,y_max>& cell) {
            for(size_t y{}; y < y_max; ++y) 
                for(size_t x{}; x < x_max; ++x) 
                    if(map[y][x] != cell.map[y][x]) return false;
            return true;
        }
        bool operator!=(gtd::CelluarAutomaton<T,x_max,y_max>& cell) {
            for(size_t y{}; y < y_max; ++y) 
                for(size_t x{}; x < x_max; ++x) 
                    if(map[y][x] != cell.map[y][x]) return true;
            return false;
        }
        void change_rule(T(*ptr)(std::pair<T*,std::vector<T*>>)) {
            rule_func = ptr;
        }
        bool endless_map = false;
        bool try_catch_rule = true;
        ~CelluarAutomaton() {}
    private:
        std::array<CellRaw,y_max> map{};
        T(*rule_func)(std::pair<T*,std::array<T*,8>>) = nullptr;
        std::pair<T*,std::array<T*,8>> prepare_dots(size_t y, size_t x) {
            std::array<T*,8> neighbour_cells{};
            unsigned short int i{};
            for(long long int ly = -1ll+(long long int)y; ly <= 1ll+(long long int)y; ++ly) {
                for(long long int lx = -1ll+(long long int)x; lx <= 1ll+(long long int)x; ++lx) {
                    if(ly == y && lx == x) continue;
                    if(endless_map) {
                        if(ly < 0) ly = y_max-1;
                        if(ly == y_max) ly = 0ll;
                        if(lx < 0) lx = x_max-1;
                        if(lx == x_max) lx = 0ll;
                        neighbour_cells[i++] = &(map[ly][lx]);
                    } else {
                        if(ly >= 0 && ly < y_max && lx >= 0 && lx < x_max) neighbour_cells[i++] = &(map[ly][lx]);
                        else neighbour_cells[i++] = nullptr;
                    }
                }
            }
            return std::make_pair(&(map[y][x]),neighbour_cells);
        }
    };

    template <class T>
    T standart_rule(std::pair<T*,std::array<T*,8>>) {}
}