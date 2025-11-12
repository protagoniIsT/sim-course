#include "helpers.h"
#include "defs.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <iostream>

namespace std {
    template <typename T>
    struct hash<std::vector<T>> {
        size_t operator()(const std::vector<T>& v) const noexcept {
            size_t h = 0;
            for (auto& x : v) {
                h ^= std::hash<T>{}(x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            }
            return h;
        }
    };
}

namespace tests {
    using namespace std;
    
    unordered_map<vector<word_t>, word_t> bdep = {
        {{0, 0},     0},
        {{1, 1},     1},
        {{3, 224},   96},
        {{11, 202}, 138},
        {{15, 85},  85},
        {{141, 206}, 74},
        {{255, 255}, 255},
        {{5, 18}, 2},
        {{42, 21}, 4},
        {{7, 48}, 48}
    };

    unordered_map<word_t, word_t> cls = {
        {0, 32},
        {34, 26},
        {-1, 32},
        {-2, 31},
        {-4, 30},
        {-8, 29},
        {-128, 25},
        {-19293, 17},
    };

    unordered_map<vector<word_t>, word_t> rotate_right = {
        {{34, 3}, 1073741828},
        {{34, 2}, (word_t)-2147483640},
        {{2147483647, 1}, (word_t)-1073741825},
    };

    unordered_map<vector<int32_t>, word_t> saturate_signed = {
        {{50, 8}, 50},
        {{200, 8}, 127},
        {{-300, 8}, (word_t)-128},
        {{1024, 10}, 511},
        {{-1024, 10}, (word_t)-512},
        {{15, 5}, 15},
        {{16, 5}, 15},
        {{-16, 5}, (word_t)-16},
        {{-17, 5}, (word_t)-16},
        {{0, 3}, 0},
        {{4, 3}, 3},
        {{-5, 3}, (word_t)-4},
        {{-1, 31}, (word_t)-1},
        {{INT32_MAX, 31}, 1073741823},
        {{INT32_MIN, 31}, (word_t)-1073741824},
    };
}


int main() {
    using namespace std;
    unordered_map<string, function<word_t(const vector<int32_t>&)>> funcs = {
        {"bit_deposit", [](const vector<int32_t>& v) {
            return helpers::bit_deposit(v[0], v[1]);
        }},
        {"count_leading_signs", [](const vector<int32_t>& v) {
            return helpers::count_leading_signs(v[0]);
        }},
        {"rotate_right", [](const vector<int32_t>& v) {
            return helpers::rotate_right(v[0], v[1]);
        }},
        {"saturate_signed", [](const vector<int32_t>& v) {
            return helpers::saturate_signed(v[0], v[1]);
        }},
    };

    unordered_map<string, vector<pair<vector<int32_t>, int32_t>>> all_tests = {
        {"bit_deposit", [] {
            vector<pair<vector<int32_t>, int32_t>> v;
            for (auto &[args, expected] : tests::bdep)
                v.push_back({{(int32_t)args[0], (int32_t)args[1]}, (int32_t)expected});
            return v;
        }()},
        {"count_leading_signs", [] {
            vector<pair<vector<int32_t>, int32_t>> v;
            for (auto &[x, expected] : tests::cls)
                v.push_back({{(int32_t)x}, (int32_t)expected});
            return v;
        }()},
        {"rotate_right", [] {
            vector<pair<vector<int32_t>, int32_t>> v;
            for (auto &[args, expected] : tests::rotate_right)
                v.push_back({{(int32_t)args[0], (int32_t)args[1]}, (int32_t)expected});
            return v;
        }()},
        {"saturate_signed", [] {
            vector<pair<vector<int32_t>, int32_t>> v;
            for (auto &[args, expected] : tests::saturate_signed)
                v.push_back({args, (int32_t)expected});
            return v;
        }()},
    };

    size_t passed = 0, failed = 0;

    for (auto &[name, cases] : all_tests) {
        cout << "\n=== " << name << " ===\n";
        auto func = funcs.at(name);

        for (auto &[args, expected] : cases) {
            word_t res = func(args);
            int32_t sres = static_cast<int32_t>(res);

            bool ok = (sres == expected);
            cout << (ok ? "PASS " : "FAIL ")
                 << name << "(";
            for (size_t i = 0; i < args.size(); ++i)
                cout << args[i] << (i + 1 < args.size() ? ", " : "");
            cout << ") = " << sres;

            if (!ok) {
                cout << " (";
                cout << res;
                cout << "), expected " << expected;
                failed++;
            } else {
                passed++;
            }
            cout << "\n";
        }
    }

    cout << "\n=== Summary ===\n";
    cout << "Passed: " << passed << "\n";
    cout << "Failed: " << failed << "\n";



    return 0;
}