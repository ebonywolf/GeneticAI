#include "Gene.h"
#include <stdlib.h>
using namespace wag;

Gene::Gene(int value):
    value(value)
{


}


Gene::Gene():value(rand())
{

}


Gene::~Gene()
{
    //dtor
}

