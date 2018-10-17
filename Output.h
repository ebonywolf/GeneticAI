#ifndef OUTPUT
#define OUTPUT
#include "Organism.h"

namespace wag{

using Units = std::vector< std::shared_ptr<Organism>> ;
using UnitMap = std::map<int, std::shared_ptr<Organism>>;


struct Progress{
        int generations=0;
        int fitnessMedian=0;
        int highestFitness=0;
        int lowestFitness=0;
        int highestId=0;
        friend std::ostream& operator<<(std::ostream& os,const Progress& p){
            os<<p.generations<<" "<<p.fitnessMedian<<" "<<p.highestFitness<<" "<<" "<<p.lowestFitness<<p.highestId;
            return os;
        }
};


struct EvolutionIO {
    virtual void printOrganism(Organism o){
        
    }
   virtual void readPopulation(){
       
    }
    virtual void printPopulation(Progress o){
       
    }
    
};


struct ProgressIO{
    iostream
    Outputstd( )

};



}



#endif // OUTPUT
