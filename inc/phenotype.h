#pragma once

class BasePhenotype
{
    //make virtual functions
    BasePhenotype();
    ~BasePhenotype();
    Phenotype();
    virtual ~Phenotype();
    virtual void mutate();
    virtual void crossover(BasePhenotype);
    void evaluate();
    void print();
    void setFitness();
    void getFitness();
    void setGenotype();
    void getGenotype();
    void setPhenotype();
    void getPhenotype();
    void setGenome();
    void getGenome();
    void setGenomeSize();
    void getGenomeSize();
    void setGenomeType();
    void getGenomeType();
};

class Phenotype : public BasePhenotype
{
public:
    Phenotype();
    ~Phenotype();
    void mutate();
    void crossover();
    void evaluate();
    void print();
    void setFitness();
    void getFitness();
    void setGenotype();
    void getGenotype();
    void setPhenotype();
    void getPhenotype();
    void setGenome();
    void getGenome();
    void setGenomeSize();
    void getGenomeSize();
    void setGenomeType();
    void getGenomeType();
};