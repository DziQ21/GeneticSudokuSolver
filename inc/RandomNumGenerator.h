#include <random>

class RandomGenerator {
private:
    static RandomGenerator *instance;
    RandomGenerator() : gen(std::random_device{}()) {}

    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;
public:
    std::mt19937 gen;
    static RandomGenerator& getInstance() {
        if (!instance) {
            instance = new RandomGenerator();
        }
        return *instance;
    }

    void setSeed(unsigned int seed) {
        gen.seed(seed);
    }


};