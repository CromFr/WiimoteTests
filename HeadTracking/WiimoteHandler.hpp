#ifndef WIIMOTEHANDLER_HPP_INCLUDED

#define WIIMOTEHANDLER_HPP_INCLUDED


#include <iostream>
#include <cstdlib>
#include <wiiuse.h>

#include "WiiPos.hpp"
#include "utils.hpp"



#define NB_WIIMOTES 2


class WiimoteHandler
{
    public:

    ///Organise le tableau des wiimotes & entame la proc�dure d'initialisation
    WiimoteHandler();

    ///Supprime les allocs dyna
    ~WiimoteHandler();


    Coord3d GetPlayerPos()const;


    private:
    wiimote** m_WMTable;
    int m_nConnectedWM;


    WiiPos* m_WiimotePos;
    //WiiPoint* PointingWM;
};



#endif