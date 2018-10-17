#ifndef DNA_H
#define DNA_H

#include <vector>
#include "GeneChain.h"
namespace wag{
class Dna: public std::vector<wag::GeneChain>
{
    public:
        using std::vector<wag::GeneChain>::vector;
        /** Default constructor */
        Dna();
       // Dna ( std::vector<GeneChain> );
        Dna cross( Dna& other) const;
        friend std::ostream& operator<<(std::ostream& os, const Dna& d){
            os<<"==========\n";
            for(int i=0; i<d.size(); i++){
                for(int j=0; j<d.at(i).size(); j++ ){
                    os<< d[i][j]<<" ";
                }
                os<<std::endl;
            }
            os<<"==========";
            return os;
        }

        /** Default destructor */
        virtual ~Dna();
    protected:
    private:
};
}
#endif // DNA_H
