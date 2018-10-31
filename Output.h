#ifndef OUTPUT
#define OUTPUT
#include "Organism.h"
#include <stdio.h>
#include <cstdlib>

namespace wag {

using Units = std::vector< std::shared_ptr<Organism>> ;
using UnitMap = std::map<int, std::shared_ptr<Organism>>;
using Pontos =   std::vector<std::pair<double, double> >;

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

    virtual UnitMap readPopulation(int gen) {

    }
    virtual void printPopulation ( Units, int gen ) {

    }
};
using EvolutionIOptr= std::shared_ptr<EvolutionIO>;


struct ProgressIO {
    virtual void printProgress(Progress){}
    virtual void saveProgress(Progress)=0;
    virtual Progress loadProgress()=0;
};
using ProgressIOptr = std::shared_ptr<ProgressIO>;



struct EvolutionMemory: public EvolutionIO{
    std::vector<UnitMap> populations;
    int size;
      EvolutionMemory()
    {
         populations.resize(100);
    }
    EvolutionMemory(int maxSize)
    {
        populations.resize(maxSize);
    }
    ~EvolutionMemory(){
    }

    virtual UnitMap readPopulation(int gen) {
        gen = gen%populations.size();
       
        return populations[gen];
    }
    virtual void printPopulation ( Units units, int gen ) {
        using namespace std;
        
        gen = gen%populations.size();
       
        UnitMap mapa;
        for (auto& x : units ){
            mapa[x->id ]=x;
        }
        populations[gen] = mapa;
    }
};


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

    virtual UnitMap readPopulation(int gen) {
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
    virtual void printPopulation ( Units units, int gen ) {
        for(auto& x: units){
            os << x<<std::endl;
        }
    }
};

template<class OrganismType>
struct EvolutionFile: public EvolutionIO{
    std::string folderName;
    int id;
    EvolutionFile(std::string folderName):folderName(folderName)
    {
        id = rand()%10000;
        std::string cmd = "mkdir -p ";
        cmd+=folderName+ "_";
        cmd+=std::to_string(id);

        system(cmd.c_str());
    }
    ~EvolutionFile(){
    }
    std::string getName(int gen){
      return folderName +"_"+std::to_string(id) +"/Gen_"+std::to_string(gen);
    }

    virtual UnitMap readPopulation(int gen) {
        using namespace std;
        UnitMap units;
        
       ifstream is(getName(gen), ios::binary );

        int i =0;
        try{
            while(is.good() && !is.eof() && is.peek() != EOF){
                
                OrganismType* basic = new OrganismType();
                is>> *basic;
                Organismptr pointer = Organismptr(basic);
                units[pointer->id] = pointer;
           
            }
        }catch(...){
            
        }
        is.close();

        return units;
    }
    virtual void printPopulation ( Units units, int gen ) {
        using namespace std;
        string name = getName(gen);
        ofstream os(name, ios::trunc | ios::binary );
        if(os.is_open()){
            for(auto& x: units){
                auto nn  = std::dynamic_pointer_cast<OrganismType>(x);
                os << *nn.get();
            }
            os.close();
        }else{
            throw runtime_error( string("Unable to create file:")+name);
        }
    }
};


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
