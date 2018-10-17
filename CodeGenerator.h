#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "Gene.h"
#include <vector>
class CodeGenerator
{
    public:
        CodeGenerator();

        virtual ~CodeGenerator();
        static double randomNormal(double medium, double variance, int distribution);
        static std::vector<int> generate(int size, int start, int end);

    protected:
    private:
};

#endif // CODEGENERATOR_H
