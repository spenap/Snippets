#include <cstdio>
#include <cstdint>
#include <iostream>

void PrintHex()
{
    uint8_t n = 0xFF;
    // C-Style
    printf("%#X\n", n);

    // lowercase hex (use std::dec or std::oct)
    std::cout << std::hex << int(n) << std::endl;
    // Show 0x (use std::noshowbase to restore)
    std::cout << std::hex << std::showbase << int(n) << std::endl;
    // uppercase hex (use std::nouppercase to restore)
    std::cout << std::hex << std::showbase << std::uppercase << int(n) << std::endl;
}

void PrintBool()
{
    bool val = true;
    printf("%s\n", val ? "true" : "false");

    std::cout << std::boolalpha << val << std::endl;
    // See std::numpunct for info on how to modify the spelling of
    // true, false and others http://en.cppreference.com/w/cpp/locale/numpunct
}

int main()
{
    PrintHex();
    PrintBool();
    return 0;
}
