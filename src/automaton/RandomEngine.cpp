#include <RandomEngine.h>
#include <cmath>
#include <algorithm>

MatlabRandomEngine::MatlabRandomEngine(unsigned int seed): mt(seed), uniformGen(0.f, 1.f) {}

float MatlabRandomEngine::uniform() {
    auto result = uniformGen(mt);
    uniformGen(mt); // skip one value to replicate matlab behaviour
    return result;
}

static double_p normalDistributionValue(double_p mean, double_p stddev, double_p x) {
    double_p coeff = (1.f / std::sqrt(2.f * M_PI * stddev*stddev));
    double_p exponent = -((x - mean) * (x - mean) / 2 * stddev * stddev);
    return coeff * std::exp(exponent);
}

static std::vector<std::pair<double_p, double_p>> normalDistribution(
        double_p mean,
        double_p stddev,
        double_p leftPoint,
        double_p domLength,
        ul pointsNumber) {
    std::vector<std::pair<double_p, double_p>> result{};
    result.reserve(pointsNumber);
    double_p step = domLength / pointsNumber;
    double_p x = leftPoint;
    for (ul i = 0; i < pointsNumber; ++i) {
        result.emplace_back(x, normalDistributionValue(mean, stddev, x));
        x += step;
    }
    return result;
}

double_p MatlabRandomEngine::normal(double_p mean, double_p stddev) {
    if (stddev <= 0) return mean;
    auto dist = normalDistribution(mean, stddev, mean - 6 * stddev, 12 * stddev, 100);
    std::vector<double_p > cumulativeSum{};
    cumulativeSum.reserve(dist.size());
    double_p sum = 0;
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
    if (n == 0)
        return {};
    else if (n == 1)
        return {0};
    std::vector<ul> result(n);
    std::vector<std::pair<float, int>> perm;
    for (ul i = 0; i < n; ++i) {
        perm.emplace_back(uniform(), i);
    }
    std::sort(perm.begin(), perm.end());
    std::transform(perm.begin(), perm.end(), result.begin(),
                   [](auto p){ return p.second;});
    return result;
}

ul RandomEngine::roulette(const std::vector<double> &probs) {
    std::vector<double> sums(probs.size());
    std::partial_sum(probs.begin(), probs.end(), sums.begin());
    float random = uniform();
    for (ul i = 0; i < probs.size(); ++i) {
        // TODO Comment
        if (i == 7) {
            return 8;
        }
        if (random < sums[i]) return i;
    }
    return probs.size();
}

StdRandomEngine::StdRandomEngine(unsigned int seed): mt(seed), uniformGen(0.0f, 1.0f), normalGen{} {}

float StdRandomEngine::uniform() {
    return uniformGen(mt);
}

double_p StdRandomEngine::normal(double_p mean, double_p stddev) {
    return normalGen(mt) * stddev + mean;
}

std::vector<ul> StdRandomEngine::randomPermutation(ul n) {
    std::vector<ul> result(n);
    std::iota(result.begin(), result.end(), 0);
    std::shuffle(result.begin(), result.end(), mt);
    return result;
}
