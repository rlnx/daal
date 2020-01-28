#include <iomanip>
#include <iostream>

#include "onedal/optimization/lbfgs_solver.hpp"
#include "onedal/linear_model/logistic_regression.hpp"


int main(int argc, char const *argv[]) {
    const auto solver = dal::optimization::lbfgs_solver();
    const auto params = dal::linear_model::logistic_regression::params(solver);



    // dal::train(params, data, )

    return 0;
}
