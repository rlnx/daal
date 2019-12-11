/* file: cor_dense_batch.cpp */
/*******************************************************************************
* Copyright 2014-2019 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/*
!  Content:
!    C++ example of dense correlation matrix computation in the batch
!    processing mode with DPC++ interfaces
!
!******************************************************************************/

/**
 * <a name="DAAL-EXAMPLE-CPP-CORRELATION_DENSE_BATCH"></a>
 * \example cor_dense_batch.cpp
 */


#define DAAL_EXECUTION_CONTEXT_VERBOSE

#include "daal_sycl.h"
#include "service.h"
#include "service_sycl.h"

using namespace std;
using namespace daal;
using namespace daal::algorithms;

using float16_t = daal::float16;

struct TablePrinter
{
    explicit TablePrinter(const NumericTablePtr &table)
        : table(table) { }
    NumericTablePtr table;
};


std::ostream &operator <<(std::ostream &stream, const TablePrinter &p)
{
    auto t = SyclHomogenNumericTable<float16_t>::cast(p.table);
    BlockDescriptor<float16_t> bd;
    t->getBlockOfRows(0, t->getNumberOfRows(), readOnly, bd);

    const size_t rows = t->getNumberOfRows();
    const size_t cols = t->getNumberOfColumns();

    auto buff = bd.getBuffer().toSycl();
    auto buf_acc = buff.get_access<cl::sycl::access::mode::read>();

    std::cout << std::setiosflags(std::ios::left);
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            std::cout << std::setw(10) << std::setiosflags(std::ios::fixed) << std::setprecision(3);
            std::cout << buf_acc[i * cols + j];
        }
        std::cout << std::endl;
    }

    t->releaseBlockOfRows(bd);
    return stream;
}

uint32_t generateMinStd(uint32_t x)
{
    constexpr uint32_t a = 16807;
    constexpr uint32_t c = 0;
    constexpr uint32_t m = 2147483647;
    return (a * x + c) % m;
}

cl::sycl::event generateData(cl::sycl::queue &q, cl::sycl::half *deviceData,
                             size_t nRows, size_t nCols)
{
    using namespace cl::sycl;
    return q.submit([&](handler &cgh)
    {
        cgh.parallel_for<class FillTable>(range<1>(nRows), [=](id<1> idx)
        {
            constexpr float genMax = 2147483647.0f;
            uint32_t genState = 7777 + idx[0] * idx[0];
            genState = generateMinStd(genState);
            genState = generateMinStd(genState);
            for (size_t j = 0; j < nCols; j++)
            {
                deviceData[idx[0] * nCols + j] = (cl::sycl::half)(genState / genMax);
                genState = generateMinStd(genState);
            }
        });
    });
}

int main(int argc, char * argv[])
{
    constexpr size_t nCols = 10;
    constexpr size_t nRows = 500;

    auto queue = cl::sycl::queue{cl::sycl::gpu_selector{}};

    services::Environment::getInstance()->setDefaultExecutionContext(
        services::SyclExecutionContext{queue}
    );

    float16_t *dataDevice = (float16_t *)cl::sycl::malloc_shared(
        sizeof(float16_t) * nRows * nCols, queue.get_device(), queue.get_context());

    generateData(queue, dataDevice, nRows, nCols).wait();

    NumericTablePtr dataTable = SyclHomogenNumericTable<float16_t>::create(
        dataDevice, nCols, nRows, cl::sycl::usm::alloc::shared);

    covariance::Batch<float16_t> algorithm;
    algorithm.input.set(covariance::data, dataTable);
    algorithm.parameter.outputMatrixType = covariance::correlationMatrix;
    algorithm.compute();
    const auto res = algorithm.getResult();

    std::cout << "Correlation matrix: " << std::endl
              << TablePrinter{res->get(covariance::correlation)} << std::endl;
    std::cout << "Mean: " << TablePrinter{res->get(covariance::mean)} << std::endl;

    cl::sycl::free(dataDevice, queue.get_context());

    return 0;
}
