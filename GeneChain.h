#ifndef GENECHAIN_H
#define GENECHAIN_H
#include <iostream>
#include <vector>
namespace wag{
class GeneChain : public std::vector<long>
{
	public:
		/** Default constructor */
         using std::vector<long>::vector;
		GeneChain(){}
		GeneChain ( std::vector<long> );

		/** Default destructor */
		virtual ~GeneChain();
		GeneChain crossOver(const GeneChain& gene)const ;
		void mutate();

        friend std::ostream& operator<< ( std::ostream&, const GeneChain& );
       
        void operator+=(const GeneChain& d){
            for (int i =0; i < this->size() && i < d.size() ; i++){
                (*this)[i]+=d[i];
            }
        }
        
        double diff(const GeneChain& d)const {
            double diff =0;
            for (int i =0; i < this->size() && i < d.size() ; i++){
                double val =  (*this)[i] - d[i];
                if(val<0) val*=-1;
                diff+=val;
            }
            return diff;
        }
	
};
}

#endif // GENECHAIN_H
