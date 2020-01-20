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
        auto row = dal::create_array<float>(table.row(2));
        const float* data = row.get_host_ptr();

        for (int col = 0; col < table.get_num_cols(); col++) {
            std::cout << data[col] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

