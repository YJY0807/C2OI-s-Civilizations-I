#define BUILD_DLL

#include <fastout.hpp>
#include <stdio.h>
#include <string>

namespace FastOut
{
    EXPORT inline Printer FastOut::Printer::operator<<(int x)
    {
        if (x == 0)
        {
            putchar('0');
            return *this;
        }
        if (x < 0)
        {
            x *= -1, putchar('-');
        }
        num = 0;
        while (x)
        {
            chr[++num] = (x % 10) + 48, x /= 10;
        }
        while (num)
        {
            putchar(chr[num--]);
        }
        return *this;
    }
    EXPORT inline Printer FastOut::Printer::operator<<(long long int x)
    {
        if (x == 0)
        {
            putchar('0');
            return *this;
        }
        if (x < 0)
        {
            x *= -1, putchar('-');
        }
        num = 0;
        while (x)
        {
            chr[++num] = (x % 10) + 48, x /= 10;
        }
        while (num)
        {
            putchar(chr[num--]);
        }
        return *this;
    }
    EXPORT inline Printer FastOut::Printer::operator<<(char x)
    {
        putchar(x);
        return *this;
    }
    EXPORT inline Printer FastOut::Printer::operator<<(const char str[])
    {
        int p = 0;
        while (str[p] != '\0')
        {
            putchar(str[p++]);
        }
        return *this;
    }
    EXPORT inline Printer FastOut::Printer::operator<<(std::string &x)
    {
        for(auto i : x)
        {
            putchar(i);
        }
        return *this;
    }
    Printer qout;
}