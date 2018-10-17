#ifndef GENE_H
#define GENE_H
#include <iostream>


namespace wag{
    class GeneChain;
class Gene
{
    public:
        Gene();
        Gene(int value);
        virtual ~Gene();
        friend std::string& operator+= (std::string&,Gene);
        friend std::string& operator+ (std::string&,Gene);
        friend std::ostream& operator<<  (std::ostream&,Gene);
        int getValue() const {return value;}
        int value;



    protected:




    private:
};
}
#endif // GENE_H
