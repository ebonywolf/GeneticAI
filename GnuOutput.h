#ifndef GNU_OUTPUT
#define GNU_OUTPUT

#include <gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <sys/time.h>
#include <unordered_map>
namespace wag{

using Pontos = std::vector<std::pair<double, double>>;
//using Pontos2 = std::vector<std::pair<double, double,double>>;


class Plotter
{
    double minX=-1,minY=-1,maxY=1,maxX=1;
    double var = 1.1;
    bool upRange= false;
    long long lastT=0; 
    Gnuplot gp;

public:
    enum Styles{
    	LINE, POINT
    };

    std::unordered_map<std::string, Pontos> pontos;
    std::unordered_map<std::string, Styles> style;


    Plotter()
    #ifdef WINDOWS
     :gp("\"C:\\\"Program Files\"\\gnuplot\\bin\\gnuplot.exe\"") 
    #endif
    {
         gp << "set xrange [-2:2]\nset yrange [-2:5]\n";
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
private:
    void updateRange(){
           if(upRange){
                gp<<"set xrange ["<<minX*var<<":"<<maxX*var<<"]\nset yrange ["<<minY*var<<":"<<maxY*var<<"]\n";
                upRange = false;
           }
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
    

    void sort(Pontos& p){
    	auto func = [](std::pair<double, double>& a,std::pair<double, double>& b){
    		return a.first < b.first;
    	};

    	std::sort(p.begin(), p.end(), func);
    }

    std::vector<std::vector<double>> toGradient(Pontos points){
    	 std::vector<std::vector<double>> novo;
    	 double tacc = 10.0 /(double) points.size();
    	 double sum = 1;
    	 for (auto& p: points) {
    		 std::vector<double> p3(3);
    		 p3[0] = p.first;
    		 p3[1] = p.second;
    		 p3[2] = sum;
			sum += tacc;
			novo.push_back(p3);
		}
    	return novo;
    }
public:
    void addPontos(std::string name, Pontos p){
        pontos[name]=squish(p);
    }



    std::string toStyle(Styles s){
    	switch (s) {
			case LINE:return "lines";

			default:
				return "p";
				break;
		}
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
        	if( style[pt.first]==LINE )
        		sort(pt.second );
            for (auto& x : pt.second  ){
                checkPoints( x.first, x.second );
            }
        }

        gp<<"set palette model RGB defined (0 \"white\",10 \"blue\")\n";
        gp<<"set errorbars front 1.000000  linecolor -1 linewidth 1.000 dashtype '-' ";

       
        updateRange();
        int i=0;


        for (auto& pt : pontos  ){
           if(i==0)gp<<"plot ";
           if(i>0)gp<<",";
           //toStyle(style[pt.first])
           gp<<"'-' with "<< "p" <<" title '"<<pt.first<<"()'";
           i++;
        }
        gp<<"\n";
        for (auto& x :pontos  ){
              gp.send1d(x.second);
        	//auto alce = toGradient(x.second);
          //  gp.send2d(alce);
        }

      
    }

};


}
#endif // GNU_OUTPUT
