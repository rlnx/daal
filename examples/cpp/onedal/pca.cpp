#include "onedal/decomposition/pca.hpp"

int main(int argc, char const *argv[]) {
  using namespace dal::decomposition;

  const auto data = dal::table();
  const auto params = pca::params();
  const auto model = dal::train(params, data).get_model();

  return 0;
}
