
#include "CodeGenerator.h"

#include "GeneChain.h"
#include "Runner.h"
#include "Permutations.h"

#include <bits/stdc++.h>
using namespace std;
using namespace wag;


#include <gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <sys/time.h>


using Pontos =   std::vector<std::pair<double, double> >;

class Plotter2
{
public:
    double minX=-2,minY=-2,maxY=2,maxX=2;
    double var = 1.1;
    bool upRange= false;
    long long lastT=0; 
    Gnuplot gp;

    std::unordered_map<std::string, Pontos> pontos;
    
    Plotter2()
    #ifdef WINDOWS
     :gp("\"C:\\\"Program Files\"\\gnuplot\\bin\\gnuplot.exe\"") 
    #endif
    {
         gp << "set xrange [0:500]\nset yrange [0:10]\n";
    }
    void updateRange(){
        if(upRange){
             gp<<"set xrange ["<<minX*var<<":"<<maxX*var<<"]\nset yrange ["<<minY*var<<":"<<maxY*var<<"]\n";
             upRange = false;
        }
    }
    
    template <class T>
    static Pontos generatePoints( double start, double end, T f, int precision=1000){
      
        double step = (end-start)/precision;
        Pontos pts;
        
        for (double i =start; i < end  ; i+=step){
            pts.push_back( std::make_pair(i, f(i)) );
        }
        return pts;
    }

    void checkPoints(double& x, double& y){
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
    }
    Pontos squish( Pontos& p){
       if(p.size()>1000){
            double var = p.size()/500.0;
            Pontos novo;
            double cont=0;
            
            double sumY=0;
            double sumX=0;
            
            for (int i =0; i < p.size(); i++){
                sumX+= p[i].first;
                sumY+= p[i].second;
                if(cont > var){
                    novo.push_back( std::make_pair(sumX/cont, sumY/cont) );
                    sumY=0;
                    sumX=0;
                    cont =0;                
                }
                cont++;
            }
            return novo;
            
            
        }else{ 
            return p;
        }
    }
    
    void addPontos(std::string name, Pontos p){
        pontos[name]=squish(p);
    }
    void addPoint(std::string name, double x, double y){
        pontos[name].push_back( std::make_pair(x,y));
        
        if(pontos[name].size()>1000) 
            pontos[name] = squish(pontos[name]);
    }
    void plot(){
        struct timeval te;
        gettimeofday(&te, NULL); // get current time
        long long tnow = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
         minX=-2,minY=-2,maxY=2,maxX=2;
        
        if(tnow - lastT >20){
            lastT = tnow;
        }else{
            return;
        }
        
        for (auto& pt : pontos ){
            for (auto& x : pt.second  ){
                checkPoints( x.first, x.second );
            }
        }
       
        updateRange();
        int i=0;
        for (auto& pt : pontos  ){
           if(i==0)gp<<"plot ";
           if(i>0)gp<<",";
           gp<<"'-' w p ls 8 title '"<<pt.first<<"'";
           i++;
        }
        gp<<"\n";
        for (auto& x :pontos  ){
              gp.send1d(x.second);
        }
      
    }

};

struct A: public Organism{
    
    A(int a){
       dna.push_back(GeneChain());
       dna[0].push_back(a *1000 );
       update();
    }
    
    A(){
   
       update();
    }
    void update(){
         fitness = dna[0][0]/1000.0;
         if(fitness>100)fitness =100;
    }
    Organismptr clone() const override{
        A* novo = new A(*this);
        std::shared_ptr<A> pp(novo);
        return std::static_pointer_cast<Organism>(pp);
    }
};

using Aptr = shared_ptr<A>;

Plotter2 plot;
    
template <class T>
void plotshit(T t, string name, double y){
    for (auto& x : t ){
        auto a = (A*)x.get();
        a->update();
        plot.addPoint(name,  a->fitness, y);  
    }
}


void createPlot(){
    Permutations perm( 0,  0.1, 10000);
    getPermutation(&perm);
  
    std::vector<std::shared_ptr<wag::Organism>> pop;
    
    for (int i =0; i < 100 ; i++){
        Aptr a = make_shared<A>(i);
       
        pop.push_back(a);
    }
    plotshit(pop, "pop", 0);
    for (int i =1; i < 11 ; i++){
      auto novopop = perm.crossOrganism(pop);
      plotshit(novopop, "pop", i);
      pop.swap(novopop);
      
    }

    
   
    
    plot.plot();
    string s;
    std::cout << "Press w" << std::endl;
    while(s!="w")
        cin>>s;
    

}


int main()
{
    createPlot();
    

    return 0;

}

