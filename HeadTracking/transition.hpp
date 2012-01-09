#ifndef TRANSITION_HPP_INCLUDED
#define TRANSITION_HPP_INCLUDED

#include <irrlicht.h>
#include "WiimoteHandler.hpp"


#define SCREEN_WIDTH_MM 345
#define SCREEN_HEIGHT_MM 195

#define WORLD_WIDTH 2000
#define WORLD_HEIGHT 2000


Wiimote3d World3dToWiimote3d(irr::core::vector3df World, bool bBorner=true);
Wiimote3d World3dToWiimote3d(float x, float y, float z, bool bBorner=true);

irr::core::vector3df Wiimote3dToWorld3d(Wiimote3d Wiimote, bool bBorner=true);
irr::core::vector3df Wiimote3dToWorld3d(float x, float y, float z, bool bBorner=true);

#endif // TRANSITION_HPP_INCLUDED
