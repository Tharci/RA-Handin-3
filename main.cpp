#include <iostream>
#include <chrono>

#include "Simulation.h"
#include "PricingStrategy.h"


int main() {
    /*
    int n = 1000;
    int k = 10;

    Simulation<ObviousPricingStrategy> obviousSim;
    auto obviousIncome = obviousSim.simulate(n, k, 1000);
    std::cout << "OBVIOUS:\n" <<  obviousIncome << std::endl << std::endl;

    Simulation<FancyPricingStrategy> fancySim;
    auto fancyIncome = fancySim.simulate(n, k, 1000);
    std::cout << "FANCY:\n" << fancyIncome << std::endl << std::endl;
    */

    Simulation<ObviousPricingStrategy> obviousSim;
    Simulation<FancyPricingStrategy> fancySim;

    for (int n = 50; n <= 1000; n += 50) {
        const std::vector<int> tickets = {1, 10, 50, 200};
        for (auto k: tickets) {
            if (k >= n/2.) continue;

            std::cout << "n = " << n << ", k = " << k << std::endl;
            auto obviousIncome = obviousSim.simulate(n, k, 1000);
            std::cout << "OBVIOUS:\n" <<  obviousIncome << std::endl << std::endl;

            auto fancyIncome = fancySim.simulate(n, k, 1000);
            std::cout << "FANCY:\n" << fancyIncome << std::endl << std::endl;
        }
    }

    obviousSim.dump("obvious.csv");
    fancySim.dump("fancy.csv");
}
