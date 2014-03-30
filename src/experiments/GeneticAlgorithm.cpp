#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm()
{
}

void GeneticAlgorithm::optimize(MatrixDouble constraints, FUNCTION func, unsigned int numberPoints) {

    constraints_ = constraints;
    int N = constraints_.rows();
    number_ = N;

    numberPoints_ = numberPoints;

    GA_struct ga;
    ga.parameters = VectorDouble(N);

    for(int i=0; i<N; ++i){
        ga.parameters[i] = min(i);
    }

    ga.fitness = func(ga.parameters);


    for(unsigned int i=0; i<numberPoints; ++i){
        GA_struct mutatedGA = mutation(ga,func);

        ga = selection( ga, mutatedGA );
    }

    qDebug() << "Result";
    print(ga.parameters);
    qDebug() << "Function:" << ga.fitness;


}

void GeneticAlgorithm::print(const MatrixDouble &matrix)
{
    qDebug() << QString("Matrix %1x%2").arg(matrix.rows()).arg(matrix.columns());
    for(int i=0; i < matrix.rows(); ++i){
        QStringList line;
        for(int j=0; j < matrix.columns(); ++j){
//            qDebug() << QString("m(%1,%2)=%3").arg(i).arg(j).arg(matrix(i,j));
              line.append( QString::number(matrix(i,j)) );
        }

        qDebug() << line.join("\t");
    }

    qDebug() << "\n";
}

void GeneticAlgorithm::print(const VectorDouble &vector)
{
    for(int i=0; i < vector.rows(); ++i){
        qDebug() << QString("v[%1]=%2").arg(i).arg(vector[i]);
    }
    qDebug() << "\n";
}

GA_struct GeneticAlgorithm::selection(const GA_struct &population1, const GA_struct &population2) {
    if(population1.fitness > population2.fitness){
        return population2;
    }
    return population1;
}

GA_struct GeneticAlgorithm::mutation(const GA_struct &population, FUNCTION func) {

    GA_struct newPopulation;
    int maxInt = std::numeric_limits<int>::max();
    do{
        newPopulation.parameters = VectorDouble(number_);

        for(int i=0; i < number_; ++i){
            double step = static_cast<double>((max(i) - min(i))/ maxInt);
            int randStep = rand();
            double value = min(i) + step*randStep;

            newPopulation.parameters[i] = value;
        }
    }while( population == newPopulation );


    newPopulation.fitness = func(newPopulation.parameters);

    return newPopulation;
}

double GeneticAlgorithm::min(int index) {
    return constraints_.at(index,0);
}

double GeneticAlgorithm::max(int index) {
    return constraints_.at(index,1);

}
