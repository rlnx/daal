#include "onedal/data_management/create_table.hpp"

#include <iostream>

int main(int argc, char const *argv[])
{
    float data[] = {
        1.f, 2.f, 3.f,
        0.f, 0.f, 0.f,
        4.f, 5.f, 6.f
    };

    auto table = dal::data_management::create_table(data, 3, 3);
    std::cout <<  "table [" << table.get_num_rows() << "x" << table.get_num_cols() << "]" << std::endl;
    
    {
        auto slice = table.rows(dal::Range{0, 2}).col(1)/*.template to_array<float>()*/;
        std::cout << "slice [" << slice.get_num_rows() << "x" << slice.get_num_cols() << "]" << std::endl;
        // float* data_ptr = arr.get_ptr();
    }

    return 0;
}

