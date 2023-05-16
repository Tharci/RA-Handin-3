//
// Created by tmarc on 11/05/2023.
//

#ifndef RA_HANDIN_3_CPP_PRICINGSTRATEGY_H
#define RA_HANDIN_3_CPP_PRICINGSTRATEGY_H

#include <vector>
#include <cmath>


class PricingStrategy {
protected:
    int all_costumers;
    int all_tickets;

public:
    PricingStrategy(int all_costumers, int all_tickets)
            : all_costumers(all_costumers), all_tickets(all_tickets) {}

    [[nodiscard]] virtual float calculate_price(int customers, int tickets) const = 0;
};


class ObviousPricingStrategy : public PricingStrategy {
    float price;

public:
    ObviousPricingStrategy(int all_costumers, int all_tickets)
    : PricingStrategy(all_costumers, all_tickets) {
        price = 1. - ((float)all_tickets / all_costumers);
    }

    [[nodiscard]] float calculate_price(int customers, int tickets) const override {
        return price;
    }
};


class FancyPricingStrategy : public PricingStrategy {
    std::vector<float> prices;
    std::vector<float> expected;
    std::pair<int, int> dim;

public:
    FancyPricingStrategy(int all_costumers, int all_tickets)
            : PricingStrategy(all_costumers, all_tickets) {
        dim = std::make_pair(all_costumers, std::min(all_tickets, all_costumers));
        prices = std::vector<float>(dim.first * dim.second);
        expected = std::vector<float>(dim.first * dim.second);
        set_array(expected, 0, 0, 1/2.);
        set_array(prices, 0, 0, 1/4.);

        for (int costumer = 1; costumer < all_costumers; costumer++) {
            for (int ticket = 0; ticket < std::min(costumer+1, all_tickets); ticket++) {
                float a = -1;
                float exp_1 = get_exp(costumer-1, ticket-1);
                float b = (1 - exp_1 + get_exp(costumer-1, ticket));
                float c = exp_1;
                float p = calculate_max_price(a, b, c);
                float exp = a * p * p + b * p + c;
                set_array(expected, costumer, ticket, exp);
                set_array(prices, costumer, ticket, p);
            }
        }
    }

    float get_exp(int costumer, int ticket) {
        if (ticket < 0) {
            return 0.;
        }
        else {
            return expected[costumer * dim.second + std::min(ticket, dim.first-1)];
        }
    }

    [[nodiscard]] float get_price(int costumer, int ticket) const {
        return prices[costumer * dim.second + std::min(ticket, dim.first-1)];
    }

    [[nodiscard]] float calculate_price(int costumer, int ticket) const override {
        return get_price(costumer-1, ticket-1);
    }

    static float calculate_max_price(float a, float b, float c) {
        auto d = (b * b) - (4 * a * c);
        auto sol1 = (-b - std::sqrt(d)) / (2 * a);
        auto sol2 = (-b + std::sqrt(d)) / (2 * a);
        return (sol1 + sol2) / 2;
    }

    void set_array(std::vector<float>& array, int i, int j, float value) const {
        array[i * dim.second + j] = value;
    }
};



#endif //RA_HANDIN_3_CPP_PRICINGSTRATEGY_H
