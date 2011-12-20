/*
 *	wiiuse
 *
 *	Written By:
 *		Michael Laforest	< para >
 *		Email: < thepara (--AT--) g m a i l [--DOT--] com >
 *
 *	Copyright 2006-2007
 *
 *	This file is part of wiiuse.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	$Header$
 *
 */

/**
 *	@file
 *
 *	@brief Example using the wiiuse API.
 *
 *	This file is an example of how to use the wiiuse library.
 */

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

#define MAX_WIIMOTES				4



/**
 *	@brief Callback that handles an event.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *
 *	This function is called automatically by the wiiuse library when an
 *	event occurs on the specified wiimote.
 */
void handle_event(struct wiimote_t* wm)

{

    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_UP)) printf("UP pressed\n");

	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_DOWN)) printf("DOWN pressed\n");

    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_LEFT)) printf("LEFT pressed\n");

    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_RIGHT)) printf("RIGHT pressed\n");

    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_MINUS)) printf("MINUS pressed\n");

    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_PLUS)) printf("PLUS pressed\n");

    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_HOME)) printf("HOME pressed\n");

    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_TWO)) printf("2 pressed\n");

	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_ONE)) printf("1 pressed\n");

    if (IS_JUST_PRESSED(wm,WIIMOTE_BUTTON_A)) printf("A pressed\n");

    if (IS_JUST_PRESSED(wm,WIIMOTE_BUTTON_B)) printf("B pressed\n");

	//the battery status has just been get in the socket by the function handle_ctrl_status
	//printf("battery level:         %.2f %%\n", 100*wm->battery_level);

    //if (WIIUSE_USING_IR(wm))printf("IR ON\n");
    //else printf("IR OFF\n");

    //if the accelerometer is turned on then print angles
	//if (WIIUSE_USING_ACC(wm))
    //printf("wiimote roll  = %f [%f]\n", wm->orient.roll, wm->orient.a_roll);
    //printf("wiimote pitch = %f [%f]\n", wm->orient.pitch, wm->orient.a_pitch);
	//printf("wiimote yaw   = %f\n", wm->orient.yaw);


	/* show events specific to supported expansions */
	if (wm->exp.type == EXP_NUNCHUK)
    {
		/* nunchuk */
		struct nunchuk_t* nc = (nunchuk_t*)&wm->exp.nunchuk;

		if (IS_PRESSED(nc, NUNCHUK_BUTTON_C)) printf("NunChuck C pressed\n");
		if (IS_PRESSED(nc, NUNCHUK_BUTTON_Z)) printf("NunChuck Z pressed\n");

		//printf("nunchuk roll  = %f\n", nc->orient.roll);
		//printf("nunchuk pitch = %f\n", nc->orient.pitch);
        //printf("nunchuk yaw   = %f\n", nc->orient.yaw);
		//printf("nunchuk joystick angle:     %f\n", nc->js.ang);
		//printf("nunchuk joystick magnitude: %f\n", nc->js.mag);
	}
    else if (wm->exp.type == EXP_CLASSIC)
    {
		/* classic controller */
		struct classic_ctrl_t* cc = (classic_ctrl_t*)&wm->exp.classic;

		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_ZL))			printf("Classic: ZL pressed\n");
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_B))			printf("Classic: B pressed\n");
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_Y))			printf("Classic: Y pressed\n");
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_A))			printf("Classic: A pressed\n");
        if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_X))			printf("Classic: X pressed\n");
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_ZR))			printf("Classic: ZR pressed\n");
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_LEFT))		printf("Classic: LEFT pressed\n");
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_UP))			printf("Classic: UP pressed\n");
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_RIGHT))		printf("Classic: RIGHT pressed\n");
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_DOWN))		printf("Classic: DOWN pressed\n");
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_FULL_L))		printf("Classic: FULL L pressed\n");
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_MINUS))		printf("Classic: MINUS pressed\n");
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_HOME))		printf("Classic: HOME pressed\n");
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_PLUS))		printf("Classic: PLUS pressed\n");
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_FULL_R))		printf("Classic: FULL R pressed\n");

		//printf("classic L button pressed:         %f\n", cc->l_shoulder);
		//printf("classic R button pressed:         %f\n", cc->r_shoulder);
		//printf("classic left joystick angle:      %f\n", cc->ljs.ang);
		//printf("classic left joystick magnitude:  %f\n", cc->ljs.mag);
		//printf("classic right joystick angle:     %f\n", cc->rjs.ang);
		//printf("classic right joystick magnitude: %f\n", cc->rjs.mag);


    }
    else if (wm->exp.type == EXP_GUITAR_HERO_3) {
		/* guitar hero 3 guitar */
		struct guitar_hero_3_t* gh3 = (guitar_hero_3_t*)&wm->exp.gh3;

		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_STRUM_UP))		printf("Guitar: Strum Up pressed\n");
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_STRUM_DOWN))	printf("Guitar: Strum Down pressed\n");
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_YELLOW))		printf("Guitar: Yellow pressed\n");
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_GREEN))		printf("Guitar: Green pressed\n");
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_BLUE))			printf("Guitar: Blue pressed\n");
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_RED))			printf("Guitar: Red pressed\n");
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_ORANGE))		printf("Guitar: Orange pressed\n");
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_PLUS))			printf("Guitar: Plus pressed\n");
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_MINUS))		printf("Guitar: Minus pressed\n");

		//printf("Guitar whammy bar:          %f\n", gh3->whammy_bar);
		//printf("Guitar joystick angle:      %f\n", gh3->js.ang);
		//printf("Guitar joystick magnitude:  %f\n", gh3->js.mag);
	}
}



/////////////////////////////////////////////////////////////////////////////////
/**
 *	@brief Callback that handles a read event.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *	@param data		Pointer to the filled data block.
 *	@param len		Length in bytes of the data block.
 *
 *	This function is called automatically by the wiiuse library when
 *	the wiimote has returned the full data requested by a previous
 *	call to wiiuse_read_data().
 *
 *	You can read data on the wiimote, such as Mii data, if
 *	you know the offset address and the length.
 *
 *	The \a data pointer was specified on the call to wiiuse_read_data().
 *	At the time of this function being called, it is not safe to deallocate
 *	this buffer.
 */
void handle_read(struct wiimote_t* wm, byte* data, unsigned short len) {
	int i = 0;

	printf("\n\n--- DATA READ [wiimote id %i] ---\n", wm->unid);
	printf("finished read of size %i\n", len);
	for (; i < len; ++i) {
		if (!(i%16))
			printf("\n");
		printf("%x ", data[i]);
	}
	printf("\n\n");
}



/////////////////////////////////////////////////////////////////////////////////
/**
 *	@brief Callback that handles a controller status event.
 *
 *	@param wm				Pointer to a wiimote_t structure.
 *	@param attachment		Is there an attachment? (1 for yes, 0 for no)
 *	@param speaker			Is the speaker enabled? (1 for yes, 0 for no)
 *	@param ir				Is the IR support enabled? (1 for yes, 0 for no)
 *	@param led				What LEDs are lit.
 *	@param battery_level	Battery level, between 0.0 (0%) and 1.0 (100%).
 *
 *	This occurs when either the controller status changed
 *	or the controller status was requested explicitly by
 *	wiiuse_status().
 *
 *	One reason the status can change is if the nunchuk was
 *	inserted or removed from the expansion port.
 */
void handle_ctrl_status(struct wiimote_t* wm) {
	printf("\n\n--- CONTROLLER STATUS [wiimote id %i] ---\n", wm->unid);

    //Display of battery level on the leds
    /*
	if (wm->battery_level >= 0.75)
	wiiuse_set_leds(wm, WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3 | WIIMOTE_LED_4);
	if ((wm->battery_level < 0.75) && (wm->battery_level >= 0.5))
	wiiuse_set_leds(wm, WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3 );
	if ((wm->battery_level < 0.5) && (wm->battery_level >= 0.25))
	wiiuse_set_leds(wm, WIIMOTE_LED_1 | WIIMOTE_LED_2 );
    if (wm->battery_level < 0.25)
	wiiuse_set_leds(wm, WIIMOTE_LED_1 );
	*/

	//printf("attachment:      %i\n", wm->exp.type);
	//printf("speaker:         %i\n", WIIUSE_USING_SPEAKER(wm));
	//printf("ir:              %i\n", WIIUSE_USING_IR(wm));
	printf("leds:            %i %i %i %i\n", WIIUSE_IS_LED_SET(wm, 1), WIIUSE_IS_LED_SET(wm, 2), WIIUSE_IS_LED_SET(wm, 3), WIIUSE_IS_LED_SET(wm, 4));
	printf("battery:         %.2f %%\n", 100*wm->battery_level);

}



//////////////////////////////////////////////////////////////////////////////////
/**
 *	@brief Callback that handles a disconnection event.
 *
 *	@param wm				Pointer to a wiimote_t structure.
 *
 *	This can happen if the POWER button is pressed, or
 *	if the connection is interrupted.
 */
void handle_disconnect(wiimote* wm) {
	printf("\n\n--- DISCONNECTED [wiimote id %i] ---\n", wm->unid);
}


void test(struct wiimote_t* wm, byte* data, unsigned short len) {
	printf("test: %i [%x %x %x %x]\n", len, data[0], data[1], data[2], data[3]);
}




/////////////////////////////////////////////////////////////////////////////////
/**
 *	@brief main()
 *
 *	Connect to up to two wiimotes and print any events
 *	that occur on either device.
 */
int main(int argc, char** argv) {

	wiimote** wiimotes; //Declaration d'un tableau de pointeurs wiimote
	int found, connected;

    bool bError;
    do{
        bError = false;

        //Initialize an array of wiimote objects.
        //The parameter is the number of wiimotes I want to create.
        wiimotes =  wiiuse_init(MAX_WIIMOTES);

        system("CLS");
        printf("Wiimote Basic Test\n\n");


        //On cherche des wiimote
        //En paramètres: le tableau de wiimotes, le nombre MAX à chercher, le timout (en secondes) pour la recherche
        //Retourne le nombre de wiimotes trouvées
        found = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);

        if (!found)
           {
                   printf ("No wiimotes found.\n");
                   bError=true;
           }


        //On connecte les wiimotes trouvées
        //paramètres: Le tableau de wiimotes, le nombre de wiimotes trouvées
        //retourne le nombre de wiimotes connectées
        connected = wiiuse_connect(wiimotes, found);

        if (connected)  printf("Connected to %i wiimotes (of %i found).\n", connected, found);
        else
            {
                printf("Failed to connect to any wiimote.\n");
                system("PAUSE");
                bError=true;
            }
    }while(bError);


	//On allume la LED correspondant au numéro pour chaque wiimote (de 1 à 4)
	wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3);
	wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_2);
	wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_3);
	wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_4);

    //Vibration pour les wiimotes 1 et 2 de 0.2 seconde
    //Arguments (wiimote à faire vibrer, 1 = vibration ; 0 = arret de la vibration)
	wiiuse_rumble(wiimotes[0], 1);
	wiiuse_rumble(wiimotes[1], 1);
	#ifndef WIN32
		usleep(200000);
	#else
		Sleep(200);
	#endif
	wiiuse_rumble(wiimotes[0], 0);
	wiiuse_rumble(wiimotes[1], 0);

    //Timout des wiimotes, valeur max pour éviter d'avoir des messages d'erreur
    wiiuse_set_timeout(wiimotes,MAX_WIIMOTES,0xFF,0xFF);

    //Utilisation de l'IR activé sur la première wiimote
    wiiuse_set_ir(wiimotes[0],TRUE);

	// Sensor Bar positioon => (wiimote à utiliser, WIIUSE_IR_ABOVE = au dessus de l'écran ; WIIUSE_IR_BELOW = dessous l'écran)
	wiiuse_set_ir_position(wiimotes[0],WIIUSE_IR_ABOVE);

	//Pause de 2 secondes avant d'enchainer la boucle infinie
    Sleep(2000);


	//Boucle infinie
	while (1) {

        //Attente des évènements (tableau des wiimotes, max de wiimotes)
		if (wiiuse_poll(wiimotes, MAX_WIIMOTES))

           {
            //Boucle sur chaque wiimote, pour trouver celle qui a fourni un évènement
			for (int i=0; i < MAX_WIIMOTES; ++i)

                {
                //switch sur les évènements de la wiimote i
                //Si pas d'ev (case Defaut) on sort du switch et on passe à la suivante
                switch (wiimotes[i]->event)

                    {
					case WIIUSE_EVENT:
						//Evenement classique (boutons, ...)
						handle_event(wiimotes[i]);
						break;

					case WIIUSE_STATUS:
						//Retour de la demande de status
						handle_ctrl_status(wiimotes[i]);
						break;

					case WIIUSE_DISCONNECT:
					case WIIUSE_UNEXPECTED_DISCONNECT:
						//deconnection
						handle_disconnect(wiimotes[i]);
						break;

					case WIIUSE_READ_DATA:
						/*
						 *	Data we requested to read was returned.
						 *	Take a look at wiimotes[i]->read_req
						 *	for the data.
						 */
						break;

					case WIIUSE_NUNCHUK_INSERTED:
						/*
						 *	a nunchuk was inserted
						 *	This is a good place to set any nunchuk specific
						 *	threshold values.  By default they are the same
						 *	as the wiimote.
						 */
						printf("Nunchuk inserted.\n");
						break;

					case WIIUSE_CLASSIC_CTRL_INSERTED:
						printf("Classic controller inserted.\n");
						break;

					case WIIUSE_GUITAR_HERO_3_CTRL_INSERTED:
						/* some expansion was inserted */
						handle_ctrl_status(wiimotes[i]);
						printf("Guitar Hero 3 controller inserted.\n");
						break;

					case WIIUSE_NUNCHUK_REMOVED:
					case WIIUSE_CLASSIC_CTRL_REMOVED:
					case WIIUSE_GUITAR_HERO_3_CTRL_REMOVED:
						/* some expansion was removed */
						handle_ctrl_status(wiimotes[i]);
						printf("An expansion was removed.\n");
						break;

					default:
						break;
                    } //fin switch
                }//fin for
            }//fin IF
	}//fin While(1)

	/*
	 *	Disconnect the wiimotes
	 */
	wiiuse_cleanup(wiimotes, MAX_WIIMOTES);

	return 0;
}
