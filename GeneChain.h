#ifndef GENECHAIN_H
#define GENECHAIN_H
#include <iostream>
#include <vector>
#include "Gene.h"
namespace wag{
class GeneChain : public std::vector<int>
{
	public:
		/** Default constructor */
         using std::vector<int>::vector;
		GeneChain(){}

		GeneChain ( std::vector<int> );

		/** Default destructor */
		virtual ~GeneChain();
		GeneChain crossOver(const GeneChain& gene)const ;
		void mutate();


        friend std::ostream& operator<< ( std::ostream&, const GeneChain& );

	protected:



	private:
};
}

#endif // GENECHAIN_H
