
#include "CodeGenerator.h"

#include "GeneChain.h"
#include "Runner.h"
#include "Permutations.h"
#include "Output.h"

#include <bits/stdc++.h>
using namespace std;
using namespace wag;


int main()
{
    ProgressIOptr  gnuplot_os = make_shared<Gnuplot_os>();

    auto func = [gnuplot_os](){
        while(true){
           // usleep(2*1000);
            Sleep(20);
            auto f =(Gnuplot_os*)gnuplot_os.get();

            f->plot();
        }
    };
    thread t(func);
    t.detach();


    srand(time(0));

    vector<int> result = {5,5,5,5,5,5,5,5,5,5};

    auto fitness=[result](Organismptr o){
        int a = 500;
        int siz = o->dna[0].size();
        if(siz!= 10)throw runtime_error("shit happened");
        for(int i =0; i< 10; i++){
            int dif = (abs(result[i] - o->dna[0][i] ))*2;
            a-=dif;
        }
        o->fitness = a;
    };


    DnaConfig dnaConfig={make_tuple(10,0,30)};

    Permutations perm(0.05 ,0.02, 7 );
    getPermutation(&perm);



    DefaultEnv<Organism> env(dnaConfig, fitness,make_shared<EvolutionFile<Organism>>("Test"));

    RunnerConfig config;
    config.fitnessTarget=499;
    config.generations=10000;
    config.population=200;
    Runner runner(config,env, gnuplot_os );
    Progress p= runner.startSimulation();


    //t.join();

    return 0;

}

