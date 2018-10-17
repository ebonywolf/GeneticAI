#ifndef OUTPUT
#define OUTPUT
#include "Organism.h"

namespace wag {

using Units = std::vector< std::shared_ptr<Organism>> ;
using UnitMap = std::map<int, std::shared_ptr<Organism>>;


struct Progress {
    int generations = 0;
    int fitnessMedian = 0;
    int highestFitness = 0;
    int lowestFitness = 0;
    int highestId = 0;
    friend std::ostream& operator<< ( std::ostream& os, const Progress& p ) {
        os << "Gen:"<<p.generations << " Med:" << p.fitnessMedian << " high:" << p.highestFitness <<  " low:" << p.lowestFitness << " winner"<< p.highestId;
        return os;
    }
};


struct EvolutionIO {
    virtual ~EvolutionIO(){};
    virtual void printOrganism ( Organism o ) {

    }
    virtual UnitMap readPopulation() {

    }
    virtual void printPopulation ( UnitMap, int gen ) {

    }
};
using EvolutionIOptr= std::shared_ptr<EvolutionIO>;

template<class OrganismType>
struct EvolutionStd: public EvolutionIO{
    std::ostream& os;
    std::istream& is;
    
    EvolutionStd(std::ostream& os = std::cout, std::istream& is = std::cin)
    :os(os),is(is)
    {
    
    }
    ~EvolutionStd(){
    }
    virtual void printOrganism ( Organism o ) {
        os<<o<<std::endl;
        
    }
    virtual UnitMap readPopulation() {
        UnitMap units;
        Organism basic;
        int i =0;
        try{
            while(is.good() && !is.eof() && is.peek() != EOF){
              //  std::cout << "Reading line"<<++i<< std::endl;
                Organismptr u = std::make_shared<OrganismType>();

                is>> basic;
                auto alce =u.get();
                *alce = basic;
                units[u->id] = u;
            }
        }catch(...){
        }

        return units;
    }
    virtual void printPopulation ( UnitMap mapa, int gen ) {
        for(auto& x: mapa){
            os << x.second<<std::endl;        
        }
        
    }
};

struct ProgressIO {
    virtual void printProgress(Progress){}
    virtual void saveProgress(Progress)=0;
    virtual Progress loadProgress()=0;
    
};
using ProgressIOptr = std::shared_ptr<ProgressIO>;

struct ProgressStd: public ProgressIO {
    std::ostream& os;
    std::istream& is;
    ProgressStd(std::ostream& os = std::cout,std::istream& is = std::cin ):
        os(os),is(is){}
    ~ProgressStd(){}
    virtual void printProgress ( Progress progress) {
        
        os<<progress<<std::endl;
    }
    virtual void saveProgress ( Progress ) {}
    virtual Progress loadProgress() {}
};



}



#endif // OUTPUT
