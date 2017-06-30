#ifndef MDV_H
#define MDV_H

#include "Engine.h"
#include "../ABS/outilDichotomie.h"

class MDV
{
    //temp
    double TEMPS_CYCLE = 0.1; // s


    //constantes
    double M_PI = 3.1415926;
    double M_MIN_TO_M_S = 1.0/60;
    double MASSE_VOLUMIQUE_AIR = 1.22; //kg/m^3

    BornesDichotomie bornes = BornesDichotomie();

    //transmission
    double RATIO_T_PONT = 5.82;
    double rendementTransmission = 0.92;
    double rapportTransmission[7] = {4.24,3.36,1.91,1.42,1,0.72,0.62}; // gear : R - 1 - 2 - 3 - 4 - 5 - 6

    //moteur
    Engine moteur;
    double pFrein;
    double pAcc;
    double pEmb;

    //structure
    double surfaceFrontale = 5; //m^2
    double trainee = 0.8; //Cx
    double resistanceRoulement = 0.008;
    double masse = 16000; // kg

    //roue
    double rayonRoue = 0.4719; // mètre

    //vehicule
    double v_auto = 0; // m/s
    int gear = 1;
    bool pointMort = true;

    public:
        MDV(){
            double a[3] = {-0.0001502,0.5648365,-263.93706};
            double b[3] = {0.0000352,-0.0985985,257.98788};

            moteur = Engine(a,b);
        }

        void fct(){
            v_auto = 0;
            moteur.demarrerMoteur(WheelToRpm(v_auto));
            pFrein = 0;
            pAcc = 0;
            pEmb = 1;
            for(int i =0; i <10; i++){
                avancer();
            }

        }
        double vitMotToWheel(){
            std::cout << moteur.getVitesse() << std::endl;
            return (moteur.getVitesse()/(RATIO_T_PONT*rapportTransmission[gear])*(2*M_PI*rayonRoue)*M_MIN_TO_M_S);
        }
        double WheelToRpm(double Wheel){
            return (Wheel*(RATIO_T_PONT*rapportTransmission[gear])/(2*M_PI*rayonRoue)*(1/M_MIN_TO_M_S));
        }
    protected:
    private:

    void avancer(){
        if(pointMort){
            pointMort = false;
            gear = 1;
            moteur.setAcceleration(false);
        }
        std::cout << v_auto*3.6 <<"km/h\n"<<std::endl;
        accelerationGagnee();
        moteur.majOmegaEngine(WheelToRpm(v_auto),(pEmb));
    }

    double resistanceAir(double v){
        return (.5*MASSE_VOLUMIQUE_AIR*surfaceFrontale*trainee*v*v);
    }
    double coupleALaRoue(){
        return ((moteur.coupleFourni(1-pEmb)*RATIO_T_PONT*rapportTransmission[gear])*rendementTransmission)*rayonRoue;
    }
    double deltaPuissance(){
        return -(resistanceAir(v_auto));
        //return (coupleALaRoue())-(resistanceAir(v_auto));
    }

    void accelerationGagnee(){
        v_auto += (deltaPuissance()/masse)*TEMPS_CYCLE;
    }/*double accelerationGagnee(){
        double Tab[3] = {(borneHauteAcceleration()-borneBasseAcceleration())/2,borneBasseAcceleration(),borneHauteAcceleration()};
        while(energieAcc(Tab[0])<-0.01 || energieAcc(Tab[0])>0.01){
            if(energieAcc(Tab[0])<-0.01){
                Tab[1] = Tab[0];
                Tab[0] = (Tab[2]+Tab[1])/2;
            }else{
                Tab[2] = Tab[0];
                Tab[0] = (Tab[2]+Tab[1])/2;
            }
        }
        std::cout << Tab[0] <<"\n"<<std::endl;
        v_auto += Tab[0]*TEMPS_CYCLE*0.5;
    }*/

    double borneHauteAcceleration(){
        double y = 0;
        while(energieAcc(y)<0)
            y+=1;
        return y;
    }
    double borneBasseAcceleration(){
        double y = 0;
        while(energieAcc(y)>0)
            y-=1;
        return y;
    }

    double energieAcc(double y){
        double x = y*(2*resistanceAir(1)*TEMPS_CYCLE*v_auto+y*resistanceAir(1)*TEMPS_CYCLE*TEMPS_CYCLE+masse)+resistanceAir(v_auto)-deltaPuissance();
        return x;
    }

};

#endif // MDV_H
