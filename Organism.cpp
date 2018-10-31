#include "Organism.h"
#include <iostream>

using namespace std;
using namespace wag;

using Organismptr = std::shared_ptr<Organism>;
long Organism::cont = 0;
Organism::Organism()
{
    id = cont;
    cont++;
}

Organism::~Organism()
{

}

Organismptr Organism::cross( Organismptr o)const{
    Organismptr clo = clone();
    clo->dna = dna.cross(o->dna);
    return clo;
}

Organismptr Organism::clone() const{
    Organism* novo = new Organism();
    *novo = *this;
    novo->id = cont;
    cont++;;
    return shared_ptr<Organism>(novo);

}
