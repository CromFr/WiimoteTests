#include "WiiPos.hpp"


using namespace std;

/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
WiiPos::WiiPos(wiimote** WMTable, wiimote* Wiimote)
{
    WiiPos::WM = Wiimote;
    WiiPos::WMTable = WMTable;

    //Configuration de la wiimote fixe
    wiiuse_set_ir(WiiPos::WM, true);

    //===========================================> Calibrage !
    char cAns;

    //Recherche des donnees de calibrage
    ifstream stCalibData("data/calib.conf");
    if(!stCalibData)
    {
        cout<<endl<<endl<<endl<<endl
            <<" /////////////////////////////////////"<<endl
            <<"O===================================O/"<<endl
            <<"| ""calib.conf"" non trouve !       |/"<<endl
            <<"| Vous devez effectuer le calibrage |/"<<endl
            <<"O===================================O/"<<endl;

        //A la question "voulez vous calibrer", la réponse sera non
        cAns='o';
    }
    else
    {
        string sVar;
        char cChar;
        do
        {
            //On récupère la valeur du champ
            stCalibData>>sVar;
            if(sVar == "DistDotToDotmm")
                stCalibData>>WiiPos::m_fCALDistDotToDotmm;

            else if(sVar == "DistDotToDotpx")
                stCalibData>>WiiPos::m_fCALDistDotToDotpx;

            else if(sVar == "DistWmToDotsmm")
                stCalibData>>WiiPos::m_fCALDistWmToDotsmm;

            else if(sVar == "Ratio")
                stCalibData>>WiiPos::m_fCALRatio;

            else if(sVar == "Scale")
                stCalibData>>WiiPos::m_fCALScale;


            //Déplace le curseur a la fin de la ligne
            do
            {
                stCalibData.get(cChar);
            }
            while(cChar!='\n' && !stCalibData.eof());
        }while(!stCalibData.eof());

        cout<<endl<<endl<<endl<<endl
            <<" /////////////////////////////////////////////////"<<endl
            <<"O===============================================O/"<<endl
            <<"| Voulez vous calibrer la Wiimote ? O/N         |/"<<endl
            <<"|  DotToDot = "<<WiiPos::m_fCALDistDotToDotmm<<"mm = "<<WiiPos::m_fCALDistDotToDotpx<<"px\t\t\t|/"<<endl
            <<"|  WmToDot = "<<WiiPos::m_fCALDistWmToDotsmm<<"mm\t\t\t\t|/"<<endl
            <<"|  Ratio = "<<WiiPos::m_fCALRatio<<"\tScale = "<<WiiPos::m_fCALScale<<"\t|/"<<endl
            <<"O===============================================O"<<endl;

        cin>>cAns;

    }

    if(cAns=='n' || cAns=='N')
    {
        //ne rien faire : les données sont déja enregistrées
    }
    else
    {
        WiiPos::Calibrer();
    }


    cout<<endl<<"Wiimote fixe calibrée et prête"<<endl;
}


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
void WiiPos::Calibrer()
{
    cout<<endl<<"<== Distance entre les deux sources infrarouges (mm) : ";
    cin>>WiiPos::m_fCALDistDotToDotmm;

    cout<<endl<<"<== Distance entre la wiimote et les sources infrarouges (mm) : ";
    cin>>WiiPos::m_fCALDistWmToDotsmm;


    cout<<endl<<"<== Positionnez les sources et appuyez sur (A)..."<<endl;

    bool bLoop = true;
    while(bLoop)
    {
        if(wiiuse_poll(WiiPos::WMTable, 2))
        {
            switch(WiiPos::WM->event)
            {
                case WIIUSE_EVENT:
                    if(IS_JUST_PRESSED(WiiPos::WM, WIIMOTE_BUTTON_A))
                    {
                        if(WiiPos::WM->ir.dot[0].visible && WiiPos::WM->ir.dot[1].visible)
                        {
                            //Calibration !

                            WiiPos::m_fCALDistDotToDotpx = sqrt(   (WiiPos::WM->ir.dot[0].x-WiiPos::WM->ir.dot[1].x)
                                                             *(WiiPos::WM->ir.dot[0].x-WiiPos::WM->ir.dot[1].x)
                                                            +
                                                              (WiiPos::WM->ir.dot[0].y-WiiPos::WM->ir.dot[1].y)
                                                             *(WiiPos::WM->ir.dot[0].y-WiiPos::WM->ir.dot[1].y) );


                            WiiPos::m_fCALRatio = WiiPos::m_fCALDistWmToDotsmm * WiiPos::m_fCALDistDotToDotpx;

                            WiiPos::m_fCALScale = WiiPos::m_fCALDistDotToDotmm / WiiPos::m_fCALDistDotToDotpx;

                            cout<<"==> Wiimote calibree !"<<endl;
                            bLoop=false;
                        }
                        else
                            cout<<"/!\\ La calibration requiers deux sources infrarouges..."<<endl;
                    }
                    break;

                default:
                    break;
            }
        }
    }


    //Enregistrement des données dans un fichier
    cout<<"==> Ecriture du fichier de configuration"<<endl;
    ofstream stCalibData("data/calib.conf");
    if(stCalibData)
    {
        stCalibData<<"DistDotToDotmm "<<WiiPos::m_fCALDistDotToDotmm<<endl
                   <<"DistDotToDotpx "<<WiiPos::m_fCALDistDotToDotpx<<endl
                   <<"DistWmToDotsmm "<<WiiPos::m_fCALDistWmToDotsmm<<endl
                   <<"Ratio "<<WiiPos::m_fCALRatio<<endl
                   <<"Scale "<<WiiPos::m_fCALScale;
    }
    else
    {
        std::cout<<"Erreur : Probleme a l'ouverture/creation du fichier"<<std::endl;
    }

}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
/*try
{
    //code pour actualiser normalement
    WiiPos.GetPosition();
}
catch(int e)
{
    if(e==ERR_NOT_ENOUGH_IRSRC)
    {
        //code pour ne pas actualiser
    }
}*/

Coord3d WiiPos::GetPosition() const
{
    Coord3d posA, posB, posC;

    while(wiiuse_poll(WiiPos::WMTable, 2))
    {
        switch(WiiPos::WM->event)
        {
            case WIIUSE_EVENT:
                //cout<<"Visibility : "<<WiiPos::WM->ir.dot[0].visible<<WiiPos::WM->ir.dot[1].visible<<WiiPos::WM->ir.dot[2].visible<<WiiPos::WM->ir.dot[3].visible<<endl;

                if(WiiPos::WM->ir.dot[0].visible && WiiPos::WM->ir.dot[1].visible)
                {
                    //Positon des dots
                    posA.x = WiiPos::WM->ir.dot[0].x;
                    posA.y = 768-WiiPos::WM->ir.dot[0].y;
                    posB.x = WiiPos::WM->ir.dot[1].x;
                    posB.y = 768-WiiPos::WM->ir.dot[1].y;

                    //Position du centre
                    posC.x = (posA.x+posB.x)/2;
                    posC.y = (posA.y+posB.y)/2;

                    //Calcul de la distance en px séparant les dots
                    float fDistDotToDot = sqrt( (WiiPos::WM->ir.dot[0].x-WiiPos::WM->ir.dot[1].x)
                                                 *(WiiPos::WM->ir.dot[0].x-WiiPos::WM->ir.dot[1].x)
                                                +
                                                  (WiiPos::WM->ir.dot[0].y-WiiPos::WM->ir.dot[1].y)
                                                 *(WiiPos::WM->ir.dot[0].y-WiiPos::WM->ir.dot[1].y) );

                    float fY=-WiiPos::m_fCALRatio / fDistDotToDot;
                    float fX=-(posC.x-512) * WiiPos::m_fCALScale * fY / WiiPos::m_fCALDistWmToDotsmm;
                    float fZ= (posC.y-384) * WiiPos::m_fCALScale * fY / WiiPos::m_fCALDistWmToDotsmm;

                    //cout<<"X="<<fX<<"\tY="<<fY<<"\tZ="<<fZ<<endl;

                    return Coord3d(fX, fY, fZ);
                }
                else
                    throw EXC_WIIPOS_NOT_ENOUGH_IRSRC;



                break;

            default:
                break;
        }
    }

    throw EXC_WIIPOS_NO_EVENT;
}
