#ifndef WIIMOTEHANDLER_HPP_INCLUDED

#define WIIMOTEHANDLER_HPP_INCLUDED


#include <iostream>
#include <cstdlib>
#include <wiiuse.h>

#include "ConfigFile.hpp"
#include "WiiPos.hpp"


#define NB_WIIMOTES 2


class WiimoteHandler
{
    public:

    ///Organise le tableau des wiimotes & entame la procédure d'initialisation
    WiimoteHandler(ConfigFile* Config);

    ///Supprime les allocs dyna
    ~WiimoteHandler();


    Wiimote3d GetPlayerPos()const;




    private:
    wiimote** m_WMTable;
    int m_nConnectedWM;
    ConfigFile* m_Config;


    WiiPos* m_WiimotePos;
    //WiiPoint* PointingWM;
};



#endif
