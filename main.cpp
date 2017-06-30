#include <iostream>
#include "MDV.h"
#include "../Gear_shift/GearBoxAI.h"
#include "../Conso_rejet/Essai/StockageConsommation.h"
using namespace std;

int main()
{
    MDV vehicule = MDV();
    IntermediaireG* i = vehicule.getI();
    vehicule.fct();
    GearBoxAI gb = GearBoxAI(i);
    StockageConsommationGeneral scg = StockageConsommationGeneral(i);
    bool b = false;
    for(int i =0; i <600; i++){
        vehicule.avancer(i);
        vehicule.setGear(gb.optimiserRapport());
        if(vehicule.getVitesse() > 30){
            vehicule.setpAcc(0);
            b = true;
        }
        if(vehicule.getVitesse() < 14&&b)
            vehicule.setpAcc(1);
        scg.calculConsommationInstantanee();
    }
    scg.calculConsommationMoyenne();
    scg.affichageConsommationMoyenne();
    return 0;
}
