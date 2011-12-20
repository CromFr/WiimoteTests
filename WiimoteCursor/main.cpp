
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <winable.h>

#ifndef WIN32
	#include <unistd.h>
#endif

#include "wiiuse.h"

#define NB_WIIMOTES 4

#define CUR_CTRL_ARROWS 0
#define CUR_CTRL_MOTPOINT 1
#define CUR_CTRL_MOTPAD 2


wiimote** WMTable;
int nConnectedWM;

void InitWiimotes();
void EndWiimotes();



int main(int argc, char** argv)
{
    InitWiimotes();

    bool bLoop = true;
    int nCursorControl = CUR_CTRL_ARROWS;
    float fMouseSpeedX=0;
    float fMouseSpeedY=0;

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

                        if(IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_MINUS)) printf("MINUS NON SUPPORTE\n");
                        if(IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_PLUS)) printf("PLUS NON SUPPORTE\n");

                        if(IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_TWO)) printf("2 NON SUPPORTE\n");

                        //Touche combo !
                        while(IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_ONE) || IS_HELD(WMTable[n], WIIMOTE_BUTTON_ONE))
                        {
                            while(wiiuse_poll(WMTable, nConnectedWM))
                            {
                                if(IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_B))
                                {
                                    printf("Controle du curseur par pointage\n");
                                    nCursorControl = CUR_CTRL_MOTPOINT;
                                    break;
                                }
                                else if(IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_A))
                                {
                                    printf("Controle du curseur en manette\n");
                                    nCursorControl = CUR_CTRL_MOTPAD;
                                    break;
                                }
                                else if(IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_UP) || IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_DOWN) || IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_RIGHT) || IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_LEFT))
                                {
                                    printf("Controle du curseur avec les fleches\n");
                                    nCursorControl = CUR_CTRL_ARROWS;
                                    break;
                                }
                            }
                        }


                        //Souris . Clic gauche = B
                        if(IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_B))
                        {
                            if(nCursorControl == CUR_CTRL_MOTPAD)mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
                            else mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                        }
                        if(IS_RELEASED(WMTable[n], WIIMOTE_BUTTON_B))
                        {
                            if(nCursorControl == CUR_CTRL_MOTPAD)mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
                            else mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                        }
                        //

                        //Souris . Bouton milieu = HOME
                        if(IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_HOME))
                            mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
                        if(IS_RELEASED(WMTable[n], WIIMOTE_BUTTON_HOME))
                            mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
                        //

                        //Souris . Clic droit = A
                        if(IS_JUST_PRESSED(WMTable[n], WIIMOTE_BUTTON_A))
                        {
                            if(nCursorControl == CUR_CTRL_MOTPAD)mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                            else mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
                        }
                        if(IS_RELEASED(WMTable[n], WIIMOTE_BUTTON_A))
                        {
                            if(nCursorControl == CUR_CTRL_MOTPAD)mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                            else mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
                        }
                        //
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
        //else//Pas d'event
        {
            #define ARROW_SPEED 3

            #define RELATIVE_DECEL 25
            #define ABSOLUTE_DECEL 10

            #define SCREEN_WIDTH 1920
            #define SCREEN_HEIGHT 1080
            for (n=0 ; n>nConnectedWM; n++);
            {
                tagPOINT CursorPos;

                if(nCursorControl==CUR_CTRL_MOTPOINT || nCursorControl==CUR_CTRL_MOTPAD)
                {

                    //Décélération permanente
                    if(fMouseSpeedX<0)
                    {
                        if(fMouseSpeedX+ABSOLUTE_DECEL>0)fMouseSpeedX=0;
                        else fMouseSpeedX+=ABSOLUTE_DECEL;
                    }
                    else if(fMouseSpeedX>0)
                    {
                        if(fMouseSpeedX-ABSOLUTE_DECEL<0)fMouseSpeedX=0;
                        else fMouseSpeedX-=ABSOLUTE_DECEL;
                    }
                    if(fMouseSpeedY<0)
                    {
                        if(fMouseSpeedY+ABSOLUTE_DECEL>0)fMouseSpeedY=0;
                        else fMouseSpeedY+=ABSOLUTE_DECEL;
                    }
                    else if(fMouseSpeedY>0)
                    {
                        if(fMouseSpeedY-ABSOLUTE_DECEL<0)fMouseSpeedY=0;
                        else fMouseSpeedY-=ABSOLUTE_DECEL;
                    }




                    GetCursorPos(&CursorPos);

                    //Déplacement du curseur
                    //printf("roll=%.2f\npitch=%.2f", WMTable[0]->orient.roll, WMTable[0]->orient.pitch);
                    int nNewX;
                    int nNewY;
                    int nDeltaX;
                    int nDeltaY;
                    if(nCursorControl==CUR_CTRL_MOTPOINT)
                    {
                        nNewX = ((WMTable[0]->orient.roll+45)/90)*SCREEN_WIDTH;
                        nNewY = ((WMTable[0]->orient.pitch+60)/90)*SCREEN_HEIGHT;
                    }
                    else if(nCursorControl==CUR_CTRL_MOTPAD)
                    {
                        nNewX = ((WMTable[0]->orient.pitch+45)/90)*SCREEN_WIDTH;
                        nNewY = ((-WMTable[0]->orient.roll+45)/90)*SCREEN_HEIGHT;
                    }

                    nDeltaX = nNewX - CursorPos.x;
                    nDeltaY = nNewY - CursorPos.y;


                    fMouseSpeedX += nDeltaX/RELATIVE_DECEL;
                    fMouseSpeedY += nDeltaY/RELATIVE_DECEL;




                    SetCursorPos(CursorPos.x+fMouseSpeedX, CursorPos.y+fMouseSpeedY);
                    //SetCursorPos(nNewX, nNewY);

                }
                else
                {
                    if(IS_PRESSED(WMTable[n], WIIMOTE_BUTTON_UP) || IS_HELD(WMTable[n], WIIMOTE_BUTTON_UP))
                    {
                        GetCursorPos(&CursorPos);
                        SetCursorPos(CursorPos.x, CursorPos.y-ARROW_SPEED);
                    }
                    if(IS_PRESSED(WMTable[n], WIIMOTE_BUTTON_DOWN) || IS_HELD(WMTable[n], WIIMOTE_BUTTON_DOWN))
                    {
                        tagPOINT CursorPos;
                        GetCursorPos(&CursorPos);
                        SetCursorPos(CursorPos.x, CursorPos.y+ARROW_SPEED);
                    }
                    if(IS_PRESSED(WMTable[n], WIIMOTE_BUTTON_LEFT) || IS_HELD(WMTable[n], WIIMOTE_BUTTON_LEFT))
                    {
                        tagPOINT CursorPos;
                        GetCursorPos(&CursorPos);
                        SetCursorPos(CursorPos.x-ARROW_SPEED, CursorPos.y);
                    }
                    if(IS_PRESSED(WMTable[n], WIIMOTE_BUTTON_RIGHT) || IS_HELD(WMTable[n], WIIMOTE_BUTTON_RIGHT))
                    {
                        tagPOINT CursorPos;
                        GetCursorPos(&CursorPos);
                        SetCursorPos(CursorPos.x+ARROW_SPEED, CursorPos.y);
                    }
                }
            }
        }
    }
    EndWiimotes();
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
        wiiuse_set_ir(WMTable[n],0);
        //wiiuse_set_smooth_alpha (WMTable[n], 255);
    }


    //Signal lumineux
    for(int n=0 ; n<4 ; n++)
    {
        int nPow = 1;
        for(int i=0 ; i<n ; i++)
            nPow*=2;

        for(int i=0 ; i<nConnectedWM ; i++)
            wiiuse_set_leds(WMTable[i], nPow*16);

        Sleep(200);

    }
    for(int i=0 ; i<nConnectedWM ; i++)
        wiiuse_set_leds(WMTable[i], 0xF0);



}



void EndWiimotes()
{
    wiiuse_cleanup(WMTable, 4);

}








