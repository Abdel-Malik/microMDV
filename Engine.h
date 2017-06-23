#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <vector>
#include <math.h>
#include <string.h>
#include <windows.h>

class Engine
{
    //constantes
    double M_PI = 3.1415926;
    //--
    double CONSO_RALENTI = 3.46; // l/h
    double RALENTI_MIN = 480;
    double ACTIVATION_RALENTI = 540;
    double ROT_MIN = 900; // (= RALENTI_MAX)
    double ROT_MAX = 2500;

    //variables
    bool acceleration = false;
    bool activationRalenti = false;
    double rot = 0; // tr/min
    double coeffMoindreCarreP[3];
    double coeffMoindreCarreCons[3];
    bool fonctionne = false; //permet de savoir si le moteur ne fonctionne plus (éteint = calage => rot = 0)
    bool rupteur = false;

    public:
        Engine(){
        };
        Engine(double coeffP[], double coeffCons[]){
            for(int i = 0; i <3 ; i++){
                coeffMoindreCarreP[i] = coeffP[i];
                coeffMoindreCarreCons[i] = coeffCons[i];
            }

        }

        void newRotSpeed(double rot){
            std::cout << rot << "rotation\n" << std::endl;
            if(rot > ROT_MAX)
                rupteur = true;
            else
                rupteur = false;
            this->rot = rot;
        }

        double getVitesse(){
            return rot;
        }

        double puissanceFournie(){
            if(acceleration && !rupteur)
                return (coeffMoindreCarreP[0]*rot*rot+coeffMoindreCarreP[1]*rot+coeffMoindreCarreP[2])*1000;
            else
                return 0;
        }
        double coupleFourni(){
            return (30*puissanceFournie())/(rot*M_PI);
        }
        double essenceConsommee(){
            double res = CONSO_RALENTI;
            if(!activationRalenti)
                res = coeffMoindreCarreCons[0]*rot*rot+coeffMoindreCarreCons[1]*rot+coeffMoindreCarreCons[2];
            return res;
        }
        void setAcceleration(bool acc){
            if(acc){
                if(activationRalenti){
                    activationRalenti = false;
                    if(rot < ROT_MIN)
                        rot = ROT_MIN;
                }
            }
            acceleration = acc;
        }
        void demarrerMoteur(){
            if(!fonctionne){
                Sleep(10);
                activationRalenti = true;
                Sleep(10);
                rot = ACTIVATION_RALENTI;
                fonctionne = true;
            }
        }
    protected:
    private:

};

#endif // ENGINE_H
