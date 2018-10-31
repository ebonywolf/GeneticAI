#ifndef PERMUTATIONS_H
#define PERMUTATIONS_H
#include "GeneChain.h"
#include "Organism.h"
#include <vector>
#include <time.h>

#include <stdlib.h>
#include <random>
#include <memory>
#include <functional>
namespace wag
{

class Permutations
{
	public:

		Permutations(double elitism = 0.05, double mutatePercent = 0.1, int mutateVar=100):
		     elitism(elitism),mutatePercent(mutatePercent), mutateVar(mutateVar){}
		     
        static std::default_random_engine generator;
        static std::uniform_real_distribution<double> unif;//(0, 1);
        
        double elitism;
        double mutatePercent;
        int mutateVar;  
		virtual ~Permutations(){}
		virtual void mutate ( GeneChain& );
		virtual GeneChain divide ( GeneChain& ) ;
		virtual std::vector<std::shared_ptr<wag::Organism>> crossOrganism( std::vector<std::shared_ptr<wag::Organism>>& organisms );
		virtual GeneChain cross ( const GeneChain&,const  GeneChain& ) ;
	protected:
	private:
};


Permutations* getPermutation ( Permutations* p = 0);
}
#endif // PERMUTATIONS_H
