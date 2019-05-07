#ifndef GNU_OUTPUT
#define GNU_OUTPUT

#include <gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <sys/time.h>
#include <unordered_map>
namespace wag{
using Pontos =   std::vector<std::pair<double, double> >;

class Plotter
{
public:
    double minX=-1,minY=-1,maxY=1,maxX=1;
    double var = 1.1;
    bool upRange= false;
    long long lastT=0; 
    Gnuplot gp;

    std::unordered_map<std::string, Pontos> pontos;
    
    Plotter()
    #ifdef WINDOWS
     :gp("\"C:\\\"Program Files\"\\gnuplot\\bin\\gnuplot.exe\"") 
    #endif
    {
         gp << "set xrange [-2:2]\nset yrange [-2:5]\n";
    }
    void updateRange(){
        if(upRange){
             gp<<"set xrange ["<<minX*var<<":"<<maxX*var<<"]\nset yrange ["<<minY*var<<":"<<maxY*var<<"]\n";
             upRange = false;
        }
    }
    
    template <class T>
    static Pontos generatePoints( double start, double end, T f, int precision=1000){
      
        
        double step = (end-start)/(double)precision;
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
    void sort(Pontos& p){
    	auto func = [](std::pair<double, double>& a,std::pair<double, double>& b){
    		return a.first < b.first;
    	};

    	std::sort(p.begin(), p.end(), func);
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
        	sort(pt.second );
            for (auto& x : pt.second  ){
                checkPoints( x.first, x.second );
            }
        }
       
        updateRange();
        int i=0;
        for (auto& pt : pontos  ){
           if(i==0)gp<<"plot ";
           if(i>0)gp<<",";
           gp<<"'-' with lines title '"<<pt.first<<"()'";
           i++;
        }
        gp<<"\n";
        for (auto& x :pontos  ){
              gp.send1d(x.second);
        }
      
    }

};


}
#endif // GNU_OUTPUT
