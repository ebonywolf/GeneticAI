#ifndef RUNNER_H
#define RUNNER_H
#include "Organism.h"
#include <iostream>
#include <memory>
#include <functional>
#include "Permutations.h"
#include "GeneChain.h"
#include "Dna.h"
#include <unordered_map>
#include <map>
#include "CodeGenerator.h"

namespace wag{
    class Runner;

struct Environment{
    virtual Units createUnits(int number){
    }

    virtual void runUnits( Units& units){
    }
     virtual UnitMap readUnits(std::istream& is){

    }

private:
    friend Runner;
};

using DnaConfig =  std::vector<std::tuple<int,int,int>>;

template<class T>
struct DefaultEnv: public Environment{

    std::vector<std::tuple<int,int,int>> dnaConfig;//int ammount, int min, int max

    std::function<void(Organismptr)> fitnessFunction;
    std::ostream& output;
    DefaultEnv<T>(std::vector<std::tuple<int,int,int>> dnaConfig,  std::function<void(Organismptr)>
                  fitnessFunction, std::ostream& output=std::cout  ):
                      dnaConfig(dnaConfig), fitnessFunction(fitnessFunction), output(output){
    }
    Units createUnits(int number){
        Units units;
        for(int i=0; i < number; i++){
            Organismptr u = std::make_shared<T>();
            Dna novo;
            for(int j=0; j< dnaConfig.size(); j++){
                GeneChain novoChain ( CodeGenerator::generate(
                std::get<0>(dnaConfig[j]),std::get<1>(dnaConfig[j]),std::get<2>(dnaConfig[j])) );
                novo.push_back(novoChain);
            }
            u->dna = novo;
            units.push_back(u);
        }
        return units;
    }
    void runUnits( Units& units){
        for(auto& x: units){
            fitnessFunction(x);
            output<<*x;
        }
    }
    UnitMap readUnits(std::istream& is){
        UnitMap units;
        Organism basic;
        int i =0;
        try{
            while(is.good() && !is.eof() && is.peek() != EOF){
              //  std::cout << "Reading line"<<++i<< std::endl;
                Organismptr u = std::make_shared<T>();

                is>> basic;
                auto alce =u.get();
                *alce = basic;
                units[u->id] = u;
            }
        }catch(...){
        }

        return units;
    }
};


struct RunnerConfig{
    int generations;
    int fitnessTarget;
    int population;
};

class Runner
{
    public:
        Runner():env(  *(new Environment())  ){}
        Runner(RunnerConfig config, Environment& env, std::ostream& progressOutput = std::cout ):
           env(env), config(config), progressOut(progressOutput) {
        }

        Progress startSimulation(){
            units = env.createUnits( config.population );
            Permutations* perm = getPermutation();
            Progress best = progress;
            while( progress.generations < config.generations ){
                env.runUnits(units);

                progress.generations++;
                double sum=0;
                double highest=-10e10;
                double lowest=10e10;
                
                int id;
                for(auto& unit: units){
                    sum+=unit->fitness;
                    if(unit->fitness > highest){
                        highest = unit->fitness;
                        id = unit-> id;
                    }
                    if( unit->fitness < lowest ){
                        lowest = unit->fitness;
                    }
                    
                }
                progress.fitnessMedian = sum/(double)units.size();
                progress.highestFitness = highest;
                progress.highestId = id;

                progressOut<<progress<<std::endl;

                Units novo =perm->crossOrganism(units);
                units.swap(novo);
                if(progress.highestFitness > best.highestFitness){
                    best = progress;
                }
                
                if(progress.highestFitness >= config.fitnessTarget)break;
            }
            progressOut<<"FINISHED: Best Gen:"<<best<<std::endl;
            return best;

        }
        virtual ~Runner(){};

    protected:

        std::ostream& progressOut = std::cout;
        RunnerConfig config;
        Environment& env;
        Units units;
        Progress progress;

    private:
};

}
#endif // RUNNER_H
