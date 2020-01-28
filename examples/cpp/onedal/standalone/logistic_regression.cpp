#include <iomanip>
#include <iostream>

#include "onedal/optimization/lbfgs/solver.hpp"
#include "onedal/linear_model/logistic_regression.hpp"

int main(int argc, char const *argv[]) {
    using namespace dal::optimization;
    using namespace dal::linear_model;

    const auto data = dal::table();
    const auto labels = dal::table();

    const auto solver = lbfgs::solver();
    const auto logreg = logistic_regression::estimator(solver);
    const auto logreg_model = dal::train(logreg, data, labels).get_model();

    return 0;
}
