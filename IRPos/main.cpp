
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

#include <SDL/SDL.h>

#define NB_WIIMOTES 4

#define DIST_DOT2WM_MM 300


wiimote** WMTable;
int nConnectedWM;

void InitWiimotes();
void EndWiimotes();
float GetDistanceBetweenDots();



int main(int argc, char** argv)
{
    freopen("CON", "w", stdout);
/*    freopen("CON", "r", stdin);
    freopen("CON", "w", stderr);
*/

    InitWiimotes();

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* oApp = SDL_SetVideoMode(1024, 768, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);

    SDL_Surface* sprRedDot;
        sprRedDot = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 16, 0, 0, 0, 0);
        SDL_FillRect(sprRedDot, NULL, SDL_MapRGB(oApp->format, 255, 0, 0));

    SDL_Surface* sprGreenDot;
        sprGreenDot = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 16, 0, 0, 0, 0);
        SDL_FillRect(sprGreenDot, NULL, SDL_MapRGB(oApp->format, 0, 255, 0));

    bool bLoop = true;

    float fRatio = -1.0;




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
                            //if(WMTable[n]->ir.dot[0].visible && WMTable[n]->ir.dot[1].visible)
                            {
                                //Calibration !

                                //TESTS ONLY :
                                //fRatio = DIST_DOT2WM_MM/GetDistanceBetweenDots();
                                fRatio = DIST_DOT2WM_MM*89.6772;

                            }
                            //else
                            {
                            //    printf("La calibration requiers deux sources infrarouges");
                            }

                        }

                        break;

					case WIIUSE_STATUS:
                        printf("Wiimote status\n");
                        printf("\n\n--- CONTROLLER STATUS [wiimote id %i] ---\n", WMTable[n]->unid);
                        printf("leds:            %i %i %i %i\n", WIIUSE_IS_LED_SET(WMTable[n], 1), WIIUSE_IS_LED_SET(WMTable[n], 2), WIIUSE_IS_LED_SET(WMTable[n], 3), WIIUSE_IS_LED_SET(WMTable[n], 4));
                        printf("battery:         %.1f %%\n", 100*WMTable[n]->battery_level);
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
        for (n=0 ; n>nConnectedWM; n++);
        {

            SDL_Rect posA, posB, posC;
            if (WMTable[n]->ir.dot[0].visible)
            {
                posA.x = WMTable[n]->ir.dot[0].x;
                posA.y = 768-WMTable[n]->ir.dot[0].y;
                SDL_BlitSurface(sprRedDot, NULL, oApp, &posA);
            }
            if (WMTable[n]->ir.dot[1].visible)
            {

                posB.x = WMTable[n]->ir.dot[1].x;
                posB.y = 768-WMTable[n]->ir.dot[1].y;
                SDL_BlitSurface(sprRedDot, NULL, oApp, &posB);
            }
            posC.x = (posA.x+posB.x)/2;
            posC.y = (posA.y+posB.y)/2;
            SDL_BlitSurface(sprGreenDot, NULL, oApp, &posC);

        }



        //system("CLS");
        printf("Distance=%.2f mm\n", fRatio/GetDistanceBetweenDots());



        SDL_Flip(oApp);


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

    return sqrt( (WMTable[0]->ir.dot[0].x-WMTable[0]->ir.dot[1].x)*(WMTable[0]->ir.dot[0].x-WMTable[0]->ir.dot[1].x)+(WMTable[0]->ir.dot[0].y-WMTable[0]->ir.dot[1].y)*(WMTable[0]->ir.dot[0].y-WMTable[0]->ir.dot[1].y) );
}








