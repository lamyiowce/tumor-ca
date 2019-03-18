#include <catch.h>
#include <IrradiationProtocol.h>
#include "StateHelper.h"

TEST_CASE("Irradiation Protocol - basic read from file test") {
    std::vector<std::string> protocolPaths = {"protocols_single", "protocols_simple"};
    std::vector<std::vector<IrradiationProtocol>> shouldBe{
            {
                IrradiationProtocol({1, 2, 3, 4}, {1, 2, 0.6, 4})
            },
            {
                IrradiationProtocol({1, 2, 3, 4, 5}, {0.1, 0.2, 0.3, 0.4, 0.5}),
                IrradiationProtocol({6, 7, 8, 9, 10}, {0.6, 0.7, 0.8, 0.9, 1.0}),
                IrradiationProtocol({11, 12, 13, 14, 15}, {1.1, 1.2, 1.3, 1.4, 1.5})
            }
    };
    std::string baseTestPath = "../tests/resources/test_protocols/";
    std::string currentTest;
    assert(protocolPaths.size() == shouldBe.size());
    for (int i = 0; i < protocolPaths.size(); i++) {
        auto testCase = protocolPaths[i];
        SECTION("Test case " + testCase) {
            currentTest = baseTestPath + testCase;
            auto protocols = IrradiationProtocol::loadFromFile(currentTest);
            REQUIRE(protocols.size() == shouldBe[i].size());
            for (ul j = 0; j < protocols.size(); j++)
                REQUIRE(protocols[j].sameAs(shouldBe[i][j]));
        }
    }
}

