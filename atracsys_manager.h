#ifndef ATRACSYS_MANAGER_H
#define ATRACSYS_MANAGER_H

class atracsys_manager
{
public:
    atracsys_manager();

    int initializeSDK();
    int isDeviceConnected();
};

#endif
