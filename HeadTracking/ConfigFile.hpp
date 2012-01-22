#ifndef CONFIGFILE_HPP_INCLUDED
#define CONFIGFILE_HPP_INCLUDED


/* Exemple de config file

#Commentaire personnalis�...
#Par exemple cette valeur m'int�resse :
MaValeur 10;

#il faut mettre un ';' pour terminer un champ de valeur
#ceci permet de faire �a :
LeNomDeMonApp L'application des h�ros !;

#Ou encore ceci pour r�cup�rer un vecteur :
MonVecteur 10.5 18.0 80.6;

#Tabulations and spaces are ignored


*/


#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>

struct ConfigLine
{
    int curpos;
    std::string name;
    std::string value;
};


class ConfigFile
{
    public:

    ///Default constructor
    ConfigFile();

    ///Create and load the config file
    ConfigFile(std::string sPath);


    ///Load the config file
    void Load(std::string sPath);

    ///Return the string that reprensents the value of the config line
    /// it can be "10" or "Thibaut CHARLES" or even "10.5 18.0 80.6" for a vector
    std::string GetValue(std::string sName);

    /// Delete the line and rewrite it
    void SetValue(std::string sName, std::string sValue);


    private:
    std::vector<struct ConfigLine> m_Config;

};

#endif // CONFIGFILE_HPP_INCLUDED
