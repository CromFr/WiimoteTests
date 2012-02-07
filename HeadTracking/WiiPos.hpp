#ifndef WIIPOS_HPP_INCLUDED

#define WIIPOS_HPP_INCLUDED


#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cmath>
#include <wiiuse.h>
#include "ConfigFile.hpp"




class Wiimote3d
{
    public:
    Wiimote3d(float x=0, float y=0, float z=0);

    virtual void set(float x, float y, float z);

    //attributs
    float x;
    float y;
    float z;

};



#define EXC_WIIPOS_NOT_ENOUGH_IRSRC 1
#define EXC_WIIPOS_NO_EVENT 2

class WiiPos
{

    public:

    WiiPos(){}

    ///Initialisation et calibration
    WiiPos(wiimote** WMTable, wiimote* Wiimote, int nConnectedWM, ConfigFile* Config);


    void Calibrer();

    ///throw EXC_WIIPOS_NOT_ENOUGH_IRSRC et EXC_WIIPOS_NO_EVENT
    Wiimote3d GetPosition() const;



    ///La wiimote utilisée
    wiimote* m_WM;


    private:
    wiimote** m_WMTable;
    int m_nConnectedWM;
    ConfigFile* m_Config;

    float m_fCALDistDotToDotmm;
    float m_fCALDistDotToDotpx;
    float m_fCALDistWmToDotsmm;
    float m_fCALDistWmToScreenCenter;

    float m_fCALRatio;
    float m_fCALScale;




};




#endif
