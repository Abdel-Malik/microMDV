/*///////////////////////////////////////////////////////////////
// * Author : Abdel-Malik Bouhassoun
// * Date : 31 Mai 2017
// Ce header contient la classe intermédiaire Générale
//Elle regroupe les fonctionnalitées des classes intermediaire des 3 proets
//ABS-Conso-Gear_shift
///////////////////////////////////////////////////////////////*/
#ifndef _IntermediaireG_h_
#define _IntermediaireG_h_

#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>

#include "../Gear_shift/E_ModeConduite.h"

class IntermediaireG{

    /**attributs**/
    //Constantes_véhicule
    public:
    static const int GEAR_MIN = 0;
    static const int GEAR_MAX = 6;

    static constexpr double VITESSE_MIN = 0;
    static constexpr double VITESSE_MAX = 125;
    static constexpr double PUISSANCE_MAX = 125;
    static constexpr double COUPLE_MAX = 125;
    static constexpr double RPM_MIN = 650;
    static constexpr double RPM_MAX = 2500;
    static constexpr double CHARGE_MIN = 0;
    static constexpr double CHARGE_MAX = 1;

    double maxX = 130*130,maxY=170*170,maxZ=9.25;
    //Données Véhicule
    private:
    bool ralentiMot;
    int currentGear;
    int nbRoues;
    std::vector<double> rayonRoues;

    //Interaction Véhicule
    double vitesseVehicule;
    double puissanceMoteur;
    double consommation;
    double chargeMoteur;
    double chargeFrein;
    std::vector<double> vitesseRoues;
    std::vector<double> freinRoues;

    //Interpolation
    int echAxe;

    //autre
    int event = 0;

    /**Méthodes**/

    public:
    //Constructeur
    IntermediaireG(){
        recuperationDonnees();
        initRoues();
    };
    /*Méthodes publiques*/

    void majDonnees(){
        recupVitesseRoues();
    }
    /*getter*/
    //données véhicule
    bool ralentiMoteur(){
        return ralentiMot;
    };
    int getGear(){
        return currentGear;
    };
    double getNbRoues(){
        return nbRoues;
    };
        //en mètre
    double getRayonRoues(int i){
        return rayonRoues[i];
    };

    //interaction véhicule
    double getVitesse(){
        return vitesseVehicule;
    };
    void setVitesse(double a){
        vitesseVehicule = a;
    };
    double getPuissanceMoteur(){
        return puissanceMoteur;
    };
    double getConsommation(){
        return consommation;
    };
    double getChargeMoteur(){
        return chargeMoteur;
    };

    double getChargeFrein(){
        return chargeFrein;
    };

    double getVitesseAngulaire(int i){
        return vitesseRoues[i];
    };

    //Interpolation
    int getEchAxe(){
        return echAxe;
    };

    double* getAxeZ(int gear, ModeConduite mode){
        double* valeurs = new double[echAxe*echAxe];
        double ci = (RPM_MAX/echAxe);
        double cj = (COUPLE_MAX/echAxe);
        double x,y;
        for(int i = 0; i < echAxe; i++){
            x = i*RPM_MAX/echAxe;
            for(int j = 0; j < echAxe; j++){
                y = j*COUPLE_MAX/echAxe;
                valeurs[i*echAxe+j] = 188+215*n(n((x-1300)*(x-1300),maxX) + 3.7*n((y-1600)*(y-1600),maxY) + n((x-1300)*(y-1600),maxX*maxY),maxZ);
            }
        }
        return valeurs;
    };
    double* getAxeZ(){
        double* valeurs = new double[echAxe*echAxe];
        double ci = (RPM_MAX/echAxe);
        double cj = (COUPLE_MAX/echAxe);
        double x,y;
        for(int i = 0; i < echAxe; i++){
            x = i*RPM_MAX/echAxe;
            x=x/10;
            for(int j = 0; j < echAxe; j++){
                y = j*COUPLE_MAX/echAxe;
                y=y/10;
                valeurs[i*echAxe+j] = 189+212*n(n((x-130)*(x-130),maxX) + 7*n((y-170)*(y-170),maxY) + 1.25*n((x-130)*(y-170),maxX*maxY),maxZ);
            }
        }
        return valeurs;
    };

    double n(double x, double maxi){
        return x/maxi;
    }


    /*setter*/
    void setFreinageRoue(int i, double chrg){
        freinRoues[i] = chrg;
    };

    void majMDV(bool a, int g, double v, double p, double cM, double cF, double conso){
        ralentiMot = a;
        currentGear = g;
        nbRoues = 6;
        vitesseVehicule = v;
        puissanceMoteur = p;
        chargeMoteur = cM;
        chargeFrein = cF;
        consommation = conso;
        echAxe = 20;
    }

    /**Méthodes privées**/

    private:
    void recuperationDonnees(){
        ralentiMot = false;
        nbRoues = 6;
        echAxe = 20;
    };

    void initRoues(){
        for(int i = 0; i < nbRoues ; i++){
            rayonRoues.push_back(0.4719);
            vitesseRoues.push_back(0);
            freinRoues.push_back(0);
        }
        //recupVitesseRoues();
    }

    void recupVitesseRoues(){
        event++;
        for(unsigned int i = 0; i < vitesseRoues.size() ; i++){
            if(event != 5 && event !=41 && event != 26)
                vitesseRoues[i] = vitesseVehicule*(1-freinRoues[i])/(rayonRoues[i]*3.6);
        }
        if(event != 5 && event != 16 && event !=18 &&  event != 26)
            vitesseVehicule -= 0.01*vitesseVehicule*chargeFrein;
        if(event == 4){
            std::cout << "Event 1 -- freinage" << std:: endl;
            chargeFrein = 1;
            for(unsigned int i = 0; i < vitesseRoues.size() ; i++){
                vitesseRoues[i] = vitesseVehicule*(1-chargeFrein)/(rayonRoues[i]*3.6);
                freinRoues[i] = chargeFrein;
            }
        }
        if(event == 15){
            std::cout << "Event 1 -- relacher" << std:: endl;
            chargeFrein = 0.52;
            for(unsigned int i = 0; i < vitesseRoues.size() ; i++){
                vitesseRoues[i] += 2;
            }
        }
        if(event == 25){
            std::cout << "Event 2 -- appuie2" << std:: endl;
            chargeFrein = 0.9;
            for(unsigned int i = 0; i < vitesseRoues.size() ; i++){
                vitesseRoues[i] -= 5.1;
            }
        }
        if(event == 40){
            std::cout << "Event 2 -- lâcher" << std:: endl;
            chargeFrein = 0;
        }
    }
};
#endif
