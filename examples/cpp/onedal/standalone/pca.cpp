#include "onedal/decomposition/pca.hpp"
#include "onedal/data_management/create_table.hpp"
#include "onedal/data_management/create_array.hpp"

#include <iostream>

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
    std::cout << "eigenvectors [" << eigvec.get_num_rows() << "x" << eigvec.get_num_cols() << "]:" << std::endl;

    auto rows = dal::create_array<float>(eigvec.rows({0, -1}));
    const float* data = rows.get_host_ptr();

    const int ld = eigvec.get_num_cols();
    for (int row = 0; row < eigvec.get_num_rows(); row++) {
      for (int col = 0; col < table.get_num_cols(); col++) {
        std::cout << data[row*ld + col] << " ";
      }
      std::cout << std::endl;
    }
  }

  return 0;
}
