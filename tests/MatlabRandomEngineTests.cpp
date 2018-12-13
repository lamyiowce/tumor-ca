#include <catch.h>
#include <RandomEngine.h>
#include <cmath>

// For reason I don't understand, in some cases Catch Approx macro doesn't work correctly
#define MAX_ERROR 0.00001f
#define FLOAT_EQUAL(F1, F2) (std::abs(F1 - F2) < MAX_ERROR)

TEST_CASE("Uniform random replicates results from matlab.") {
    MatlabRandomEngine mre(7);
    REQUIRE(mre.uniform() == Approx(0.0763082893));
    REQUIRE(mre.uniform() == Approx(0.7799187922));
    REQUIRE(mre.uniform() == Approx(0.4384092314));
    REQUIRE(mre.uniform() == Approx(0.7234651778));
}

TEST_CASE("Normal distributed random replicates results from matlab.") {
    MatlabRandomEngine mre(7);
    REQUIRE(mre.normal(0, 1) == Approx(-1.44f));
    REQUIRE(mre.normal(0, 1) == Approx(0.72f));
    REQUIRE(FLOAT_EQUAL(mre.normal(0, 1), -0.12f));
    REQUIRE(mre.normal(0, 1) == Approx(0.6f));
}

TEST_CASE("Random permutation gives the same order as original algorithm.") {
    MatlabRandomEngine mre(7);
    REQUIRE(mre.randomPermutation(10) == std::vector<ul>{7, 3, 0, 5, 9, 2, 8, 4, 6, 1});
    REQUIRE(mre.randomPermutation(3) == std::vector<ul>{1, 2, 0});
    REQUIRE(mre.randomPermutation(4) == std::vector<ul>{3, 2, 1, 0});
    REQUIRE(mre.randomPermutation(1) == std::vector<ul>{0});
    REQUIRE(mre.randomPermutation(5) == std::vector<ul>{3, 4, 2, 0, 1});
}

TEST_CASE("Random roulette replicates results from matlab.") {
    MatlabRandomEngine mre(7);
    std::vector<float> probs{0.2f, 0.1f, 0.2f, 0.4f, 0.05f, 0.05f};
    REQUIRE(mre.roulette(probs) == 0);
    REQUIRE(mre.roulette(probs) == 3);
    REQUIRE(mre.roulette(probs) == 2);
    REQUIRE(mre.roulette(probs) == 3);
    REQUIRE(mre.roulette(probs) == 5);
}

TEST_CASE("Mixed calls replicate results from matlab.") {
    MatlabRandomEngine mre(167);
    std::vector<float> probs{0.2f, 0.1f, 0.4f, 0.05f, 0.25f};
    REQUIRE(mre.uniform() == Approx(0.898379048));
    REQUIRE(mre.normal(0, 1) == Approx(-0.96));
    REQUIRE(mre.normal(0, 1) == Approx(-0.36));
    REQUIRE(mre.uniform() == Approx(0.273527979));
    REQUIRE(mre.randomPermutation(6) == std::vector<ul>{2, 3, 1, 0, 5, 4});
    REQUIRE(mre.roulette(probs) == 1);
    REQUIRE(mre.randomPermutation(3) == std::vector<ul>{2, 1, 0});
    REQUIRE(mre.roulette(probs) == 4);
}