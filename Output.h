#ifndef OUTPUT
#define OUTPUT
#include "Organism.h"
#include <stdio.h>
#include <cstdlib>

#include <gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <sys/time.h>

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
        Organism basic;
       ifstream is(getName(gen), ios::binary );

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
        is.close();

        return units;
    }
    virtual void printPopulation ( Units units, int gen ) {
        using namespace std;
        string name = getName(gen);
        ofstream os(name, ios::trunc | ios::binary );
        if(os.is_open()){
            for(auto& x: units){
                os << x;
            }
            os.close();
        }else{
            throw runtime_error( string("Unable to create file:")+name);
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

class Gnuplot_os: public ProgressIO
{
public:
    double minX=-2,minY=200,maxY=2,maxX=2;
    double var = 1.1;
    bool upRange= false;

   Gnuplot gp;

    Pontos lowest,highest,mediam;

    Gnuplot_os():gp("\"C:\\\"Program Files\"\\gnuplot\\bin\\gnuplot.exe\"")
        {
        std::cout<<"Starting"<<std::endl;
         gp << "set xrange [-2:2]\nset yrange [-2:5]\n";
    }
    void updateRange(){
        if(upRange){
             gp<<"set xrange ["<<minX*var<<":"<<maxX*var<<"]\nset yrange ["<<minY*var<<":"<<maxY*var<<"]\n";
             upRange = false;
        }

    }
    void addPoints(double x, double y, Pontos& p){
        if( x<minX ){
            minX = x; upRange= true;
        }
        if( x>maxX ){
            maxX = x;  upRange= true;
        }
        if( y<minY ){
            minY = y; upRange= true;
        }
        if( y>maxY ){
            maxY = y;  upRange= true;
        }
        auto point = std::make_pair(x,y);
        p.push_back(point);

    }
    void plot(){
       struct timeval te;
        gettimeofday(&te, NULL); // get current time
       long long tnow = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
       static auto lastT = tnow;
       if(tnow - lastT >20){
         lastT = tnow;
       }else{
        return;
       }

        updateRange();
        gp<<"plot '-' with lines title 'lowest',";
        gp<<" '-' with lines title 'highest',";
        gp<<" '-' with lines title 'mediam'\n";
        gp.send1d( (lowest));
        gp.send1d((highest));
        gp.send1d((mediam));
    }

    virtual void printProgress ( Progress p) {
        addPoints( p.generations, p.fitnessMedian,mediam  );
        addPoints( p.generations, p.highestFitness,highest  );
        addPoints( p.generations, p.lowestFitness,lowest  );

    }
    virtual void saveProgress ( Progress ) {}
    virtual Progress loadProgress() {}

};


}



#endif // OUTPUT
