#include <iomanip>
#include <iostream>

#include "onedal/random/mt19937.hpp"
#include "onedal/optimization/lbfgs.hpp"
#include "onedal/linear_model/logistic_regression.hpp"

int main(int argc, char const *argv[]) {
    using namespace dal::random;
    using namespace dal::optimization;
    using namespace dal::linear_model;

    const auto data = dal::table();
    const auto labels = dal::table();

    const auto mt = mt19937::generator(7777);

    const auto solver = lbfgs::solver()
        .set_accuracy_threshold(0.0001)
        .set_max_iteration_count(1000);

    const auto logreg_params = logistic_regression::params()
        .set_penalty_l1(0.0)
        .set_penalty_l2(0.5)
        .set_intercept_flag(true);

    // const auto logreg = logistic_regression::estimator(solver, mt);

    // 1. Use cases of state for ML algorithms
    // 1.1. What kind of entities we need to support these use cases and their definition
    // 2. Consistency between complex and simple cases (ML vs Low order moments)
    // 3. Order of arguments in train/infer/... for readability
    // 4. How minimal example looks like (implace objects)

    const auto logreg_params = logistic_regression::params()
        .set_penalty_l1(0.0)
        .set_penalty_l2(0.5);

    // logistic_regression::train(logreg_params, data, labels);

    const auto logreg_model = dal::train(logistic_regression::algorithm(), data, labels).get_model();

    return 0;
}
