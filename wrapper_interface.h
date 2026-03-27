#pragma once

#include <windows.h>
void runAtracsys();
class AtracsysWrapper
{
public:
    static bool load();
    static int initialize();
    static int getValue();

private:
    static HMODULE dllHandle;

    typedef int (*InitFunc)();
    typedef int (*GetValueFunc)();

    static InitFunc initFunc;
    static GetValueFunc getValueFunc;
};
