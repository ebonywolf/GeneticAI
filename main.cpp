#include <iostream>
#include "CodeGenerator.h"
#include <time.h>
#include <stdlib.h>
#include "Gene.h"
#include <vector>
#include <typeinfo>
#include <tuple>
#include "GeneChain.h"
#include "Runner.h"
#include <fstream>
#include "Permutations.h"

#include <bits/stdc++.h>
#include "gnuplot-iostream.h"
#include <boost/tuple/tuple.hpp>
using namespace std;
using namespace wag;

using Pontos =   std::vector<std::pair<double, double> >;

class Gnuplot_os: public ostream
{
public:
    double minX=-2,minY=-2,maxY=2,maxX=2;
    double var = 1.2;
    Gnuplot gp;
    Pontos lowest,highest,mediam;
    
    Gnuplot_os(){
         gp << "set xrange [-2:2]\nset yrange [-2:2]\n";
    }
    void updateRange(){
        gp<<"set xrange ["<<minX*var<<":"<<maxX*var<<"]\nset yrange ["<<minY*var<<":"<<maxY*var<<"]\n";
    }
    
    void addPoints(double x, double y, Pontos& p){
        if( x<minX ){
            minX = x;        
        }
        if( x>maxX ){
            maxX = x;        
        }
        if( y<minY ){
            minY = y;        
        }
        if( y>maxY ){
            maxY = y;        
        }
        auto point = make_pair(x,y);
        p.push_back(point);
        
       
    }
    void plot(){
        gp<<"clear\n";
        updateRange();
        gp<<"plot '-' with lines title 'lowest'\n";
        gp<<"plot '-' with lines title 'highest'\n";
        gp<<"plot '-' with lines title 'mediam'\n";
        gp.send1d(lowest);
        gp.send1d(highest);
        gp.send1d(mediam);
    }
    
    template <class T>
    ostream& operator<<( T p) {
        cout<<p;
        return *this;
    }
     
    ostream& operator<<( int p) {
        cout<<p;
        return *this;
    }
    
    ostream& operator<<( Progress& p){
        cout<<"here"<<endl;
        addPoints( p.generations, p.fitnessMedian,mediam  );
        addPoints( p.generations, p.highestFitness,highest  );
        addPoints( p.generations, p.lowestFitness,lowest  );
        plot();
        return *this;
    }
    
    void displayHeader() {
         if(false == displayedHeader) {
            *this << "HEADER2" << endl;
            displayedHeader = true;
         }
      }
   private:
      bool displayedHeader { false };

};


int main()
{
    Gnuplot_os gnuplot_os; 
 
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
    Permutations perm(0.05,30 );
    getPermutation(&perm);
    
    if(file.is_open()){

        DefaultEnv<Organism> env(dnaConfig, fitness,file);

        RunnerConfig config;
        config.fitnessTarget=490;
        config.generations=300;
        config.population=40;
        Runner runner(config,env);
        Progress p= runner.startSimulation();

        file.close();

        ifstream inp("output.dat",ios::binary);
        if(inp.is_open()){
           UnitMap mapa= env.readUnits(inp);
           Organismptr o =mapa[p.highestId];
           cout<<"Organism:"<<o->id<<" "<<o->fitness<<endl;
           cout<<o->dna<<endl;
           inp.close();

        }else{
            cout<<"Foo"<<endl;
        }


    }else{
        std::cout<< "fooo" <<std::endl;
    }
    while(true){
        
    }

    return 0;

}
