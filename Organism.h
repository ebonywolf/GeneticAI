#ifndef ORGANISM_H
#define ORGANISM_H
#include <vector>
#include "GeneChain.h"
#include <memory>
#include "Dna.h"
#include <tuple>
#include <iostream>

namespace wag  {
class Organism;
 using Organismptr = std::shared_ptr<Organism>;
    struct Organism: public std::enable_shared_from_this<Organism>
    {
        public:
            Organism();
            Organism(const Organism& o):Organism(){
                fitness = o.fitness;
                dna = o.dna;
            }
          
            double fitness ;
            long id;
            wag::Dna dna;
            
            virtual Organismptr clone() const;
            virtual Organismptr cross( Organismptr )const;

            friend std::ostream& operator<<(std::ostream& out, const Organism& o){
                out.write((char *) &o.id, sizeof(long));
                out.write((char *) &o.fitness, sizeof(double));
                int m = o.dna.size();
                out.write((char *) &m, sizeof(int));
                for(int i=0; i< m; i++){
                    int n = o.dna[i].size();
                    out.write((char *) &n, sizeof(int));
                    for(int j=0; j< n; j++){
                        long val = o.dna[i][j];
                        out.write((char *) &val, sizeof(long));
                    }
                }

                return out;
            }
             friend std::istream& operator>>(std::istream& in, Organism& o){
                //id //fitness //M size of dna // n size of chain //for every chain
                in.read((char *) &o.id, sizeof(long));
                in.read((char *) &o.fitness, sizeof(double));
                int m ;
                in.read((char *) &m, sizeof(int));
                o.dna.resize(m);
                for(int i=0; i< m; i++){
                    int n ;
                    in.read((char *) &n, sizeof(int));
                    o.dna[i].resize(n);

                    for(int j=0; j< n; j++){
                        long val = 0;;
                        in.read((char *) &val, sizeof(long));
                         o.dna[i][j] = val;
                    }
                }


                return in;
            }
            virtual ~Organism();
        protected:
        private:
             static long cont;
    };
}
#endif // ORGANISM_H
