
#include "CodeGenerator.h"
#include "Gene.h"
#include "GeneChain.h"
#include "Runner.h"
#include "Permutations.h"
#include "Output.h"

#include <bits/stdc++.h>
#include "gnuplot-iostream.h"
#include <boost/tuple/tuple.hpp>
using namespace std;
using namespace wag;

using Pontos =   std::vector<std::pair<double, double> >;

class Gnuplot_os: public ProgressIO
{
public:
    double minX=-2,minY=200,maxY=2,maxX=2;
    double var = 1.1;
    bool upRange= false;

    Gnuplot gp;
    Pontos lowest,highest,mediam;
    
    Gnuplot_os(){
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
        auto point = make_pair(x,y);
        p.push_back(point);
        
    }
    void plot(){
     
       // gp<<"clear\n";
        updateRange();
        gp<<"plot '-' with lines title 'lowest',";
        gp<<" '-' with lines title 'highest',";
        gp<<" '-' with lines title 'mediam'\n";
        gp.send1d(lowest);
        gp.send1d(highest);
        gp.send1d(mediam);
    }
    
    virtual void printProgress ( Progress p) {
        addPoints( p.generations, p.fitnessMedian,mediam  );
        addPoints( p.generations, p.highestFitness,highest  );
        addPoints( p.generations, p.lowestFitness,lowest  );
        
    }
    virtual void saveProgress ( Progress ) {}
    virtual Progress loadProgress() {}
    
};



int main()
{

    ProgressIOptr  gnuplot_os = make_shared<Gnuplot_os>(); 
    
    auto func = [gnuplot_os](){
        while(true){
            usleep(2*1000); 
            auto f =(Gnuplot_os*)gnuplot_os.get();
            
            f->plot();
        }
    };
    thread t(func);
    
 
    srand(time(0));

    vector<int> result = {5,5,5,5,5,5,5,5,5,5};


    /*
    {
    Dna dna= {result};
    ofstream file("output.dat",ios::trunc | ios::binary);
    Organismptr o = make_shared<Organism>();
    o->id = 5;
    o->fitness = 100;
    o->dna = dna;
    file<<*o;
    file.close();
      ifstream file2("output.dat", ios::binary);
      file2>>*o;

 
    }

    return 0;
    */
    auto fitness=[result](Organismptr o){
        int a = 500;
        int siz = o->dna[0].size();
        if(siz!= 10)throw runtime_error("shit happened");
        for(int i =0; i< 10; i++){
            int dif = (abs(result[i] - o->dna[0][i] ))*2;
            a-=dif;
        }
        o->fitness = a;
    };


    DnaConfig dnaConfig={make_tuple(10,0,30)};
    ofstream file("output.dat",ios::trunc | ios::binary);
    
    Permutations perm(0.7, 5 );
    getPermutation(&perm);
    
    if(file.is_open()){

        DefaultEnv<Organism> env(dnaConfig, fitness,make_shared<EvolutionStd<Organism>>(file));

        RunnerConfig config;
        config.fitnessTarget=495;
        config.generations=1000;
        config.population=4000;
        Runner runner(config,env, gnuplot_os );
        Progress p= runner.startSimulation();

        file.close();


    }else{
        std::cout<< "fooo" <<std::endl;
    }
   
    t.join();

    return 0;

}
