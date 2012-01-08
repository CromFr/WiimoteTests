#include <iostream>
#include <irrlicht.h>

#include "WiimoteHandler.hpp"

#define ID_CAMERA 1



using namespace irr;



int main()
{

    WiimoteHandler WMHndl;


    /*
    //Test du syst de positionnement
    while(1)
    {
        try
        {
            Coord3d PlayerPos = WMHndl.GetPlayerPos();

            std::cout<<PlayerPos.x<<"   \t"<<PlayerPos.y<<"   \t"<<PlayerPos.z<<std::endl;
        }
        catch(int e)
        {
            if(e==EXC_WIIPOS_NOT_ENOUGH_IRSRC)
                std::cout<<"Pas assez de sources"<<std::endl;
            else if(e==EXC_WIIPOS_NO_EVENT)
                std::cout<<"Pas d'event correct trouve"<<std::endl;
            else
                std::cout<<"Erreur inconnue"<<std::endl;
        }
    }
    */



    std::cout<<std::endl<<std::endl<<std::endl<<std::endl
        <<" ////////////////////////////"<<std::endl
        <<"O==========================O/"<<std::endl
        <<"| Lancement du moteur 3D ! |/"<<std::endl
        <<"O==========================O"<<std::endl;

    //Création de la fenêtre
    IrrlichtDevice *oDev = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1440,900), 32);

    video::IVideoDriver* oDriver = oDev->getVideoDriver();

    scene::ISceneManager *oSM = oDev->getSceneManager ();

    oDev->getCursorControl()-> setVisible(false);


    //--------------------



    scene::ICameraSceneNode* nodeCamera = oSM->addCameraSceneNode(0, core::vector3df(0, 0, 0), core::vector3df(0, 0, 0), ID_CAMERA);
    //nodeCamera->bindTargetAndRotation(true);
    //nodeCamera->setRotation(core::vector3df(0, 100, 100));


    /*scene::ICameraSceneNode* nodeCamera = oSM->addCameraSceneNodeFPS (
	0,       // le noeud parent de la caméra
	100.0,           // la vitesse de rotation de la caméra
	0.1,             // la vitesse de déplacement
	ID_CAMERA,                    // numéro d'identification du noeud
	0,     // une map permettant de re-affecter les touches
	5,            // taille de la keyMap
	true,   // autorise ou non les mouvements sur l'axe vertical
	10.0,             // vitesse de déplacement lors d'un saut
	false,          // inverse ou non la rotation de la caméra
	true);*/


    nodeCamera->setFarValue(5000);
    //--------------------

    //Création du cube-background
    scene::IAnimatedMeshSceneNode *nodeBackGround = oSM->addAnimatedMeshSceneNode(oSM->getMesh("data/cube.3ds"));
    //nodeBackGround->setMaterialTexture(0, oDriver->getTexture("data/mesh.bmp"));
    nodeBackGround->setMaterialFlag(irr::video::EMF_LIGHTING, true);



    oSM->setAmbientLight(video::SColor(128,255,255,255));

    //oSM->addLightSceneNode (0, core::vector3df(1000,0,500), video::SColor(255,0,255,0), 100.0f);









    //Boucle principale
    while(oDev->run())
    {
        //Prépare le rendu
        oDriver->beginScene(true, true, video::SColor(255,0,0,255));


        //fRota+=0.1;
        //nodeCamera->setRotation(irr::core::vector3df(fRota, fRota/2, fRota/3));

        try
        {
            Coord3d posPlayer = WMHndl.GetPlayerPos();

            //Si la récup des coordonnées s'est bien passée
            nodeCamera->setPosition(core::vector3df(posPlayer.x, posPlayer.z, -posPlayer.y-1000));
        }
        catch(int e)
        {

        }





        //Rendre la scène
        oSM->drawAll();

        //Affichage de la scène
        oDriver->endScene();
    }
}
