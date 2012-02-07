#include "WiiPos.hpp"


using namespace std;

/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
Wiimote3d::Wiimote3d(float x, float y, float z)
{
    Wiimote3d::x=x;
    Wiimote3d::y=y;
    Wiimote3d::z=z;
}

void Wiimote3d::set(float x, float y, float z)
{
    Wiimote3d::x=x;
    Wiimote3d::y=y;
    Wiimote3d::z=z;
}





/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
WiiPos::WiiPos(wiimote** WMTable, wiimote* Wiimote, int nConnectedWM, ConfigFile* Config)
{
    m_WMTable = WMTable;
    m_WM = Wiimote;
    m_Config = Config;
    m_nConnectedWM = nConnectedWM;


    //Configuration de la wiimote fixe
    wiiuse_set_ir(m_WM, true);

    //===========================================> Calibrage !
    char cAns;

    //Recherche des donnees de calibrage

    try
    {
        m_Config->Print();

        istringstream* iss;
        iss=new istringstream(Config->GetValue("DistDotToDotmm"));         *iss>>m_fCALDistDotToDotmm;          delete iss;
        iss=new istringstream(Config->GetValue("DistDotToDotpx"));         *iss>>m_fCALDistDotToDotpx;          delete iss;
        iss=new istringstream(Config->GetValue("DistWmToDotsmm"));         *iss>>m_fCALDistWmToDotsmm;          delete iss;
        iss=new istringstream(Config->GetValue("DistWmToScreenCenter"));   *iss>>m_fCALDistWmToScreenCenter;    delete iss;
        iss=new istringstream(Config->GetValue("Ratio"));                  *iss>>m_fCALRatio;                   delete iss;
        iss=new istringstream(Config->GetValue("Scale"));                  *iss>>m_fCALScale;                   delete iss;



        cout<<endl<<endl<<endl<<endl
            <<" /////////////////////////////////////////////////"<<endl
            <<"O===============================================O/"<<endl
            <<"| Voulez vous calibrer la Wiimote ? O/N         |/"<<endl
            <<"|  DotToDot = "<<m_fCALDistDotToDotmm<<"mm = "<<m_fCALDistDotToDotpx<<"px\t\t\t|/"<<endl
            <<"|  WmToDot = "<<m_fCALDistWmToDotsmm<<"mm\tWmToScreen = "<<m_fCALDistWmToScreenCenter<<"\t|/"<<endl
            <<"|  Ratio = "<<m_fCALRatio<<"\tScale = "<<m_fCALScale<<"\t|/"<<endl
            <<"O===============================================O"<<endl;

        cin>>cAns;
    }
    catch(int ex)
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

    if(cAns=='n' || cAns=='N')
    {
        //ne rien faire : les données sont déja enregistrées
    }
    else
    {
        Calibrer();
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
    cin>>m_fCALDistDotToDotmm;

    cout<<endl<<"<== Distance entre la wiimote et les sources infrarouges (mm) : ";
    cin>>m_fCALDistWmToDotsmm;


    cout<<endl<<"<== Positionnez les sources et appuyez sur (A)..."<<endl;

    bool bLoop = true;
    while(bLoop)
    {
        if(wiiuse_poll(m_WMTable, m_nConnectedWM))
        {
            switch(m_WM->event)
            {
                case WIIUSE_EVENT:
                    if(IS_JUST_PRESSED(m_WM, WIIMOTE_BUTTON_A))
                    {
                        if(m_WM->ir.dot[0].visible && m_WM->ir.dot[1].visible)
                        {
                            //Calibration !

                            m_fCALDistDotToDotpx = sqrt(   (m_WM->ir.dot[0].x-m_WM->ir.dot[1].x)
                                                             *(m_WM->ir.dot[0].x-m_WM->ir.dot[1].x)
                                                            +
                                                              (m_WM->ir.dot[0].y-m_WM->ir.dot[1].y)
                                                             *(m_WM->ir.dot[0].y-m_WM->ir.dot[1].y) );


                            m_fCALRatio = m_fCALDistWmToDotsmm * m_fCALDistDotToDotpx;

                            m_fCALScale = m_fCALDistDotToDotmm / m_fCALDistDotToDotpx;

                            cout<<"==> Wiimote calibree !"<<endl;

                            string tmp;
                            tmp=m_fCALDistDotToDotmm;        m_Config->SetValue("DistDotToDotmm", tmp);
                            tmp=m_fCALDistDotToDotpx;        m_Config->SetValue("DistDotToDotpx", tmp);
                            tmp=m_fCALDistWmToDotsmm;        m_Config->SetValue("DistWmToDotsmm", tmp);
                            tmp=m_fCALDistWmToScreenCenter;  m_Config->SetValue("DistWmToScreenCenter", tmp);
                            tmp=m_fCALRatio;                 m_Config->SetValue("Ratio", tmp);
                            tmp=m_fCALScale;                 m_Config->SetValue("Scale", tmp);

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
}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
Wiimote3d WiiPos::GetPosition() const
{
    Wiimote3d posA, posB, posC;

    while(wiiuse_poll(m_WMTable, m_nConnectedWM))
    {
        switch(m_WM->event)
        {
            case WIIUSE_EVENT:
                if(m_WM->ir.dot[0].visible && m_WM->ir.dot[1].visible)
                {
                    //Positon des dots
                    posA.x = m_WM->ir.dot[0].x;
                    posA.y = 768-m_WM->ir.dot[0].y;
                    posB.x = m_WM->ir.dot[1].x;
                    posB.y = 768-m_WM->ir.dot[1].y;

                    //Position du centre
                    posC.x = (posA.x+posB.x)/2;
                    posC.y = (posA.y+posB.y)/2;

                    //Calcul de la distance en px séparant les dots
                    float fDistDotToDot = sqrt( (m_WM->ir.dot[0].x-m_WM->ir.dot[1].x)
                                                 *(m_WM->ir.dot[0].x-m_WM->ir.dot[1].x)
                                                +
                                                  (m_WM->ir.dot[0].y-m_WM->ir.dot[1].y)
                                                 *(m_WM->ir.dot[0].y-m_WM->ir.dot[1].y) );


                    // TODO (Administrateur#1#): Gestion du pitch
                    float fPitch = (-m_WM->orient.pitch/180)*3.14159265;//;
                    cout<<fPitch<<endl;
                    //*cos((fPitch/180)*3.14159265)

                    float fY0=(-m_fCALRatio / fDistDotToDot);
                    float fX0=-(posC.x-512) * m_fCALScale * fY0 / m_fCALDistWmToDotsmm;
                    float fZ0= ((posC.y-384) * m_fCALScale * fY0 / m_fCALDistWmToDotsmm);

                    //Methode Alex
                    float fY1=cos(fPitch)*fY0 + sin(fPitch)*fZ0;
                    float fX1=fX0;
                    float fZ1=-sin(fPitch)*fY0 + cos(fPitch)*fZ0 + m_fCALDistWmToScreenCenter;

                    //Methode thibaut
                    //float fX1=fX0;
                    //float fY1=cos(fPitch)*fY0 - sin(fPitch)*fZ0;
                    //float fZ1=cos(fPitch)*fZ0 + sin(fPitch)*fX0 + m_fCALDistWmToScreenCenter;

                    //cout<<"X="<<fX<<"\tY="<<fY<<"\tZ="<<fZ<<endl;

                    return Wiimote3d(fX1, fY1, fZ1);
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
