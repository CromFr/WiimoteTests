#include "WiimoteHandler.hpp"

using namespace std;

/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
WiimoteHandler::WiimoteHandler(ConfigFile* Config)
{
    m_Config = Config;



    cout<<"==> Initialisation du tableau des wiimotes"<<endl;
    m_WMTable = wiiuse_init(NB_WIIMOTES);

    cout<<"==> Recherche des wiimotes..."<<endl;
    int nFound = wiiuse_find(m_WMTable, NB_WIIMOTES, 5);
    cout<<" => "<<nFound<<" wiimote(s) trouvee(s)"<<endl;
    if(nFound==0)
    {
        cout<<endl<<endl<<"IMPOSSIBLE DE CONTINUER"<<endl;
        while(1);
    }

    cout<<"==> Connection des wiimotes..."<<endl;
    m_nConnectedWM = wiiuse_connect(m_WMTable, nFound);
    cout<<" => "<<m_nConnectedWM<<" wiimote(s) connectee(s)"<<endl;
    if(m_nConnectedWM==0)
    {
        cout<<endl<<endl<<"IMPOSSIBLE DE CONTINUER"<<endl;
        while(1);
    }

    wiiuse_set_timeout(m_WMTable, m_nConnectedWM, 0xFF, 0xFF);


    cout<<endl<<endl<<endl<<endl
        <<" ////////////////////////////////////////////////"<<endl
        <<"O==============================================O/"<<endl
        <<"| Appuyez sur le bouton (A) de la wiimote fixe |/"<<endl
        <<"O==============================================O"<<endl;

    float fBattery[NB_WIIMOTES];

    bool bLoop = true;
    while(bLoop)
    {
        if(wiiuse_poll(m_WMTable, m_nConnectedWM))
        {
            int n;
            for (n=0 ; n>m_nConnectedWM; n++);
            {
                switch(m_WMTable[n]->event)
                {
                    case WIIUSE_STATUS:
                        fBattery[n] = 100*m_WMTable[n]->battery_level;
                        break;

                    case WIIUSE_EVENT:
                        if(IS_JUST_PRESSED(m_WMTable[n], WIIMOTE_BUTTON_A))
                        {
                            cout<<"==> WiiMote fixe trouvee"<<endl;

                            //Jingle de connexion
                            wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_1 | WIIMOTE_LED_4);
                            Sleep(200);
                            wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_2 | WIIMOTE_LED_3);
                            Sleep(200);
                            wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_1 | WIIMOTE_LED_4);
                            Sleep(200);
                            wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_2 | WIIMOTE_LED_3);
                            Sleep(200);

                            if(fBattery[n]>=75)
                                wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3 | WIIMOTE_LED_4);
                            else if(fBattery[n]>=50)
                                wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3);
                            else if(fBattery[n]>=25)
                                wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_1 | WIIMOTE_LED_2);
                            else
                                wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_1);


                            //création & configuration de la WiiPos
                            m_WiimotePos = new WiiPos(m_WMTable, m_WMTable[n], m_nConnectedWM, Config);

                            bLoop=false;
                        }
                        break;

                    default:
                        break;
                }
            }
        }
    }

}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
WiimoteHandler::~WiimoteHandler()
{
    delete m_WiimotePos;

    wiiuse_cleanup(m_WMTable, NB_WIIMOTES);
};











/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/

Wiimote3d WiimoteHandler::GetPlayerPos()const
{
    try
    {
        return m_WiimotePos->GetPosition();
    }
    catch(int e)
    {
        throw e;
    }
}



void Update()
{







}















