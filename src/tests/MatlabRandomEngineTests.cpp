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

TEST_CASE("Random roulette replicates results from matlab.") {
    MatlabRandomEngine mre(7);
    std::vector<double> probs{0.2, 0.1, 0.2, 0.4, 0.05, 0.05};
    REQUIRE(mre.roulette(probs) == 0);
    REQUIRE(mre.roulette(probs) == 3);
    REQUIRE(mre.roulette(probs) == 2);
    REQUIRE(mre.roulette(probs) == 3);
    REQUIRE(mre.roulette(probs) == 5);
}
