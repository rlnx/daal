#include <iomanip>
#include <iostream>

#include "onedal/table_homogen.hpp"
#include "onedal/decomposition/pca.hpp"

std::ostream &operator <<(std::ostream& stream, const dal::table& table) {
    const auto x = dal::flatten<float, dal::access_mode::read>(table);
    for (std::int64_t i = 0; i < table.get_row_count(); i++) {
        for (std::int64_t j = 0; j < table.get_column_count(); j++) {
            std::cout << std::setw(10)
                      << std::setiosflags(std::ios::fixed)
                      << std::setprecision(3)
                      << x[i * table.get_column_count() + j];
        }
        std::cout << std::endl;
    }
    return stream;
}

int main(int argc, char const *argv[]) {
    using namespace dal::decomposition;

    constexpr std::int64_t row_count = 5;
    constexpr std::int64_t column_count = 3;

    const float data[] = {
        1.f,  2.f,  3.f,
        1.f,  -1.f, 0.f,
        4.f,  5.f,  6.f,
        1.f,  2.f,  5.f,
        -4.f, 3.f,  0.f
    };

    const auto data_table = dal::table_homogen(data, row_count, column_count);

    const auto pca_desc = pca::descriptor()
        .set_component_count(3)
        .set_is_deterministic(true);

    const auto result = dal::train(pca_desc, data_table);

    std::cout << "Eigenvectors:" << std::endl
              << result.get_eigenvectors() << std::endl;

    std::cout << "Eigenvalues:" << std::endl
              << result.get_eigenvalues() << std::endl;

    return 0;
}
