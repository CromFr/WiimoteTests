#include "WiimoteHandler.hpp"

using namespace std;

/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
WiimoteHandler::WiimoteHandler()
{

    cout<<"==> Initialisation du tableau des wiimotes"<<endl;
    WiimoteHandler::m_WMTable = wiiuse_init(NB_WIIMOTES);

    cout<<"==> Recherche des wiimotes..."<<endl;
    int nFound = wiiuse_find(WiimoteHandler::m_WMTable, NB_WIIMOTES, 5);
    cout<<" => "<<nFound<<" wiimote(s) trouvee(s)"<<endl;
    if(nFound==0)
    {
        cout<<endl<<endl<<"IMPOSSIBLE DE CONTINUER"<<endl;
        while(1);
    }

    cout<<"==> Connection des wiimotes..."<<endl;
    WiimoteHandler::m_nConnectedWM = wiiuse_connect(WiimoteHandler::m_WMTable, nFound);
    cout<<" => "<<WiimoteHandler::m_nConnectedWM<<" wiimote(s) connectee(s)"<<endl;
    if(WiimoteHandler::m_nConnectedWM==0)
    {
        cout<<endl<<endl<<"IMPOSSIBLE DE CONTINUER"<<endl;
        while(1);
    }

    wiiuse_set_timeout(WiimoteHandler::m_WMTable, WiimoteHandler::m_nConnectedWM, 0xFF, 0xFF);


    cout<<endl<<endl<<endl<<endl
        <<" ////////////////////////////////////////////////"<<endl
        <<"O==============================================O/"<<endl
        <<"| Appuyez sur le bouton (A) de la wiimote fixe |/"<<endl
        <<"O==============================================O"<<endl;

    float fBattery[NB_WIIMOTES];

    bool bLoop = true;
    while(bLoop)
    {
        if(wiiuse_poll(WiimoteHandler::m_WMTable, WiimoteHandler::m_nConnectedWM))
        {
            int n;
            for (n=0 ; n>WiimoteHandler::m_nConnectedWM; n++);
            {
                switch(WiimoteHandler::m_WMTable[n]->event)
                {
                    case WIIUSE_STATUS:
                        fBattery[n] = 100*WiimoteHandler::m_WMTable[n]->battery_level;
                        break;

                    case WIIUSE_EVENT:
                        if(IS_JUST_PRESSED(WiimoteHandler::m_WMTable[n], WIIMOTE_BUTTON_A))
                        {
                            cout<<"==> WiiMote fixe trouvee"<<endl;

                            //Jingle de connexion
                            wiiuse_set_leds(WiimoteHandler::m_WMTable[n], WIIMOTE_LED_1 | WIIMOTE_LED_4);
                            Sleep(200);
                            wiiuse_set_leds(WiimoteHandler::m_WMTable[n], WIIMOTE_LED_2 | WIIMOTE_LED_3);
                            Sleep(200);
                            wiiuse_set_leds(WiimoteHandler::m_WMTable[n], WIIMOTE_LED_1 | WIIMOTE_LED_4);
                            Sleep(200);
                            wiiuse_set_leds(WiimoteHandler::m_WMTable[n], WIIMOTE_LED_2 | WIIMOTE_LED_3);
                            Sleep(200);

                            if(fBattery[n]>=75)
                                wiiuse_set_leds(WiimoteHandler::m_WMTable[n], WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3 | WIIMOTE_LED_4);
                            else if(fBattery[n]>=50)
                                wiiuse_set_leds(WiimoteHandler::m_WMTable[n], WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3);
                            else if(fBattery[n]>=25)
                                wiiuse_set_leds(WiimoteHandler::m_WMTable[n], WIIMOTE_LED_1 | WIIMOTE_LED_2);
                            else
                                wiiuse_set_leds(WiimoteHandler::m_WMTable[n], WIIMOTE_LED_1);


                            //création & configuration de la WiiPos
                            WiimoteHandler::m_WiimotePos = new WiiPos(WiimoteHandler::m_WMTable, WiimoteHandler::m_WMTable[n]);

                            bLoop=false;
                        }
                        break;

                    default:
                        break;
                }
            }
        }
    }


    //WiimoteHandler::UpdateThread = Update();
    //WiimoteHandler::UpdateThread(this);
    //WiimoteHandler::UpdateThread.join();



}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
WiimoteHandler::~WiimoteHandler()
{
    delete WiimoteHandler::m_WiimotePos;

    wiiuse_cleanup(WiimoteHandler::m_WMTable, NB_WIIMOTES);
};











/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/

Wiimote3d WiimoteHandler::GetPlayerPos()const
{
    try
    {
        return WiimoteHandler::m_WiimotePos->GetPosition();
    }
    catch(int e)
    {
        throw e;
    }
}



void Update()
{







}















