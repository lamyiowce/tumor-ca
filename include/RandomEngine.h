#ifndef TUMOR_RANDOMENGINE_H
#define TUMOR_RANDOMENGINE_H

#include <vector>
#include <random>
#include <util.h>

class RandomEngine {
public:

    /**
     * Random real number from [0, 1] with uniform distribution.
     */
    virtual float uniform() = 0;

    /**
     * Random real number with normal distribution for given mean value and standard deviation.
     * For negative stddev, returns mean value
     * @param mean - mean value
     * @param stddev - standard deviation
     */
    virtual float normal(float mean, float stddev) = 0;

    /**
     * Returns random permutation of numbers form 0 to (n - 1)
     * @param n - length of permutation
     */
    virtual std::vector<ul> randomPermutation(ul n) = 0;

    /**
     * Choose random number with given probabilities.
     * Returns number i from 0 to probs.size()-1 with the probs[i] probability.
     * If sum of probs is lower than 1.0, probs.size() is returned with (1.0 - sum(probs)) probability
     * @param probs - probabilities
     */
    virtual ul roulette(const std::vector<float> &probs);
};

class MatlabRandomEngine: public RandomEngine {

    std::mt19937 mt;
    std::uniform_real_distribution<float> uniformGen;

public:
    explicit MatlabRandomEngine(unsigned int seed);

    float uniform() override;

    float normal(float mean, float stddev) override;

    std::vector<ul> randomPermutation(ul n) override;

};


#endif