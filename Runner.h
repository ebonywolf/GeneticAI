#ifndef RUNNER_H
#define RUNNER_H

#include <bits/stdc++.h>

#include "Organism.h"
#include "Permutations.h"
#include "GeneChain.h"
#include "Dna.h"
#include "CodeGenerator.h"
#include "Output.h"
namespace wag{
    class Runner;

struct Environment{
    
    virtual Units createUnits(int number){
    }
    virtual std::shared_ptr<EvolutionIO> getIO(){}

    virtual void runUnits( Units& units){
    }
   

private:
    friend Runner;
};
using Envptr = std::shared_ptr<Environment>;
using DnaConfig =  std::vector<std::tuple<int,int,int>>;

template<class T>
struct DefaultEnv: public Environment{

    std::vector<std::tuple<int,int,int>> dnaConfig;//int ammount, int min, int max
    std::function<void(Organismptr)> fitnessFunction;
    EvolutionIOptr io;
      
    
    DefaultEnv<T>(std::vector<std::tuple<int,int,int>> dnaConfig,  std::function<void(Organismptr)>
                  fitnessFunction, EvolutionIOptr io = std::make_shared<EvolutionStd<T>>()   ):
                      dnaConfig(dnaConfig), fitnessFunction(fitnessFunction), io(io)
    {
       
                          
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
    std::shared_ptr<EvolutionIO> getIO(){
        return io;
    }

    void runUnits( Units& units){
        for(auto& x: units){
            fitnessFunction(x);
        }
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
        Runner():env( *(new Environment())),progressOut( std::make_shared<ProgressStd>()  )
        {}
        Runner(RunnerConfig config, Environment& env, 
               ProgressIOptr progressOutput= std::make_shared<ProgressStd>() ):
           env(env), config(config), progressOut(progressOutput) {
        }
        void resume(){
        }

        Progress startSimulation(){
            units = env.createUnits( config.population );
            Permutations* perm = getPermutation();
            Progress best = progress;
            while( progress.generations < config.generations ){
                env.runUnits(units);
                auto io = env.getIO();
                io->printPopulation(units,  progress.generations);
       
                double sum=0;
                double highest=-10e10;
                double lowest=10e10;
                
                int id=0;
                    
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
                progress.lowestFitness = lowest;

                progressOut->printProgress(progress);
               
                progress.generations++;
                
                Units novo =perm->crossOrganism(units);
                units.swap(novo);
                if(progress.highestFitness > best.highestFitness){
                    best = progress;
                }
                
                if(progress.highestFitness >= config.fitnessTarget)break;
            }
         //   progressOut->saveProgress(progress);
             return best;

        }
        virtual ~Runner(){};

    protected:

        ProgressIOptr progressOut;
        RunnerConfig config;
        Environment& env;
        
        Units units;
        Progress progress;

    private:
};

}
#endif // RUNNER_H
