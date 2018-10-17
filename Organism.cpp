#include "Organism.h"
#include <iostream>

using namespace std;
using namespace wag;

    using Organismptr = std::shared_ptr<Organism>;
Organism::Organism()
{

    static long cont=0;
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
    int alce = novo->id;
    *novo = *this;
    novo->id = alce;
    return shared_ptr<Organism>(novo);

}
