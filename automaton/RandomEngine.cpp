#include <cmath>
#include <RandomEngine.h>

MatlabRandomEngine::MatlabRandomEngine(unsigned int seed): mt(), uniformGen(0.f, 1.f) {
    mt.seed(seed);
}

float MatlabRandomEngine::uniform() {
    auto result = uniformGen(mt);
    uniformGen(mt); // skip one value to replicate matlab behaviour
    return result;
}

static float normalDistributionValue(float mean, float stddev, float x) {
    auto coeff = (1 / std::sqrt(2 * M_PI_2f32 * stddev*stddev));
    float exponent = -((x - mean) * (x - mean) / 2 * stddev * stddev);
    return coeff * std::exp(exponent);
}

static std::vector<std::pair<float, float>> normalDistribution(
        float mean,
        float stddev,
        float leftPoint,
        float domLength,
        ul pointsNumber) {
    std::vector<std::pair<float, float>> result;
    result.reserve(pointsNumber);
    float step = domLength / pointsNumber;
    float x = leftPoint;
    for (ul i = 0; i < pointsNumber; ++i) {
        result.emplace_back(x, normalDistributionValue(mean, stddev, x));
        x += step;
    }
    return result;
}

float MatlabRandomEngine::normal(float mean, float stddev) {
    if (stddev <= 0) return mean;
    auto dist = normalDistribution(mean, stddev, mean - 6 * stddev, 12 * stddev, 100);
    std::vector<float> cumulativeSum;
    cumulativeSum.reserve(dist.size());
    float sum = 0;
    for (auto &point: dist) {
        sum += point.second;
        cumulativeSum.push_back(sum);
    }
    for (auto &prob: cumulativeSum) {
        prob = prob / sum;
    }
    auto random = uniform();
    for (ul i = 0; i < dist.size(); ++i) {
        if (random < cumulativeSum[i]) return dist[i].first;
    }
    return dist.back().first;
}

// Very inefficient implementation but gives the same results as original algorithm
// for traversing a collection in random order
std::vector<ul> MatlabRandomEngine::randomPermutation(ul n) {
    std::vector<ul> result;
    if (n == 0)
        return result;
    std::vector<bool> chosen(n);
    for (ul elemsChosen = 0; elemsChosen < n - 1; ++elemsChosen) {
        float minScore = 2.0;
        ul minIndex = 0;
        for (ul i = 0; i < n; ++i) { // every time we choose element with the lowest score
            if (!chosen[i]) {
                float score = uniform();
                if (score < minScore) {
                    minScore = score;
                    minIndex = i;
                }
            }
        }
        result.push_back(minIndex);
        chosen[minIndex] = true;
    }
    // add last element without drawing a score
    for (ul i = 0; i < n; ++i) {
        if (!chosen[i]) result.push_back(i);
    }
    return result;
}

ul RandomEngine::roulette(const std::vector<float> &probs) {
    std::vector<float> sums(probs.size());
    std::partial_sum(probs.begin(), probs.end(), sums.begin());
    float random = uniform();
    for (ul i = 0; i < probs.size(); ++i) {
        if (random < sums[i]) return i;
    }
    return probs.size();
}
