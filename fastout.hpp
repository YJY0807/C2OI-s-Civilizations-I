#ifndef FASTOUT_HPP
#define FASTOUT_HPP

#ifdef BUILD_DLL
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

#include <string>

namespace FastOut
{
    class EXPORT Printer
    {
        int num;
        char chr[21];

    public:
        EXPORT inline Printer operator<<(int x);
        EXPORT inline Printer operator<<(long long int x);
        EXPORT inline Printer operator<<(char x);
        EXPORT inline Printer operator<<(const char str[]);
        EXPORT inline Printer operator<<(std::string &x);
    };
    EXPORT extern Printer qout;
#define endl '\n'
}

#endif
