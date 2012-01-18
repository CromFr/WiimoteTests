#include <iostream>
#include <irrlicht.h>

#include "WiimoteHandler.hpp"
#include "transition.hpp"

#define ID_CAMERA 1



using namespace irr;




float DegToRad(float fDeg)
{
    return 3.14159365*fDeg/180;
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
    #define FULLSCREEN false
    IrrlichtDevice *oDev = 0;
    if(FULLSCREEN)
        oDev = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1920,1080), 32, true);
    else
        oDev = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1800,900), 32, false);

    oDev->setWindowCaption(L"Irrlicht : HeadTracking");

    video::IVideoDriver* oDriver = oDev->getVideoDriver();

    scene::ISceneManager *oSM = oDev->getSceneManager ();

    oDev->getCursorControl()-> setVisible(true);
    //--------------------


    //==================== Chargement des images 2d
    video::ITexture *texNoIRSrc = oDriver->getTexture("data/no_ir_src.png");
    //--------------------


    //==================== Setup de la caméra
    scene::ISceneNode* nodeCamContainer = oSM->addEmptySceneNode();

    scene::ICameraSceneNode* nodeCamera = oSM->addCameraSceneNode(nodeCamContainer, core::vector3df(0, 0, 0), core::vector3df(0, 0, 10), ID_CAMERA);
    nodeCamera->bindTargetAndRotation(false);

    nodeCamera->setFarValue(5000);
    //--------------------


    //==================== Création du cube-background
    scene::IAnimatedMeshSceneNode *nodeBackGround = oSM->addAnimatedMeshSceneNode(oSM->getMesh("data/cube.3ds"));
    //nodeBackGround->setMaterialTexture(0, oDriver->getTexture("data/mesh.bmp"));
    nodeBackGround->setMaterialFlag(irr::video::EMF_LIGHTING, true);
    //--------------------


    //==================== Joueur
    scene::IAnimatedMeshSceneNode *nodePlayer = oSM->addAnimatedMeshSceneNode(oSM->getMesh("data/joueur.3ds"), nodeCamera, -1, core::vector3df(0, 0, 100), core::vector3df(0, 0, 0), core::vector3df(0.5, 0.5, 0.5));
    nodePlayer->setMaterialFlag(irr::video::EMF_LIGHTING, true);
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


            core::vector3df posCamera = Wiimote3dToWorld3d(posPlayer);


            //On positionne la caméra à l'endroit du joueur
            nodeCamContainer->setPosition(posCamera);


            //==================== On calcule le champs de vision
            //Horizontalement
            float fHrzFOV1=atan((WORLD_WIDTH/2+posCamera.X)/posCamera.Z);
            float fHrzFOV2=atan((-WORLD_WIDTH/2+posCamera.X)/posCamera.Z);
            float fHrzFOV = fabs(fHrzFOV1-fHrzFOV2);
            // ...

            //Verticalement
            float fVerFOV1=atan((WORLD_HEIGHT/2+posCamera.Y)/posCamera.Z);
            float fVerFOV2=atan((-WORLD_HEIGHT/2+posCamera.Y)/posCamera.Z);
            float fVerFOV = fabs(fVerFOV1-fVerFOV2);

            nodeCamera->setAspectRatio(fHrzFOV/fVerFOV);
            nodeCamera->setFOV(fHrzFOV);
            //--------------------


            core::vector3df facFacing((abs(fVerFOV1)-fVerFOV)/2.0, -(abs(fHrzFOV1)-fHrzFOV)/2.0, 0);
            //core::vector3df facFacing(0, 0, 0);
            nodeCamContainer->setRotation(facFacing);






            core::stringw sCaption = L"fac ver=";
            sCaption += facFacing.X;
            sCaption += L"  fac hrz=";
            sCaption += facFacing.Y;
            oDev->setWindowCaption(sCaption.c_str());

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


        i++;
        //Sleep(100);
    }
}
