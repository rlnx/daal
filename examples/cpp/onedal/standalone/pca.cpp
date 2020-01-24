#include <iostream>

#include "onedal/decomposition/pca.hpp"
#include "onedal/homogen_table.hpp"

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
        const auto eigvec = model.get_eigenvectors();
        std::cout << "eigenvectors "
                  << "[" << eigvec.get_num_rows()
                  << "x" << eigvec.get_num_cols()
                  << "]:" << std::endl;

        for (int vector_num = 0; vector_num < eigvec.get_num_cols(); vector_num++) {
            std::cout << "(" << vector_num << "): ";

            auto eigvec_array = dal::flatten<float, dal::access_mode::read>(
                eigvec, dal::column_range({vector_num, vector_num+1}));

            float *eigvec_ptr = eigvec_array.get_pointer();

            for(int i = 0; i < eigvec_array.get_size(); i++) {
                std::cout << eigvec_ptr[i] << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
