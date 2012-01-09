#include <iostream>
#include <irrlicht.h>

#include "WiimoteHandler.hpp"
#include "transition.hpp"

#define ID_CAMERA 1



using namespace irr;




float DegToRad(float fDeg)
{
    return 3.1415936*fDeg/180;
}


void TranslateCameraToPos(scene::ICameraSceneNode* oCamera, irr::core::vector3df Pos)
{
    oCamera->setTarget(core::vector3df(Pos.X, Pos.Y, Pos.Z+1));
    oCamera->setPosition(core::vector3df(Pos.X, Pos.Y, Pos.Z));

}



int main()
{

    WiimoteHandler WMHndl;


    //==================== Test du syst de positionnement
    #define TEST_POSITIONNING false
    while(TEST_POSITIONNING)
    {
        try
        {
            Wiimote3d PlayerPos = WMHndl.GetPlayerPos();

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
    //--------------------




    std::cout<<std::endl<<std::endl<<std::endl<<std::endl
        <<" ////////////////////////////"<<std::endl
        <<"O==========================O/"<<std::endl
        <<"| Lancement du moteur 3D ! |/"<<std::endl
        <<"O==========================O"<<std::endl;


    //==================== Création de la fenêtre
    #define FULLSCREEN true
    IrrlichtDevice *oDev = 0;
    if(FULLSCREEN)
        oDev = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1920,1080), 32, true);
    else
        oDev = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1440,900), 32, false);

    oDev->setWindowCaption(L"Irrlicht : HeadTracking");

    video::IVideoDriver* oDriver = oDev->getVideoDriver();

    scene::ISceneManager *oSM = oDev->getSceneManager ();

    oDev->getCursorControl()-> setVisible(false);
    //--------------------


    //==================== Chargement des images 2d
    video::ITexture *texNoIRSrc = oDriver->getTexture("data/no_ir_src.png");
    //--------------------


    //==================== Setup de la caméra
    scene::ICameraSceneNode* nodeCamera = oSM->addCameraSceneNode(0, core::vector3df(0, 0, 0), core::vector3df(0, 0, 0), ID_CAMERA);
    nodeCamera->bindTargetAndRotation(true);

    nodeCamera->setFarValue(5000);
    //--------------------


    //==================== Création du cube-background
    scene::IAnimatedMeshSceneNode *nodeBackGround = oSM->addAnimatedMeshSceneNode(oSM->getMesh("data/cube.3ds"));
    //nodeBackGround->setMaterialTexture(0, oDriver->getTexture("data/mesh.bmp"));
    nodeBackGround->setMaterialFlag(irr::video::EMF_LIGHTING, true);
    //--------------------


    //==================== Eclairage
    oSM->setAmbientLight(video::SColor(255,255,255,255));

    //oSM->addLightSceneNode (0, core::vector3df(1000,0,500), video::SColor(255,0,255,0), 100.0f);
    //--------------------







    int i=0;

    bool bNoIRSrc = false;
    //Boucle principale
    while(oDev->run())
    {
        //Prépare le rendu
        oDriver->beginScene(true, true, video::SColor(255,0,0,255));


        try
        {
        //==
            //On récupère les coordonnées du joueur
            //Cette methode peut throw
            Wiimote3d posPlayer = WMHndl.GetPlayerPos();

            //GetPlayerPos n'a pas throw : il y a assez de sources
            bNoIRSrc=false;


            //On positionne la caméra à l'endroit du joueur
            TranslateCameraToPos(nodeCamera, Wiimote3dToWorld3d(posPlayer.x, 0, posPlayer.z));


            //==================== On calcule la rotation de la caméra
            //Horizontalement
            float fHrzAlpha1=atan(posPlayer.x/(-posPlayer.y));
            float fHrzAlpha2=atan( (600-posPlayer.x)/(-posPlayer.y));
            // ...

            //Verticalement
            // ...

            //On l'applique à la caméra
            float fRotaRad = fHrzAlpha1-fHrzAlpha2;
            //nodeCamera->setRotation(core::vector3df(0, core::PI*fRotaRad/180, 0));
            //--------------------


            //==================== Calcul du champs de vision horizontal
            //float fHrzFOV = fHrzAlpha1 + fHrzAlpha2;
            //nodeCamera->setFOV(fHrzFOV);
            //--------------------



            /*
            core::stringw sCaption = L"Rota=";
            sCaption += (fHrzAlpha1-fHrzAlpha2);
            oDev->setWindowCaption(sCaption.c_str());*/

        //==
        }
        catch(int e)
        {
            if(e==EXC_WIIPOS_NOT_ENOUGH_IRSRC)
            {
                bNoIRSrc=true;
            }
        }



        //Rendre la scène
        oSM->drawAll();


        //Notification GUI
        if(bNoIRSrc)
            oDriver->draw2DImage(texNoIRSrc, irr::core::position2d<irr::s32>(50,50),
                        irr::core::rect<irr::s32>(0,0,200,200), 0,
                        irr::video::SColor(255, 255, 255, 255), true);


        //Affichage de la scène
        oDriver->endScene();


        //i++;
        //Sleep(100);
    }
}
