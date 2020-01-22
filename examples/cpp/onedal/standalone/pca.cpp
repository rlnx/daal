#include <iostream>

#include "onedal/decomposition/pca.hpp"
#include "onedal/data_management/create_table.hpp"
#include "onedal/data_management/create_array.hpp"

int main(int argc, char const *argv[]) {
    using namespace dal::decomposition;

    float data[] = {
        1.f,  2.f,  3.f,
        1.f,  -1.f, 0.f,
        4.f,  5.f,  6.f,
        1.f,  2.f,  5.f,
        -4.f, 3.f,  0.f
    };

    auto table = dal::create_table(data, 5, 3);
    const auto params = pca::params<>()
                            .set_components_count(2)
                            .set_is_deterministic(true);

    const auto model = dal::train(params, table).get_model();

    {
        auto eigvec = model.get_eigenvectors();
        std::cout << "eigenvectors "
                  << "[" << eigvec.get_num_rows()
                  << "x" << eigvec.get_num_cols()
                  << "]:" << std::endl;

        for (int vector_num = 0; vector_num < eigvec.get_num_cols(); vector_num++) {
            std::cout << "(" << vector_num << "): ";

            auto vec = dal::create_array<float>(eigvec.cols(vector_num));
            const float* vec_data = vec.get_host_ptr();

            for(int i = 0; i < eigvec.get_num_rows(); i++) {
                std::cout << vec_data[i] << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
