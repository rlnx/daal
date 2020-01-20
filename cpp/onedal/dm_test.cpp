#include "onedal/data_management/create_table.hpp"
#include "onedal/data_management/create_array.hpp"

#include <iostream>

int main(int argc, char const *argv[])
{
    float data[] = {
        1.f, 2.f, 3.f,
        0.f, 0.f, 0.f,
        4.f, 5.f, 6.f
    };

    auto table = dal::create_table(data, 3, 3);
    std::cout <<  "table [" << table.get_num_rows() << "x" << table.get_num_cols() << "]" << std::endl;

    {
        auto slice = table.rows({0, 3, 2}).cols(0);
        std::cout << "slice [" << slice.get_num_rows() << "x" << slice.get_num_cols() << "]" << std::endl;

        auto array = dal::create_array<float>(slice);
        const float* data = array.get_host_ptr();

        for (int row = 0; row < slice.get_num_rows(); row++) {
            for (int col = 0; col < slice.get_num_cols(); col++) {
                std::cout << data[row*slice.get_num_cols() + col] << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}

