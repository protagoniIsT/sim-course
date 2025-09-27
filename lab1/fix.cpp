#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
using namespace std;
typedef uint32_t uint;
typedef unsigned long long int ullint;
uint uint_format(uint numb, int leng) {
    return numb & ((1ull << leng) - 1);
}
uint invert_number(uint ch, int a_b){
    return uint_format((~ch) + 1, a_b);
}
ullint ullint_format(ullint numb, int leng) {
    return numb & ((1ull << leng) - 1);
}
ullint xbit(ullint numb, int bit) {
    return numb & (1ull << bit);
}
bool is_negative(uint ch, int a_b) {
    return ch & (1ull << (a_b - 1));
}
uint rounding(uint ch, int a, int b, int ok, bool m) {
    ullint sdvig = ch;
    sdvig *= 1000;
    uint drob = (sdvig >> b) % 1000;
    if (ok == 0)
        return drob;
    ullint after = ullint_format(sdvig, b);
    if (ok == 1) {
        if (xbit(after, b - 1)) {
            if (after - xbit(after, b - 1))
                return drob + 1;
            return drob + (xbit(sdvig, b) != 0);
        }
        return drob;
    }
    if (ok == 2) {
        if (!m)
            return drob + (after != 0);
        return drob;
    }
    if (ok == 3) {
        if (m)
            return drob + (after != 0);
        return drob;
    }
    return 0;
}
void oper_rounding(uint &result, int a, int b, int ok, char oper, uint after, bool &m, uint delit){
    if(oper == '*'){
        if (ok == 1)
            if(xbit(after, b - 1)){
                if(after > xbit(after, b - 1))
                    result += 1;
                else
                    result += xbit(result, 0);
            }
        if (ok == 2)
            if (!m)
                result += (after != 0);
        if (ok == 3)
            if(m)
                result += (after != 0);
        if (m)
            result = invert_number(result, a + b);
    }
    if (oper == '/') {
        if (ok == 1)
            if ((after << 1) >= delit) {
                if((after << 1) > delit)
                    result += 1;
                else
                    result += xbit(result, 0);
            }
        if (ok == 2)
            if(!m)
                result += (after != 0);
        if (ok == 3)
            if(m)
                result += (after != 0);
        if (m)
            result = invert_number(result, a + b);
    }
    m = false;
}
void out_uint(uint val, int a, int b, int ok, char oper, uint after, bool &m, uint delit) {
    uint ch = val;
    oper_rounding(ch, a, b, ok, oper, after, m, delit);
    if (is_negative(ch, a + b)) {
        m = true;
        ch = invert_number(ch, a + b) ;
    }
    uint drob = rounding(ch, a, b, ok, m);
    uint integer = ch >> b;
    if(!(!drob && !integer) && m)
        cout << '-';
    if (drob >= 1000) {
        integer++;
        drob %= 1000;
    }
    cout << integer << '.';
    if (drob < 100)
        cout << '0';
    if (drob < 10)
        cout << '0';
    cout << drob;
}
void invert_for_oper(uint &ch1, uint &ch2, uint a_b, bool &m){
    bool otr1 = is_negative(ch1, a_b), otr2 = is_negative(ch2, a_b);
    if(otr1)
        ch1 = invert_number(ch1, a_b);
    if(otr2)
        ch2 = invert_number(ch2, a_b);
    m = otr1 ^ otr2;
}
uint operations(char oper, int ok, int a, int b, uint n1, uint n2, uint &after, bool &m) {
    ullint result = n1;
    if (oper == '+')
        result += n2;
    if (oper == '-')
        result -= n2;
    if (oper == '*') {
        uint ch1 = n1, ch2 = n2;
        invert_for_oper(ch1, ch2, a + b, m);
        result = (ullint) ch1 * ch2;
        after = uint_format(result, b);
        result >>= b;
    }
    if (oper == '/') {
        uint ch1 = n1, ch2 = n2;
        invert_for_oper(ch1, ch2, a + b, m);
        if(is_negative(ch1, a + b))
            ch1 = invert_number(ch1, a + b);
        if(is_negative(ch2, a + b))
            ch2 = invert_number(ch2, a + b);
        result = ((ullint)ch1 << b) / ch2;
        after = ((ullint)ch1 << b) % ch2;
    }
    return ullint_format(result, a + b);
}
int main(int argc, char* argv[])
{
    int a = 0, b = 0, ok = 0;
    uint ch1 = 0, ch2 = 0, after;
    char oper = ' ';
    string ch = argv[1];
    bool point = false, m = false;
    for(auto el : ch){
        if(el != '.' && !point)
            a = a * 10 + (el - '0');
        if(point)
            b = b * 10 + (el - '0');
        if (el == '.' && !point)
            point = true;
    }
    ok = stoi(argv[2]);
    if(argv[3][0] == '+' || argv[3][0] == '-' || argv[3][0] == '*' || argv[3][0] == '/'){
        oper = argv[3][0];
        ch1 = uint_format(stoul(argv[4], nullptr, 16), a + b);
        ch2 = uint_format(stoul(argv[5], nullptr, 16), a + b);
    } else {
        ch1 = uint_format(stoul(argv[3], nullptr, 16), a + b);
    }
    if(oper == '/' && ch2 == 0) {
        cout << "div_by_zero\n";
        return 0;
    }
    uint result = operations(oper, ok, a, b, ch1, ch2, after, m);
    if(is_negative(ch2, a + b))
        ch2 = invert_number(ch2, a + b);
    out_uint(result, a, b, ok, oper, after, m, ch2);
    return 0;
}