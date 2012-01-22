
#include "ConfigFile.hpp"

using namespace std;
/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
ConfigFile::ConfigFile()
{

}

/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
ConfigFile::ConfigFile(string sPath)
{
    Load(sPath);
}

/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
void ConfigFile::Load(string sPath)
{
    //Open the file
    ifstream strifConfigFile(sPath.c_str());

    if(!strifConfigFile)
    {
        //Create the file
        ofstream strofConfigFile(sPath.c_str());
        strofConfigFile.close();
    }

    string sName;
    char cChar;
    do
    {
        //Ignore \n, spaces or tabulations
        do
        {
            strifConfigFile.get(cChar);
        }while( (cChar==' ' || cChar=='\t' || cChar=='\n' ) && !strifConfigFile.eof());

        //The cursor is after the first interessant caracter

        if(cChar=='#')//Comments
        {
            //cout<<"It's a comment line"<<endl<<"     ";
            string sLine;
            getline(strifConfigFile, sLine);
            //cout<<sLine<<endl;
        }
        else
        {
            //cout<<"Humm... let's read that !"<<endl;
            struct ConfigLine CurrConfigLine;

            //Getting line name
            string sWord;
            strifConfigFile>>sWord;

            CurrConfigLine.name = cChar+sWord;


            //Ignore spaces and tabulations between name and values
            do
            {
                strifConfigFile.get(cChar);
            }while(cChar==' ' || cChar=='\t');


            while(cChar!=';')
            {
                CurrConfigLine.value+=cChar;
                strifConfigFile.get(cChar);
            }

            m_Config.push_back(CurrConfigLine);
            //cout<<"Adding a line : "<<endl<<"     Name="<<CurrConfigLine.name<<endl<<"     Value(s)="<<CurrConfigLine.value<<endl;

        }

    //cout<<"-------------------------\n";
    }while(!strifConfigFile.eof());

    cout<<"Config file loaded : "<<sPath<<endl;

}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
std::string ConfigFile::GetValue(std::string sName)
{
    for(unsigned int i=0 ; i<m_Config.size() ; i++)
    {
        if(m_Config[i].name == sName)
            return m_Config[i].value;
    }
    return "NO VALUE FOUND";
}















