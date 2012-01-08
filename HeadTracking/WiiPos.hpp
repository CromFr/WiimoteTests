#ifndef WIIPOS_HPP_INCLUDED

#define WIIPOS_HPP_INCLUDED


#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <wiiuse.h>

#include "utils.hpp"



#define EXC_WIIPOS_NOT_ENOUGH_IRSRC 1
#define EXC_WIIPOS_NO_EVENT 2



class WiiPos
{

    public:
    ///Initialisation et calibration
    WiiPos(wiimote** WMTable, wiimote* Wiimote);

    //~WiiPos();


    void Calibrer();

    ///throw EXC_WIIPOS_NOT_ENOUGH_IRSRC et EXC_WIIPOS_NO_EVENT
    Coord3d GetPosition() const;



    ///La wiimote utilisée
    wiimote* WM;


    private:
    wiimote** WMTable;

    float m_fCALDistDotToDotmm;
    float m_fCALDistDotToDotpx;
    float m_fCALDistWmToDotsmm;

    float m_fCALRatio;
    float m_fCALScale;




};




#endif
