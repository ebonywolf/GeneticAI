#include "GeneChain.h"
#include <stdlib.h>
#include "CodeGenerator.h"
#include "Permutations.h"
using namespace wag;
using namespace std;

GeneChain::GeneChain(std::vector<int> v):vector<int>(v){


}

GeneChain GeneChain::crossOver(const GeneChain& gene) const{
    auto perm = getPermutation();
    return perm->cross(*this, gene);

}
void GeneChain::mutate(){
    auto perm = getPermutation();
     perm->mutate(*this);

}
GeneChain::~GeneChain()
{
	//dtor
}

namespace wag{
    std::ostream& operator<< ( std::ostream& c, const GeneChain& p){
        for(auto x: p){
            c << x << " ";
        }
        return c;

    }
}

