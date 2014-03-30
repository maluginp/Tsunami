#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H
#include "defines.h"

using namespace tsunami;


struct GA_struct{
    VectorDouble parameters;
    double fitness;

    bool operator ==(const GA_struct& other) const{
        if(parameters.rows() == other.parameters.rows()){
            int N = parameters.rows();
            for(int i=0;i<N; ++i){
                if(parameters[i] != other.parameters[i]){
                    return false;
                }
            }
            return true;
        }

        return false;
    }
};

class GeneticAlgorithm{
public:
    GeneticAlgorithm();

    void optimize(MatrixDouble constraints, FUNCTION func, unsigned int numberPoints=50000000);
private:
    void print(const MatrixDouble& matrix);
    void print(const VectorDouble& vector);

    GA_struct selection( const GA_struct& population1,
                         const GA_struct& population2);

    GA_struct mutation(const GA_struct& population, FUNCTION func);

    double min(int index);
    double max(int index);
    unsigned int numberPoints_;
    int number_;
    MatrixDouble constraints_;
};

#endif // GENETICALGORITHM_H
