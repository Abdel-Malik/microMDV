#ifndef MDV_H
#define MDV_H

#include "Engine.h"
#include "IntermediaireG.h"
#include "../Gear_shift/GrilleInterpolation.h"
class MDV
{
    //temp
    double TEMPS_CYCLE = 0.005; // s (5ms)
    GrilleInterpolation BSFC;
    double* axeZ;

    //constantes
    double M_PI = 3.1415926;
    double M_MIN_TO_M_S = 1.0/60;
    double MASSE_VOLUMIQUE_AIR = 1.22; //kg/m^3

    //IntermediaireG
    IntermediaireG i = IntermediaireG();

    //transmission
    double RATIO_T_PONT = 5.82;
    double rendementTransmission = 0.92;
    double rapportTransmission[7] = {4.24,3.36,1.91,1.42,1,0.72,0.62}; // gear : R - 1 - 2 - 3 - 4 - 5 - 6
    double plageVitesse[7][2] = {{0,0},{1.643,6.318},{2.89,11.114},{3.887,14.949},{5.519,21.227},{7.665,29.482},{8.902,34.238}};

    //moteur
    Engine moteur;
    double pFrein;
    double pAcc;
    double pEmb;

    //structure
    double surfaceFrontale = 5; //m^2
    double trainee = 0.8; //Cx
    double coeffResistanceRoulement = 0.008;
    double masse = 16000; // kg

    //roue
    double rayonRoue = 0.4719; // mètre

    //vehicule
    double v_auto = 0; // m/s
    int gear = 1;
    bool pointMort = true;

    public:
        void test(){
            std::cout << i.getVitesse() << std::endl;
        }
        MDV(){
            double a[3] = {-0.0001502,0.5648365,-263.93706};
            double b[3] = {0.0000352,-0.0985985,257.98788};

            moteur = Engine(a,b);
            axeZ = i.getAxeZ();

            BSFC = GrilleInterpolation(ModeConduite::ECO, axeZ, 20);
        }

        void fct(){
            v_auto = 0;
            moteur.demarrerMoteur(WheelToRpm(v_auto));
            pFrein = 0;
            pAcc = 1;
            majInter();
        }
        double getVitesse(){
            return v_auto*3.6;
        }
        void setpAcc(double a){
            pAcc = a;
        }
    void avancer(int t){
        if(t%1 == 0){
            std::cout << v_auto << "m/s || "<<v_auto*3.6 <<"km/h - temps :"<<TEMPS_CYCLE*t<<"s || gear ="<<gear<<"\n"<<std::endl;
        }
        accelerationGagnee();
        moteur.majOmegaEngine(WheelToRpm(v_auto),pAcc,gear);
        majInter();
    }
        double vitMotToWheel(){
            std::cout << moteur.getVitesse() << std::endl;
            return (moteur.getVitesse()/(RATIO_T_PONT*rapportTransmission[gear])*(2*M_PI*rayonRoue)*M_MIN_TO_M_S);
        }
        double WheelToRpm(double Wheel){
            return (Wheel*(RATIO_T_PONT*rapportTransmission[gear])/(2*M_PI*rayonRoue)*(1/M_MIN_TO_M_S));
        }

        IntermediaireG* getI(){
            return &i;
        }

        void setGear(int g){
            gear = g;
        }
    protected:
    private:


    double resistanceAir(double v){
        return (.5*MASSE_VOLUMIQUE_AIR*surfaceFrontale*trainee*v*v);
    }
    double resistanceRoulement(){
        return coeffResistanceRoulement*masse*9.81;
    }


    void accelerationGagnee(){
        double v = plageVitesse[gear][1]*pAcc+plageVitesse[gear][0]*(1-pAcc);
        double Fmoteur = 0.5*masse*((v*v)-(v_auto*v_auto))*rendementTransmission;
        if(Fmoteur < 0){
            Fmoteur = Fmoteur*(0.6*(moteur.getVitesse()/2500));
        }
        double Fres = resistanceAir(v_auto)+resistanceRoulement();
        v_auto += ((Fmoteur-Fres)/masse)*TEMPS_CYCLE;
    }


    //Voisin inf' gauche
    void majInter(){
        i.majMDV(moteur.activationRalenti, gear, v_auto, moteur.puissanceFournie(pAcc), pAcc, pFrein, axeZ[(int)(20*moteur.getVitesse()/2500)+(int)(20*moteur.coupleFourni(pAcc)/1700)]);
    }
/*
    void majInter(){
        i.majMDV(moteur.activationRalenti, gear, v_auto, moteur.puissanceFournie(pAcc), pAcc, pFrein, BSFC.interpolerPoint(moteur.getVitesse(),pAcc));
    }*/
};

#endif // MDV_H
