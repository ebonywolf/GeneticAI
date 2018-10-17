#include "Permutations.h"
#include <stdlib.h>
#include <vector>
#include <tuple>
using namespace std;

namespace wag
{
     std::default_random_engine Permutations::generator = std::default_random_engine(time(0));
     std::uniform_real_distribution<double> Permutations::unif = std::uniform_real_distribution<double>(0, 1);
Permutations* getPermutation ( Permutations* p)
{
	static Permutations* singleton=0;

	if ( p != 0 ) {
		if ( singleton != 0 ) {
			delete ( singleton );
		}
		singleton = p;
	} else {
		// p==0
	}
	return singleton;
}

void Permutations::mutate ( GeneChain& g )
{
	auto percent  = mutatePercent;
	auto var = mutateVar;

	for ( auto&x : g ) {
        double d = unif(generator);
        double v = unif(generator);
        v = var*v - var/2.0; 
		if ( d < percent ) { //percent of mutation happening
			
            x += v;
		}
	}

}

std::vector<std::shared_ptr<Organism>> Permutations::crossOrganism( std::vector<std::shared_ptr<Organism>>& units ){
    double total=0;
    std::vector<std::shared_ptr<Organism>> novounits;

    for(auto& x: units){
        total += x->fitness;
    }
    vector< tuple<shared_ptr<Organism>, double > > intervals;
    double inte=0;
    for(auto& x: units){
        double var = x->fitness / total;
        inte+=var;
        intervals.push_back(make_tuple(x,inte));
    }
    static auto findDaddy = [ ](decltype(intervals)& intervals, Organismptr op=0){
        double r = unif(generator);
        for(auto& x: intervals){
            if( get<1>(x) > r  ){
                if(op.get()!=0 && op->id == get<0>(x)->id){
                    continue;                
                }
                return get<0>(x);
            }
        }
        return get<0>( intervals[0]);
        throw(runtime_error("Unable to find dad in intervals"));

    };

   // cerr<<"Interval Total:"<<inte<<endl;
    int totalUnits = units.size();
    
    
    while(totalUnits>0){
        auto daddy = findDaddy(intervals);
        decltype(daddy) mommy;
        
        mommy = findDaddy(intervals,daddy);
        
        auto son = daddy->cross(mommy);
        /*
        cout<<"Daddy:"<<daddy->id<<" "<<daddy->fitness<<daddy->dna<<" ";
        std::cout<< "Mommy:"<<mommy->id<<" "<<mommy->fitness<<mommy->dna<<std::endl;
          std::cout<< "Son:"<<son->id<<" "<<son->dna <<std::endl;
      */
      
        novounits.push_back(son);
        totalUnits--;
    }
    return novounits;
}
GeneChain Permutations::divide ( GeneChain& a )
{
	GeneChain division;
	for ( auto x : a ) {
		if ( rand() % 100 < 50 ) {
			division.push_back ( x );
		}
	}
	return division;
}





GeneChain Permutations::cross ( const GeneChain& a ,const GeneChain& b )
{
    
    
	GeneChain genes;
	int contA = a.size();
	int contB = b.size();
    auto geneA = a.begin();
    auto geneB = b.begin();

	if(contA != contB){
        throw runtime_error("Genechain not the same size");
	}
	int i=0;
	while( i++ < contA){
        if(rand()%100 <50 ){
            genes.push_back( *geneA );
        }else{
            genes.push_back( *geneB );
        }
        geneA++;
        geneB++;
	}
    mutate(genes);

	return genes;
	
}
}
