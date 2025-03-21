#pragma once
#include "CAL_Exceptions.hpp"
#include <stdexcept>
#include <utility>
#include <vector>
#include <string>
#include <array>
#include <cmath>
#include <map>
#include <set>

#define GTD_CA_RULE_ARG(T) std::pair<const T*,std::array<const T*,8>>
#define GTD_CA_NEIGHB_RULE(Size) std::array<std::pair<long long int,long long int>,Size>
#define GTD_CA_NEIGHB_PAIR std::pair<long long int,long long int>
#define GTD_CA_MAP(T,Y,X) std::array<std::array<T,X>,Y>
#define GTD_CA_ROW(T,X) std::array<T,X>

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
        class CellRow {
        public:
            CellRow() {}
            T& operator[](size_t indx) {
                if(indx < 0) throw gtd::excp::OutOfRange("Value is too low for this celluar row");
                if(indx >= x_max) throw gtd::excp::OutOfRange("Value is too big for this celluar row");
                return row[indx];
            }
            void operator=(std::array<T,x_max> _row) {
                row = _row;
            }
        private:
            std::array<T,x_max> row{};
        };
        CelluarAutomaton() : y_size{y_max}, x_size{x_max} {
            if(x_max < 3) throw gtd::excp::OutOfRange("x-axis value is too low for creating celluar");
            if(y_max < 3) throw gtd::excp::OutOfRange("y-axis value is too low for creating celluar");
        }
        CelluarAutomaton(T(*r_func)(std::pair<const T*,std::array<const T*,8>>)) : CelluarAutomaton() {
            rule_func =  r_func;
        }
        CelluarAutomaton(T tmpl) : CelluarAutomaton() {
            for(size_t y{}; y < y_size; ++y) for(size_t x{}; x < x_size; ++x) map[y][x] = tmpl;
        }
        CelluarAutomaton(gtd::CelluarAutomaton<T,y_max,x_max>& autom) : CelluarAutomaton() {
            size_t i{};
            for(CellRow arr : autom.map) map[i++] = arr;
            rule_func =  autom.rule_func;
        }
        CelluarAutomaton(T tmpl, T(*r_func)(std::pair<const T*,std::array<const T*,8>>)) : CelluarAutomaton(tmpl) {
            rule_func =  r_func;
        }
        void step() {
            if(rule_func == nullptr) return;
            std::vector<HCell> h_map{};
            for(size_t y{}; y < y_size; ++y) {
                for(size_t x{}; x < x_size; ++x) {
                    T new_value{};
                    if(try_catch_rule) {
                        try {
                            new_value = rule_func(prepare_dots(y,x));
                        } catch(const std::exception& e) {
                            throw gtd::excp::InvalidRule(e.what());
                        }
                    } else {
                        new_value = rule_func(prepare_dots(y,x));
                    }
                    if(new_value != map[y][x]) h_map.emplace_back(y,x,new_value);
                }
            }
            for(auto h_cell : h_map) map[h_cell.y][h_cell.x] = h_cell.value;
        }
        void step(T(*r_func)(std::pair<const T*,std::array<const T*,8>>)) {
            std::vector<HCell> h_map{};
            for(size_t y{}; y < y_size; ++y) {
                for(size_t x{}; x < x_size; ++x) {
                    T new_value{};
                    if(try_catch_rule) {
                        try {
                            new_value = r_func(prepare_dots(y,x));
                        } catch(const std::exception& e) {
                            throw gtd::excp::InvalidRule(e.what());
                        }
                    } else {
                        new_value = r_func(prepare_dots(y,x));
                    }
                    if(new_value != map[y][x]) h_map.emplace_back(y,x,new_value);
                }
            }
            for(auto h_cell : h_map) map[h_cell.y][h_cell.x] = h_cell.value;
        }
        template <size_t Arr_Size>
        void step(T(*r_func)(std::pair<const T*,std::array<const T*,Arr_Size>>)) {
            long long int layers = (long long int)rule_arg_arr_layers(Arr_Size);
            if(layers == 0ull) throw gtd::excp::InvalidRule("Invalid neighbours number");
            std::vector<HCell> h_map{};
            for(size_t y{}; y < y_size; ++y) {
                for(size_t x{}; x < x_size; ++x) {
                    T new_value{};
                    if(try_catch_rule) {
                        try {
                            new_value = r_func(prepare_dots<Arr_Size>(y,x,layers));
                        } catch(const std::exception& e) {
                            throw gtd::excp::InvalidRule(e.what());
                        }
                    } else {
                        new_value = r_func(prepare_dots<Arr_Size>(y,x,layers));
                    }
                    if(new_value != map[y][x]) h_map.emplace_back(y,x,new_value);
                }
            }
            for(auto h_cell : h_map) map[h_cell.y][h_cell.x] = h_cell.value;
        }
        template <size_t Arr_Size>
        void step(T(*r_func)(std::pair<const T*,std::array<const T*,Arr_Size>>), const GTD_CA_NEIGHB_RULE(Arr_Size)& neighb_rule) {
            if(Arr_Size == 0ull) throw gtd::excp::InvalidRule("Too low number of neighbours");
            std::vector<HCell> h_map{};
            for(size_t y{}; y < y_size; ++y) {
                for(size_t x{}; x < x_size; ++x) {
                    T new_value{};
                    if(try_catch_rule) {
                        try {
                            new_value = r_func(prepare_dots(y,x,neighb_rule));
                        } catch(const std::exception& e) {
                            throw gtd::excp::InvalidRule(e.what());
                        }
                    } else {
                        new_value = r_func(prepare_dots(y,x,neighb_rule));
                    }
                    if(new_value != map[y][x]) h_map.emplace_back(y,x,new_value);
                }
            }
            for(auto h_cell : h_map) map[h_cell.y][h_cell.x] = h_cell.value;
        }
        void step(std::string str) {}
        CellRow& operator[](size_t indx) {
            if(indx < 0) throw gtd::excp::OutOfRange("Value is too low for this celluar");
            if(indx >= y_size) throw gtd::excp::OutOfRange("Value is too big for this celluar");
            return map[indx];
        }
        void draw() {}
        std::string draw(std::string(*r_func)(T)) {
            std::string output{};
            for(size_t y{}; y < y_size; ++y) {
                for(size_t x{}; x < x_size; ++x) {
                    output += r_func(map[y][x]);
                }
                output += '\n';
            }
            return output;
        }
        void get_map(const gtd::CelluarAutomaton<T,y_max,x_max>& autom) {
            size_t i{};
            for(CellRow arr : autom.map) map[i++] = arr;
        }
        void get_map(std::array<std::array<T,x_max>,y_max>& _map) {
            size_t i{};
            for(std::array<T,x_max>& arr : _map) map[i++] = arr;
        }
        gtd::CelluarAutomaton<T,y_max,x_max>& operator=(const gtd::CelluarAutomaton<T,y_max,x_max>& autom) {
            size_t i{};
            for(CellRow arr : autom.map) map[i++] = arr;
            rule_func = autom.rule_func;
            return *this;
        }
        bool operator==(gtd::CelluarAutomaton<T,x_max,y_max>& cell) {
            for(size_t y{}; y < y_size; ++y) 
                for(size_t x{}; x < x_size; ++x) 
                    if(map[y][x] != cell.map[y][x]) return false;
            return true;
        }
        bool operator!=(gtd::CelluarAutomaton<T,x_max,y_max>& cell) {
            for(size_t y{}; y < y_size; ++y) 
                for(size_t x{}; x < x_size; ++x) 
                    if(map[y][x] != cell.map[y][x]) return true;
            return false;
        }
        void change_rule(T(*ptr)(std::pair<const T*,std::array<const T*,8>>)) {
            rule_func = ptr;
        }
        bool endless_map = false;
        bool try_catch_rule = true;
        const size_t y_size{};
        const size_t x_size{};
        static size_t rule_arg_arr_size(size_t layers) {
            return std::pow((layers*2ull)+1ull,2)-1.;
        }
        static size_t rule_arg_arr_layers(size_t arr_size) {
            if(available_arg_sizes_to_layers.count(arr_size) > 0) return available_arg_sizes_to_layers[arr_size];
            else {
                size_t checking_layers = std::floor((std::floor(std::sqrt(arr_size+1ull))-1u)/2);
                if(rule_arg_arr_size(checking_layers) == arr_size) {
                    available_arg_sizes_to_layers.insert({arr_size, checking_layers});
                    return checking_layers;
                }
            }
            return 0ull;
        }
        ~CelluarAutomaton() {}
    private:
        std::array<CellRow,y_max> map{};
        T(*rule_func)(std::pair<const T*,std::array<const T*,8>>) = nullptr;
        std::pair<const T*,std::array<const T*,8>> prepare_dots(size_t y, size_t x) {
            std::array<const T*,8> neighbour_cells{};
            unsigned short int i{};
            for(long long int ly = -1ll+static_cast<long long int>(y); ly <= 1ll+static_cast<long long int>(y); ++ly) {
                for(long long int lx = -1ll+static_cast<long long int>(x); lx <= 1ll+static_cast<long long int>(x); ++lx) {
                    if(ly == y && lx == x) continue;
                    if(endless_map) {
                        size_t n_ly{}; size_t n_lx{};
                        if(ly < 0ll) n_ly = y_size-1ull;
                        else if(ly >= y_size) n_ly = 0ull;
                        else n_ly = ly;
                        if(lx < 0ll) n_lx = x_size-1ull;
                        else if(lx >= x_size) n_lx = 0ull;
                        else n_lx = lx;
                        neighbour_cells[i++] = &(map[n_ly][n_lx]);
                    } else {
                        if(ly >= 0 && ly < y_size && lx >= 0 && lx < x_size) neighbour_cells[i++] = &(map[ly][lx]);
                        else neighbour_cells[i++] = nullptr;
                    }
                }
            }
            return std::make_pair<const T*,std::array<const T*,8>>(&(map[y][x]),std::move(neighbour_cells));
        }
        template <size_t ArrSize>
        std::pair<const T*,std::array<const T*,ArrSize>> prepare_dots(size_t y, size_t x, long long int layers) {
            std::array<const T*,ArrSize> neighbour_cells{};
            unsigned long long int i{};
            for(long long int ly = -layers+static_cast<long long int>(y); ly <= layers+static_cast<long long int>(y); ++ly) {
                for(long long int lx = -layers+static_cast<long long int>(x); lx <= layers+static_cast<long long int>(x); ++lx) {
                    if(ly == y && lx == x) continue;
                    if(endless_map) {
                        size_t n_ly{}; size_t n_lx{};
                        if(ly < 0ll) n_ly = y_size+ly;
                        else if(ly >= y_size) n_ly = ly-y_size;
                        if(lx < 0ll) n_lx = x_size+lx;
                        else if(lx >= x_size) n_lx = lx-x_size;
                        neighbour_cells[i++] = &(map[n_ly][n_lx]);
                    } else {
                        if(ly >= 0 && ly < y_size && lx >= 0 && lx < x_size) neighbour_cells[i++] = &(map[ly][lx]);
                        else neighbour_cells[i++] = nullptr;
                    }
                }
            }
            return std::make_pair<const T*,std::array<const T*,ArrSize>>(&(map[y][x]),std::move(neighbour_cells));
        }
        template <size_t ArrSize>
        std::pair<const T*,std::array<const T*,ArrSize>> prepare_dots(const size_t y, const size_t x, const GTD_CA_NEIGHB_RULE(ArrSize)& neighb_rule) {
            std::array<const T*,ArrSize> neighbour_cells{};
            unsigned long long int i{};
            for(std::pair<long long int,long long int> p : neighb_rule) {
                long long int ly = p.first+static_cast<long long int>(y);
                long long int lx = p.second+static_cast<long long int>(y);
                if(ly == y && lx == x) throw gtd::excp::InvalidRule("Cell itself is in array of neighbours");
                if(endless_map) {
                    size_t n_ly{}; size_t n_lx{};
                    if(ly < 0ll) n_ly = y_size+ly;
                    else if(ly >= y_size) n_ly = ly-y_size;
                    if(lx < 0ll) n_lx = x_size+lx;
                    else if(lx >= x_size) n_lx = lx-x_size;
                    if(ly >= 0 && ly < y_size && lx >= 0 && lx < x_size) neighbour_cells[i++] = &(map[ly][lx]);
                    else neighbour_cells[i++] = throw gtd::excp::InvalidRule("Incorrect neighbour rule");
                } else {
                    if(ly >= 0 && ly < y_size && lx >= 0 && lx < x_size) neighbour_cells[i++] = &(map[ly][lx]);
                    else neighbour_cells[i++] = nullptr;
                }
            }
            return std::make_pair<const T*,std::array<const T*,ArrSize>>(&(map[y][x]),std::move(neighbour_cells));
        }
        static inline std::map<size_t,size_t> available_arg_sizes_to_layers{};
    };
}