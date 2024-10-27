#include <vector>
#include <random>
#include "SudokuLoader.h"
#include "Genotype.h"
#include <unordered_set>
#include <iostream>
#include <cassert>
#include <algorithm> 
#include <random>  
#include <iostream>

SoloNumGenotype::SoloNumGenotype(const Sudoku& sudoku):BaseGenotype(sudoku)
{
    genoType = SoloNum;
    evalSudokuValid = false;
    int count = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] == -1) {
                count++;
            }
        }
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 9);
    sudokunumbers.reserve(count);
    for(int i = 0; i < count; i++)
    {
        sudokunumbers.push_back(dis(gen));
    }
}

SoloNumGenotype::~SoloNumGenotype()
{
    // Destructor implementation
}

SoloNumGenotype::SoloNumGenotype(const Sudoku& sudoku, std::vector<short> numbers):BaseGenotype(sudoku)
{
    genoType = SoloNum;
    sudokunumbers = numbers;
    evalSudokuValid = false;
}

BaseGenotype* SoloNumGenotype::crossover(BaseGenotype &other)
{

    BaseGenotype* result = nullptr;
    switch (other.getGenoType())
    {
    case SoloNum:
        {
            SoloNumGenotype& otherSolo = static_cast<SoloNumGenotype&>(other);
            const std::vector<short>& otherNumbers = otherSolo.getNumbers();
            std::vector<short> newNumbers;
            newNumbers.reserve(sudokunumbers.size());

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, 1); 

        for (size_t i = 0; i < sudokunumbers.size(); ++i)
        {
            if (distrib(gen) == 0)
            {
                newNumbers.push_back(sudokunumbers[i]);
            }
            else
            {
                newNumbers.push_back(otherNumbers[i]);
            }
        }
        result = new SoloNumGenotype(sudoku, newNumbers); 
        }
        break;
    
    default:
        assert("ilegal crossover"&&0);
    }

    return result;
}

void BaseGenotype::print()
{
    std::cout << getPrintStr() << std::endl;
}
std::string BaseGenotype::getPrintStr()
{
    if (!evalSudokuValid) {
        fillEvalSudoku();
    }
    evaluate();
    std::string str;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            str += std::to_string(evalSudoku[i][j]);
            str += " ";
        }
        str += "\n";
    }
    str+= "Eval value: " + std::to_string(evalValue)+"\n";
    std::string rowStr = "Row collisions: ", colStr = "Column collisions: ", boxStr = "Box collisions: ";
    for (int i = 0; i < 9; i++) {
        rowStr += std::to_string(i) + ":" + std::to_string(rowcount[i]) + ", ";
        colStr += std::to_string(i) + ":" + std::to_string(colcount[i]) + ", ";
        boxStr += std::to_string(i) + ":" + std::to_string(boxcount[i]) + ", ";
    }
    str += rowStr + "\n" + colStr + "\n" + boxStr + "\n";
    
    return str;
}
void SoloNumGenotype::mutate(float mutationRate)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    std::uniform_int_distribution<> dis2(0, 8);
    for (size_t i = 0; i < sudokunumbers.size(); i++)
    {
        if (dis(gen) < mutationRate)
        {
            evalSudokuValid = false;
            sudokunumbers[i] = dis2(gen);
        }
    }
}
void SoloNumGenotype::fillEvalSudoku()
{
    int count = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] == -1) {
                evalSudoku[i][j] = sudokunumbers[count++];
            }
            else
            {
                evalSudoku[i][j] = sudoku[i][j];
            }
        }
    }
    evalSudokuValid = true;
}

void BaseGenotype::evaluate() {
    // std::cout<<"BaseGenotype::evaluate()"<<std::endl;
    if (!evalSudokuValid) {
        fillEvalSudoku();
    }
    evalValue = 0;
    for (int i = 0; i < 9; i++) {
        rowcount[i] = 0;
        colcount[i] = 0;
        boxcount[i] = 0;
    }

    // Check for row and column collisions
    for (int i = 0; i < 9; i++) {
        std::array<bool,9> colisionRow = {false, false, false, false, false, false, false, false, false};
        std::array<bool,9> colisionCol = {false, false, false, false, false, false, false, false, false};
        for (int j = 0; j < 9; j++) {
            // Row check
            if (colisionRow[evalSudoku[i][j]-1]) {
                evalValue++;
                rowcount[i]++;
            } else {
                colisionRow[evalSudoku[i][j]-1] = true;
            }

            // Column check
            if (colisionCol[evalSudoku[j][i]-1]) {
                evalValue++;
                colcount[i]++;
            } else {
                colisionCol[evalSudoku[j][i]-1] = true;
            }
        }
    }
    int box = 0;
    // Check for 3x3 subgrid collisions
    for (int row = 0; row < 9; row += 3) {
        for (int col = 0; col < 9; col += 3) {
            std::array<bool,9> colisionBox = {false, false, false, false, false, false, false, false, false};
            box++;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int val = evalSudoku[row + i][col + j];
                    if (colisionBox[val-1]) {
                        // if(box==1)
                        // std::cout<< "row col val" << box <<" "<<row<<i<<" "<<col<<j<<" "<<val<<std::endl;
                        evalValue++;
                        boxcount[box-1]++;
                    } else {
                        colisionBox[val-1] = true;
                    }
                }
            }
        }
    }
}

FullPermutationGenotype::FullPermutationGenotype(const Sudoku& sudoku):BaseGenotype(sudoku)
{
    genoType = FullPermutation;
    evalSudokuValid = false;
    std::vector<short> remainingNumbers={9,9,9,9,9,9,9,9,9};
    std::random_device rd;
    std::mt19937 gen(rd());
    int size = 0;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(sudoku[i][j] != -1)
            {
                evalSudoku[i][j] = sudoku[i][j];
                remainingNumbers[sudoku[i][j]-1]--;
            }
            else
            {
                size++;
            }

        }
    }
    //create sudoku numbersand shuffle them
    sudokunumbers.reserve(size);
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < remainingNumbers[i]; j++)
        {
            sudokunumbers.push_back(i+1);
        }
    }
    std::shuffle(sudokunumbers.begin(),sudokunumbers.end(),gen);
}

FullPermutationGenotype::FullPermutationGenotype(const Sudoku& sudoku, std::vector<short> numbers):BaseGenotype(sudoku)
{
    genoType = FullPermutation;
    sudokunumbers = numbers;
    evalSudokuValid = false;
    validateGenotype(__LINE__);
}

void FullPermutationGenotype::validateGenotype(int line) {
    fillEvalSudoku();
    std::vector<int> count(9, 0);
    for (const auto& row : evalSudoku) {
        for (const auto& num : row) {
            if (num >= 1 && num <= 9) {
                count[num - 1]++;
            }
        }
    }

    for (int i = 0; i < 9; ++i) {
        if (count[i] != 9) {
            print();
            std::cout <<"line "<<line<<" Invalid genotype: incorrect number of " << i + 1 << " in evalSudoku" << std::endl;
            throw std::runtime_error("Invalid genotype: incorrect number of each digit in evalSudoku");
        }
    }
}

//now working impelmentastion of  (PMX)
// FullPermutationGenotype& parent1 = *this;
//             FullPermutationGenotype& parent2 = dynamic_cast<FullPermutationGenotype&>(other);

//             // Ensure both parents have the same size
//             assert(parent1.sudokunumbers.size() == parent2.sudokunumbers.size());

//             size_t size = parent1.sudokunumbers.size();
//             std::vector<short> offspring(size, -1);

//             // Random number generator
//             std::random_device rd;
//             std::mt19937 gen(rd());
//             std::uniform_int_distribution<> dis(0, size - 1);

//             // Select two crossover points
//             int crossoverPoint1 = dis(gen);
//             int crossoverPoint2 = dis(gen);

//             // Ensure crossoverPoint1 < crossoverPoint2
//             if (crossoverPoint1 > crossoverPoint2) {
//                 std::swap(crossoverPoint1, crossoverPoint2);
//             }

//             // Copy the segment between the crossover points from parent1 to offspring
//             for (int i = crossoverPoint1; i <= crossoverPoint2; ++i) {
//                 offspring[i] = parent1.sudokunumbers[i];
//             }

//             // Map the remaining elements from parent2 to offspring
//             for (int i = crossoverPoint1; i <= crossoverPoint2; ++i) {
//                 if (std::find(offspring.begin(), offspring.end(), parent2.sudokunumbers[i]) == offspring.end()) {
//                     int pos = i;
//                     while (crossoverPoint1 <= pos && pos <= crossoverPoint2) {
//                         pos = std::distance(parent2.sudokunumbers.begin(), std::find(parent2.sudokunumbers.begin(), parent2.sudokunumbers.end(), parent1.sudokunumbers[pos]));
//                     }
//                     offspring[pos] = parent2.sudokunumbers[i];
//                 }
//             }

//             // Fill in the remaining positions with elements from parent2
//             for (std::size_t i = 0; i < size; ++i) {
//                 if (offspring[i] == -1) {
//                     offspring[i] = parent2.sudokunumbers[i];
//                 }
//             }

//             // Create the result genotype
//             auto res=new FullPermutationGenotype(sudoku,offspring);
//             res->validateGenotype(__LINE__);
//             result = res;


BaseGenotype* FullPermutationGenotype::crossover(BaseGenotype &other)
{
    BaseGenotype* result = nullptr;
    switch (other.getGenoType())
    {
    case FullPermutation:
    {
            FullPermutationGenotype& parent1 = *this;
            FullPermutationGenotype& parent2 = dynamic_cast<FullPermutationGenotype&>(other);

            // Ensure both parents have the same size
            assert(parent1.sudokunumbers.size() == parent2.sudokunumbers.size());

            size_t size = parent1.sudokunumbers.size();
            std::vector<short> offspring(size, -1);

            // Random number generator
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, size - 1);

            std::array<short,9> unusednumbers;
            unusednumbers.fill(0);
            for (std::size_t i = 0; i < size; ++i) 
            {
                if (parent1.sudokunumbers[i] == parent2.sudokunumbers[i]) 
                {
                    offspring[i] = parent1.sudokunumbers[i];
                }
                else
                {
                    unusednumbers[parent1.sudokunumbers[i]-1]++;
                }
            }
            for (std::size_t i = 0; i < size; ++i) 
            {
                if (offspring[i] == -1) 
                {
                    //select number frm une of parents at random and insert it  
                    std::uniform_int_distribution<> dis2(0, 1);
                    if (dis2(gen) == 0&&unusednumbers[parent1.sudokunumbers[i]-1]>0) 
                    {
                        offspring[i] = parent1.sudokunumbers[i];
                        unusednumbers[parent1.sudokunumbers[i]-1]--;
                        
                    }
                    else if(unusednumbers[parent2.sudokunumbers[i]-1]>0)
                    {
                        offspring[i] = parent2.sudokunumbers[i];
                        unusednumbers[parent2.sudokunumbers[i]-1]--;
                    }
                }
            }
            std::vector<short> rest;
            for (std::size_t i = 0; i < 9; ++i) 
            {
                for (short j = 0; j < unusednumbers[i]; ++j) 
                {
                    rest.push_back(i+1);
                }
            }
            std::shuffle(rest.begin(), rest.end(), gen);
            for (std::size_t i = 0; i < size; ++i) 
            {
                if (offspring[i] == -1) 
                {
                    offspring[i] = rest.back();
                    rest.pop_back();
                }
            }
            result = new FullPermutationGenotype(sudoku, offspring);
    }
    break;
    
    default:
        assert("ilegal crossover"&&0);
    }

    return result;
}

//TODO optimize probably
void FullPermutationGenotype::mutate(float mutationRate)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // Use a binomial distribution to determine the number of mutations
    std::binomial_distribution<> binom_dist(sudokunumbers.size(), mutationRate);
    size_t numMutations = binom_dist(gen)+1;
    
    // If numMutations is greater than 0, shuffle a subset of the vector
    if (numMutations > 1)
    {
        // Create a vector of indices
        std::vector<int> indices(sudokunumbers.size());
        std::iota(indices.begin(), indices.end(), 0);

        // Shuffle the indices
        std::shuffle(indices.begin(), indices.end(), gen);

        // Select the first numMutations indices
        std::vector<int> selectedIndices(indices.begin(), indices.begin() + numMutations);

        std::vector<int> subset;
        for (int index : selectedIndices)
        {
            subset.push_back(sudokunumbers[index]);
        }
        std::shuffle(subset.begin(), subset.end(), gen);

        // Place the shuffled subset back into sudokunumbers
        for (size_t i = 0; i < selectedIndices.size(); ++i)
        {
            sudokunumbers[selectedIndices[i]] = subset[i];
        }
    }
    // Invalidate the evaluation
    evalSudokuValid = false;
    validateGenotype(__LINE__);
}

void FullPermutationGenotype::fillEvalSudoku()
{
    int count = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] == -1) {
                evalSudoku[i][j] = sudokunumbers[count++];
            }
            else
            {
                evalSudoku[i][j] = sudoku[i][j];
            }
        }
    }
    evalSudokuValid = true;
}


RowPermutationGenotype::RowPermutationGenotype(const Sudoku& sudoku):BaseGenotype(sudoku)
{
    genoType = RowPermutation;
    evalSudokuValid = false;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    for(int i = 0; i < 9; i++)
    {
        std::vector<bool> remainingNumbers(9,true);
        for(int j = 0; j < 9; j++)
        {
            if(sudoku[i][j] != -1)
            {
                remainingNumbers[sudoku[i][j]-1]=false;
            }
        }
        for(int j = 0; j < 9; j++)
        {
            if(remainingNumbers[j])
            {
                rows[i].push_back(j+1);
            }
        }
        std::shuffle(rows[i].begin(),rows[i].end(),gen);
        
    }
}
RowPermutationGenotype::RowPermutationGenotype(const Sudoku& sudoku, std::array<std::vector<short>,9> rows):BaseGenotype(sudoku)
{
    genoType = RowPermutation;
    //use std::move
    evalSudokuValid = false;
    this->rows = rows;
}
BaseGenotype* RowPermutationGenotype::crossover(BaseGenotype &other)
{
    BaseGenotype* result = nullptr;
    switch (other.getGenoType())
    {
        case RowPermutation:
        {
            RowPermutationGenotype& parent1 = *this;
            RowPermutationGenotype& parent2 = dynamic_cast<RowPermutationGenotype&>(other);
            //select rows at random and create new pop using that
            // Random number generator
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 1);
            std::array<std::vector<short>, 9> offspringRows;
            // Select rows at random from the two parents
            for (size_t i = 0; i < parent1.rows.size(); ++i) {
                if (dis(gen) == 0) {
                    offspringRows[i] = parent1.rows[i];
                } else {
                    offspringRows[i] = parent2.rows[i];
                }
            }

            // Create the result genotype
            result = new RowPermutationGenotype(sudoku, offspringRows);
        }   
        break;
        default:
            assert("illegal crossover" && 0);
    }
    return result;
}
void RowPermutationGenotype::mutate(float mutationRate)
{
    evalSudokuValid = false;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    for (size_t i = 0; i < rows.size(); i++)
    {
        if (dis(gen) < mutationRate)
        {
            std::shuffle(rows[i].begin(), rows[i].end(), gen);
        }
    }
}
void RowPermutationGenotype::fillEvalSudoku()
{
    for (int i = 0; i < 9; i++) {
        int selected=0;
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] == -1) {
                evalSudoku[i][j] = rows[i][selected++];
            }
            else
            {
                evalSudoku[i][j] = sudoku[i][j];
            }
        }
    }
}