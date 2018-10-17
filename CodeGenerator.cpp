#include "CodeGenerator.h"
#include <stdlib.h>
#include "Gene.h"
using namespace std;
using namespace wag;
CodeGenerator::CodeGenerator()
{
    //ctor
}

CodeGenerator::~CodeGenerator()
{
    //dtor
}

vector<int> CodeGenerator::generate(int size, int start, int end){
    vector<int> vetor;
    vetor.resize(size);
    int range = end-start;
    for (int i=0;i<size;i++){
        vetor[i]= rand()%range + start;
    }
    return vetor;
}

double CodeGenerator::randomNormal(double medium, double variance, int distribution){
    double sum=0;

    if(distribution<=0)distribution=1;
    if(variance<0)variance*=-1;

    for (int i=0;i<distribution;i++){
        double var=(double)rand()/RAND_MAX; //v
         var=var*variance*2-variance;
        var = var+medium;
        sum+=var;
    }
    return (double)sum/distribution;

 }
