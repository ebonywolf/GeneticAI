#include "Dna.h"
namespace wag{
Dna::Dna()
{
    //ctor
}

Dna::~Dna()
{
    //dtor
}
 Dna Dna::cross( Dna& other) const{
    Dna novoChain;
   // if(size()!= other.size())throw std::runtime_error("Different sized chain crossing");

    for(int i =0; i < size() && i < other.size(); i++){
       auto novo = at(i).crossOver( other[i]);
       novoChain.push_back(novo);
    }
    return novoChain;

 }


}
