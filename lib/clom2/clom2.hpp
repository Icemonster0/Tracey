/**
 * MIT License
 *
 * Copyright (c) 2023, 2024 András Borsányi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CLOM2_H
#define CLOM2_H

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>

#define CLOM2_SET_ARGS(c, v) clom2::args.reserve(c); for (int i = 0; i < c; ++i) clom2::args.emplace_back(v[i]);

#define CLOM2_CHECK_FOR_HELP_BEGIN(flag, alt_flag) \
clom2::help_flag = #flag; \
clom2::alt_help_flag = #alt_flag; \
clom2::check_for_help = clom2::find_flag_value(#flag, #alt_flag); \
if (clom2::check_for_help) \
    std::cout << "Available options:\nprimary-name, alternate-name (type) ['default-value']: description\n\n";

#define CLOM2_CHECK_FOR_HELP_END() \
if (clom2::check_for_help) { \
    std::cout << clom2::help_flag << ", " << clom2::alt_help_flag << " (flag): Display this help and exit" << '\n'; \
    exit(0); \
} \
clom2::check_for_help = false;

#define CLOM2_GENERAL_SETTING(var_name, prim_name, alt_name, T, value, hint, converter) \
T var_name = converter(clom2::find_setting_value(#prim_name, #alt_name, #value)); \
std::string var_name##_hint = clom2::process_hint(#hint, #prim_name", "#alt_name" ("#T") ['"#value"']: "#hint);

#define CLOM2_SETTING_STRING(var_name, prim_name, alt_name, value, hint) CLOM2_GENERAL_SETTING(var_name, prim_name, alt_name, std::string, value, hint, std::string)
#define CLOM2_SETTING_STRING_VEC(var_name, prim_name, alt_name, value, hint) CLOM2_GENERAL_SETTING(var_name, prim_name, alt_name, std::vector<std::string>, value, hint, clom2::str_to_str_vec)
#define CLOM2_SETTING_INT(var_name, prim_name, alt_name, value, hint) CLOM2_GENERAL_SETTING(var_name, prim_name, alt_name, int, value, hint, clom2::str_to_int)
#define CLOM2_SETTING_INT_VEC(var_name, prim_name, alt_name, value, hint) CLOM2_GENERAL_SETTING(var_name, prim_name, alt_name, std::vector<int>, value, hint, clom2::str_to_int_vec)
#define CLOM2_SETTING_FLOAT(var_name, prim_name, alt_name, value, hint) CLOM2_GENERAL_SETTING(var_name, prim_name, alt_name, float, value, hint, clom2::str_to_float)
#define CLOM2_SETTING_FLOAT_VEC(var_name, prim_name, alt_name, value, hint) CLOM2_GENERAL_SETTING(var_name, prim_name, alt_name, std::vector<float>, value, hint, clom2::str_to_float_vec)
#define CLOM2_SETTING_DOUBLE(var_name, prim_name, alt_name, value, hint) CLOM2_GENERAL_SETTING(var_name, prim_name, alt_name, double, value, hint, clom2::str_to_double)
#define CLOM2_SETTING_DOUBLE_VEC(var_name, prim_name, alt_name, value, hint) CLOM2_GENERAL_SETTING(var_name, prim_name, alt_name, std::vector<double>, value, hint, clom2::str_to_double_vec)

#define CLOM2_FLAG(var_name, prim_name, alt_name, hint) \
bool var_name = clom2::find_flag_value(#prim_name, #alt_name); \
std::string var_name##_hint = clom2::process_hint(#hint, #prim_name", "#alt_name" (flag): "#hint);

namespace clom2 {
    extern std::vector<std::string> args;
    extern bool check_for_help;
    extern std::string help_flag, alt_help_flag;

    std::string find_setting_value(std::string prim_name, std::string alt_name, std::string default_value);
    bool find_flag_value(std::string prim_name, std::string alt_name);
    std::string process_hint(std::string hint, std::string message);
    std::vector<std::string> str_to_str_vec(std::string str);
    int str_to_int(std::string str);
    std::vector<int> str_to_int_vec(std::string str);
    float str_to_float(std::string str);
    std::vector<float> str_to_float_vec(std::string str);
    double str_to_double(std::string str);
    std::vector<double> str_to_double_vec(std::string str);

#ifdef CLOM2_IMPLEMENTATION

    std::vector<std::string> args;
    bool check_for_help = false;
    std::string help_flag, alt_help_flag;

    std::string find_setting_value(std::string prim_name, std::string alt_name, std::string default_value) {
        auto i = std::find(args.begin(), args.end(), prim_name);
        auto j = std::find(args.begin(), args.end(), alt_name);

        if (args.end() == i && args.end() == j) {
            return default_value;
        }

        if (--args.end() == i) {
            std::cout << "error: Missing value for setting '" << prim_name << "'\n";
            exit(-1);
        }
        if (--args.end() == j) {
            std::cout << "error: Missing value for setting '" << alt_name << "'\n";
            exit(-1);
        }

        if (args.end() == i)
            return (*++j);
        else
            return (*++i);
    }

    bool find_flag_value(std::string prim_name, std::string alt_name) {
        auto i = std::find(args.begin(), args.end(), prim_name);
        auto j = std::find(args.begin(), args.end(), alt_name);

        if (args.end() == i && args.end() == j) return false;
        else return true;
    }

    std::string process_hint(std::string hint, std::string message) {
        if (check_for_help) std::cout << message << '\n';
        return hint;
    }

    // Converter Functions

    std::vector<std::string> str_to_str_vec(std::string str) {
        std::string tmp;
        std::stringstream ss {str};
        std::vector<std::string> vec;

        while(getline(ss, tmp, ' ')) vec.push_back(tmp);

        return vec;
    }

    int str_to_int(std::string str) {
        int v;
        try {
            v = std::stoi(str);
        } catch (...) {
            std::cout << "error: Failed to parse the value '" << str << "' (Should be an integer)\n";
            exit(-1);
        }
        return v;
    }

    std::vector<int> str_to_int_vec(std::string str) {
        std::vector<int> vec;
        std::vector<std::string> str_vec = str_to_str_vec(str);

        for (auto s : str_vec) vec.push_back(str_to_int(s));

        return vec;
    }

    float str_to_float(std::string str) {
        float v;
        try {
            v = std::stof(str);
        } catch (...) {
            std::cout << "error: Failed to parse the value '" << str << "' (Should be a float)\n";
            exit(-1);
        }
        return v;
    }

    std::vector<float> str_to_float_vec(std::string str) {
        std::vector<float> vec;
        std::vector<std::string> str_vec = str_to_str_vec(str);

        for (auto s : str_vec) vec.push_back(str_to_float(s));

        return vec;
    }

    double str_to_double(std::string str) {
        double v;
        try {
            v = std::stod(str);
        } catch (...) {
            std::cout << "error: Failed to parse the value '" << str << "' (Should be a double)\n";
            exit(-1);
        }
        return v;
    }

    std::vector<double> str_to_double_vec(std::string str) {
        std::vector<double> vec;
        std::vector<std::string> str_vec = str_to_str_vec(str);

        for (auto s : str_vec) vec.push_back(str_to_double(s));

        return vec;
    }

#endif /* CLOM2_IMPLEMENTATION */

} /* end of namespace clom2 */

#endif /* end of include guard: CLOM2_H */
