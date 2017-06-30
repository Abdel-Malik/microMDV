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
    double ratioCW = 11.2;
    //--
    double CONSO_RALENTI = 3.46; // l/h
    double RALENTI_MIN = 580;
    double ACTIVATION_RALENTI = 615;
    double ROT_MIN = 640; // (= RALENTI_MAX)
    double ROT_MAX = 2500;

    //variables
    bool acceleration = false;
    bool activationRalenti = false;
    double omega = 0; // tr/min
    double coeffMoindreCarreP[3];
    double coeffMoindreCarreCons[3];
    bool fonctionne = false; //permet de savoir si le moteur ne fonctionne plus (éteint <=> calage)
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

        void majOmegaEngine(double rotationRoue, double coeff){
            std::cout << omega << "rotation\n" << std::endl;
            if(omega < RALENTI_MIN){
                fonctionne = false; //calage
                activationRalenti = false;
                rupteur = false;
                omega = rotationRoue;
            }else if(omega <= ACTIVATION_RALENTI){
                activationRalenti = true;
                rupteur = false;
            }else if(omega > ROT_MIN){
                fonctionne = true; //calage
                activationRalenti = false;
                rupteur = false;
            }else if(omega > ROT_MAX){
                rupteur = true;
            }
            omega += augmentationRegime(coeff)+rotationRoue*(1-coeff);
        }

        double getVitesse(){
            return omega;
        }
        //Watt
        double puissanceFournie(double coeff){
            double res = 0;
            std::cout << activationRalenti << " -- bool\n" << std::endl;
            if(fonctionne && acceleration && !rupteur)
                return coeff*(coeffMoindreCarreP[0]*omega*omega+coeffMoindreCarreP[1]*omega+coeffMoindreCarreP[2])*1000;
            if(activationRalenti && fonctionne && !acceleration && !rupteur)
                return coeff*(coeffMoindreCarreP[0]*omega*omega+coeffMoindreCarreP[1]*omega+coeffMoindreCarreP[2])*270;

            return res;
        }
        //N.m
        double coupleFourni(double coeff){
            return (30*puissanceFournie(coeff))/(omega*M_PI);
        }
        //g/kW.h
        double essenceConsommee(){
            return coeffMoindreCarreCons[0]*omega*omega+coeffMoindreCarreCons[1]*omega+coeffMoindreCarreCons[2];
        }
        void setAcceleration(bool acc){
            if(acc){
                if(activationRalenti){
                    activationRalenti = false;
                }
            }
            acceleration = acc;
        }
        void demarrerMoteur(double rotationRoue){
            if(!fonctionne){
                Sleep(10);
                activationRalenti = true;
                Sleep(10);
                if(rotationRoue > ACTIVATION_RALENTI)
                    omega = rotationRoue;
                else
                    omega = ACTIVATION_RALENTI;
                fonctionne = true;
            }
        }

        double puissanceResistante(double coeff){
            int c = 3;
            if(omega < ROT_MIN)
                c = 1;
            return coeff*ratioCW*omega*c;
        }
    protected:
    private:

        double augmentationRegime(double coeff){
            return (puissanceFournie(coeff)-puissanceResistante(coeff))/ratioCW;
        }

};

#endif // ENGINE_H
