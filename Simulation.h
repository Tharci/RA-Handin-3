//
// Created by tmarc on 11/05/2023.
//

#ifndef RA_HANDIN_3_CPP_SIMULATION_H
#define RA_HANDIN_3_CPP_SIMULATION_H

#include <chrono>
#include <random>
#include <numeric>
#include <vector>
#include <string>
#include <fstream>


struct SimulationResult {
    int costumers;
    int tickets;
    double income;
    double stdDev;
    long long time;

    friend std::ostream& operator<<(std::ostream& os, const SimulationResult& result) {
        os << "Customers: " << result.costumers << ", Tickets: " << result.tickets
           << ", Income: " << result.income << ", Standard Deviation: " << result.stdDev
           << ", Time: " << result.time << "ms";
        return os;
    }
};


template <class StrategyType>
class Simulation {
    std::mt19937_64 rng;
    std::uniform_real_distribution<double> unif{0., 1.};
    std::vector<SimulationResult> results;

public:
    Simulation() {
        // initialize the random number generator with time-dependent seed
        uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
        rng.seed(ss);
    }

    [[nodiscard]] double simulateOnce(int all_costumers, int all_tickets) {
        auto strategy = StrategyType(all_costumers, all_tickets);
        auto tickets = all_tickets;
        double income = 0.;
        for(int customers = all_costumers; customers > 0; customers--) {
            auto price = strategy.calculate_price(customers, tickets);
            auto offer = unif(rng);
            if (offer > price) {
                tickets--;
                income += price;
                if (tickets == 0) {
                    break;
                }
            }
        }

        return income;
    }

    [[nodiscard]] SimulationResult simulate(int all_costumers, int all_tickets, int n_sim = 1000) {
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<double> incomes(n_sim);
        for (int i = 0; i < n_sim; i++) {
            incomes[i] = simulateOnce(all_costumers, all_tickets);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        double income = std::accumulate(incomes.begin(), incomes.end(), 0.0) / incomes.size();
        double sq_sum = std::inner_product(incomes.begin(), incomes.end(), incomes.begin(), 0.0);
        double stdev = std::sqrt(sq_sum / incomes.size() - income * income);

        SimulationResult result = {all_costumers, all_tickets, income, stdev, duration};
        results.push_back(result);
        return result;
    }

    void dump(const std::string& filename) {
        std::ofstream file;
        file.open(filename);
        file << "costumers,tickets,income,stdDev,time" << std::endl;
        for (auto result : results) {
            file << result.costumers << "," << result.tickets << "," << result.income << "," << result.stdDev << "," << result.time << std::endl;
        }
        file.close();
    }
};

#endif //RA_HANDIN_3_CPP_SIMULATION_H
