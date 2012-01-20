
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <winable.h>
#include <math.h>

#ifndef WIN32
	#include <unistd.h>
#endif

#include "wiiuse.h"

//#include <SDL/SDL.h>

#define NB_WIIMOTES 4

#define DIST_DOT2WM_MM 100
#define DIST_DOT2DOT_MM 20


wiimote** WMTable;
int nConnectedWM;
const ir_dot_t* dot0=0;
const ir_dot_t* dot1=0;

void InitWiimotes();
void EndWiimotes();
float GetDistanceBetweenDots();

struct coord {int x;int y;};



int main(int argc, char** argv)
{
    InitWiimotes();


    bool bLoop = true;

    float fRatio = -1.0;
    float fScale = -1.0;




    while(bLoop)
    {

        int n;

        if(wiiuse_poll(WMTable, nConnectedWM))
        {
            for (n=0 ; n>nConnectedWM; n++);
            {
                switch(WMTable[n]->event)
                {
                    case WIIUSE_NONE:
                        break;

                    case WIIUSE_CONNECT:
                        printf("Wiimote connectee\n");break;
					case WIIUSE_DISCONNECT:
                        printf("Wiimote deconnectee\n");break;
					case WIIUSE_UNEXPECTED_DISCONNECT:
                        printf("Wiimote deconnectee a la bourrin\n");break;

                    case WIIUSE_EVENT:
                        if(IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_A))
                        {
                            /*
                            //Recherche des deux sources infrarouges
                            for(int i=0 ; i<4 && (dot0==0 && dot1==0) ; i++)
                            {
                                if(WMTable[n]->ir.dot[i].visible)
                                {
                                    if(dot0==0)
                                        dot0 = &WMTable[n]->ir.dot[i];
                                    else
                                        dot1 = &WMTable[n]->ir.dot[i];
                                }
                            }*/

                            dot0=&WMTable[n]->ir.dot[0];
                            dot1=&WMTable[n]->ir.dot[1];

                            if(dot0->visible && dot1->visible)
                            {
                                //Calibration !

                                float fDistDotToDot = GetDistanceBetweenDots();


                                fRatio = DIST_DOT2WM_MM*fDistDotToDot;

                                //TESTS ONLY :
                                fRatio = DIST_DOT2WM_MM*273.1;


                                fScale = DIST_DOT2DOT_MM/fDistDotToDot;

                                //TESTS ONLY
                                fScale = DIST_DOT2DOT_MM/273.1;





                                printf("Wiimote calibree !\n");

                            }
                            else
                            {
                                printf("La calibration requiers deux sources infrarouges\n");
                            }

                        }

                        break;

					case WIIUSE_STATUS:
                        printf("Wiimote status\n");
                        printf("\n\n--- CONTROLLER STATUS [wiimote id %i] ---\n", WMTable[n]->unid);
                        printf("leds:            %i %i %i %i\n", WIIUSE_IS_LED_SET(WMTable[n], 1), WIIUSE_IS_LED_SET(WMTable[n], 2), WIIUSE_IS_LED_SET(WMTable[n], 3), WIIUSE_IS_LED_SET(WMTable[n], 4));
                        printf("battery:         %.1f %%\n", 100*WMTable[n]->battery_level);

                        printf("\n\nWIIMOTE NON CALIBREE ! Appuyez sur A une fois les sources placees a %dmm de la wiimote\n", DIST_DOT2WM_MM);
                        break;

					case WIIUSE_READ_DATA:
                        printf("Wiimote read\n");
                        break;


					case WIIUSE_NUNCHUK_INSERTED:
                        printf("Nunchuk insere\n");break;
					case WIIUSE_NUNCHUK_REMOVED:
                        printf("Nunchuk enleve\n");break;

					case WIIUSE_CLASSIC_CTRL_INSERTED:
					case WIIUSE_GUITAR_HERO_3_CTRL_INSERTED:
					case WIIUSE_CLASSIC_CTRL_REMOVED:
					case WIIUSE_GUITAR_HERO_3_CTRL_REMOVED:
                        break;
                }
            }
        }

        #define ARROW_SPEED 3

        #define SCREEN_WIDTH 1920
        #define SCREEN_HEIGHT 1080
        struct coord posA, posB, posC;
        for (n=0 ; n>nConnectedWM; n++);
        {



        }


        if(fRatio!=-1.0 && fScale!=-1.0)
        {
            printf("\n\nPosition :\n");
            if(dot0->visible && dot1->visible)
            {
                //Positon des dots
                posA.x = dot0->x;
                posA.y = 768-dot0->y;
                posB.x = dot1->x;
                posB.y = 768-dot1->y;

                //Position du centre
                posC.x = (posA.x+posB.x)/2;
                posC.y = (posA.y+posB.y)/2;

                //Calcul de la sistance en px séparant les dots
                float fDistDotToDot = sqrt( (dot0->x-dot1->x)*(dot0->x-dot1->x)+(dot0->y-dot1->y)*(dot0->y-dot1->y) );

                //Distance suivant z
                //float fZ = fRatio/fDistDotToDot;

                //printf("x=%.2fmm      y=%.2fmm      z=%.2fmm\n", (posC.x-512)*fScale*fZ/DIST_DOT2WM_MM, (posC.y-384)*fScale*fZ/DIST_DOT2WM_MM, fZ);

                float fY=-fRatio/fDistDotToDot;
                float fX=-(posC.x-512)*fScale*fY/DIST_DOT2WM_MM;
                float fZ=(posC.y-384)*fScale*fY/DIST_DOT2WM_MM;

                printf("x=%.2fmm      y=%.2fmm      z=%.2fmm\n", fX, fY, fZ);
            }
            else
                printf("Sources IR hors du champ de vision\n");
        }



        //SDL_Flip(oApp);


    }
    EndWiimotes();

    return 0;
}




void InitWiimotes()
{
    printf("==> Initialisation du tableau des wiimotes\n");
    WMTable = wiiuse_init(NB_WIIMOTES);

    printf("\n==> Recherche des wiimotes...\n");
    int nFound = wiiuse_find(WMTable, NB_WIIMOTES, 5);
    printf("...%d wiimotes trouvees\n", nFound);

    printf("\n==> Connection des wiimotes...\n");
    nConnectedWM = wiiuse_connect(WMTable, nFound);
    printf("...%d wiimotes connectees\n", nConnectedWM);


    wiiuse_set_timeout(WMTable, nConnectedWM, 0xFF, 0xFF);

    for(int n=0 ; n<nConnectedWM; n++)
    {
        wiiuse_set_ir(WMTable[n],1);
        wiiuse_rumble(WMTable[n], true);
    }


    //Signal lumineux
    for(int n=0 ; n<4 ; n++)
    {
        int nPow = 1;
        for(int i=0 ; i<n ; i++)
            nPow*=2;

        for(int i=0 ; i<nConnectedWM ; i++)
        {
            wiiuse_set_leds(WMTable[i], nPow*16);
            if(n==0)wiiuse_rumble(WMTable[i], false);
        }


        Sleep(100);

    }
    for(int n=0 ; n<nConnectedWM ; n++)
    {
        wiiuse_set_leds(WMTable[n], 0x90);
    }



}



void EndWiimotes()
{
    wiiuse_cleanup(WMTable, 4);

}



float GetDistanceBetweenDots()
{

    return sqrt( (dot0->x-dot1->x)*(dot0->x-dot1->x)+(dot0->y-dot1->y)*(dot0->y-dot1->y) );
}








