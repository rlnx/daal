/* file: numeric_table_sycl_homogen.h */
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

#ifndef __SYCL_HOMOGEN_NUMERIC_TABLE_H__
#define __SYCL_HOMOGEN_NUMERIC_TABLE_H__

#ifdef DAAL_SYCL_INTERFACE
#include <CL/sycl.hpp>
#endif

#include "data_management/data/numeric_table_sycl.h"
#include "data_management/data/internal/conversion.h"
#include "data_management/data/homogen_numeric_table.h"
#include "oneapi/internal/utils.h"

namespace daal
{
namespace data_management
{
namespace interface1
{
/**
 * @ingroup sycl
 * @{
 */

/**
 *  <a name="DAAL-CLASS-DATA_MANAGEMENT__SYCLHOMOGENNUMERICTABLE"></a>
 *  \brief Class that provides methods to access data stored as a one-dimentional SYCL* buffer.
 *  Table rows contain feature vectors, and columns contain values of individual features.
 *  \tparam DataType Defines the underlying data type that describes a Numeric Table
 */
template <typename DataType = DAAL_DATA_TYPE>
class DAAL_EXPORT SyclHomogenNumericTable : public SyclNumericTable
{
public:
    DECLARE_SERIALIZABLE_TAG();
    DECLARE_SERIALIZABLE_IMPL();

    DAAL_CAST_OPERATOR(SyclHomogenNumericTable)

public:
    /**
     *  Constructs a Numeric Table with buffer object
     *  \param[in]  buffer         Buffer with a homogeneous data set
     *  \param[in]  nColumns       Number of columns in the table
     *  \param[in]  nRows          Number of rows in the table
     *  \param[out] stat           Status of the numeric table construction
     *  \return     Numeric table with user-allocated memory
     */
    static services::SharedPtr<SyclHomogenNumericTable<DataType> > create(const services::Buffer<DataType> & buffer, size_t nColumns = 0,
                                                                          size_t nRows = 0, services::Status * stat = NULL)
    {
        DAAL_DEFAULT_CREATE_TEMPLATE_IMPL_EX(SyclHomogenNumericTable, DataType, DictionaryIface::notEqual, buffer, nColumns, nRows);
    }

#ifdef DAAL_SYCL_INTERFACE_USM
    static services::SharedPtr<SyclHomogenNumericTable<DataType> >
        create(const services::SharedPtr<DataType> &usmData,
               size_t nColumns, size_t nRows,
               const cl::sycl::usm::alloc &usmAllocType,
               services::Status *stat = NULL)
    {
        const size_t bufferSize = nColumns * nRows;
        return create(services::Buffer<DataType>(usmData, bufferSize, usmAllocType), nColumns, nRows, stat);
    }
#endif

#ifdef DAAL_SYCL_INTERFACE_USM
    static services::SharedPtr<SyclHomogenNumericTable<DataType> >
        create(DataType *usmData,
               size_t nColumns, size_t nRows,
               const cl::sycl::usm::alloc &usmAllocType,
               services::Status *stat = NULL)
    {
        const size_t bufferSize = nColumns * nRows;
        return create(services::Buffer<DataType>(usmData, bufferSize, usmAllocType), nColumns, nRows, stat);
    }
#endif

    /**
     *  Constructs a Numeric Table
     *  \param[in]  nColumns              Number of columns in the table
     *  \param[in]  nRows                 Number of rows in the table
     *  \param[in]  memoryAllocationFlag  Flag that controls internal memory allocation for data in the numeric table
     *  \param[out] stat                  Status of the numeric table construction
     *  \return     Numeric table with user-allocated memory
     */
    static services::SharedPtr<SyclHomogenNumericTable<DataType> > create(size_t nColumns, size_t nRows, AllocationFlag memoryAllocationFlag,
                                                                          services::Status * stat = NULL)
    {
        DAAL_DEFAULT_CREATE_TEMPLATE_IMPL_EX(SyclHomogenNumericTable, DataType, DictionaryIface::notEqual, nColumns, nRows, memoryAllocationFlag);
    }

    /**
     *  Constructs a Numeric Table with memory allocation controlled via a flag and fills the table with a constant
     *  \param[in]  nColumns                Number of columns in the table
     *  \param[in]  nRows                   Number of rows in the table
     *  \param[in]  memoryAllocationFlag    Flag that controls internal memory allocation for data in the numeric table
     *  \param[in]  constValue              Constant to initialize entries of the homogeneous numeric table
     *  \param[out] stat                    Status of the numeric table construction
     *  \return     Numeric table initialized with a constant
     */
    static services::SharedPtr<SyclHomogenNumericTable<DataType> > create(size_t nColumns, size_t nRows, AllocationFlag memoryAllocationFlag,
                                                                          const DataType & constValue, services::Status * stat = NULL)
    {
        DAAL_DEFAULT_CREATE_TEMPLATE_IMPL_EX(SyclHomogenNumericTable, DataType, DictionaryIface::notEqual, nColumns, nRows, memoryAllocationFlag,
                                             constValue);
    }

    SyclHomogenNumericTable() : SyclNumericTable(0, 0, DictionaryIface::notEqual) {}

    ~SyclHomogenNumericTable() DAAL_C11_OVERRIDE { freeDataMemoryImpl(); }

    services::Status getBlockOfRows(size_t vector_idx, size_t vector_num, ReadWriteMode rwflag, BlockDescriptor<double> & block) DAAL_C11_OVERRIDE
    {
        return getTBlock<double>(vector_idx, vector_num, rwflag, block);
    }

    services::Status getBlockOfRows(size_t vector_idx, size_t vector_num, ReadWriteMode rwflag, BlockDescriptor<float> & block) DAAL_C11_OVERRIDE
    {
        return getTBlock<float>(vector_idx, vector_num, rwflag, block);
    }

    services::Status getBlockOfRows(size_t vector_idx, size_t vector_num, ReadWriteMode rwflag, BlockDescriptor<int> & block) DAAL_C11_OVERRIDE
    {
        return getTBlock<int>(vector_idx, vector_num, rwflag, block);
    }

    services::Status getBlockOfRows(size_t vector_idx, size_t vector_num, ReadWriteMode rwflag, BlockDescriptor<float16> & block) DAAL_C11_OVERRIDE
    {
        return getTBlock<float16>(vector_idx, vector_num, rwflag, block);
    }

    services::Status releaseBlockOfRows(BlockDescriptor<double> & block) DAAL_C11_OVERRIDE { return releaseTBlock<double>(block); }

    services::Status releaseBlockOfRows(BlockDescriptor<float> & block) DAAL_C11_OVERRIDE { return releaseTBlock<float>(block); }

    services::Status releaseBlockOfRows(BlockDescriptor<int> & block) DAAL_C11_OVERRIDE { return releaseTBlock<int>(block); }

    services::Status releaseBlockOfRows(BlockDescriptor<float16> & block) DAAL_C11_OVERRIDE { return releaseTBlock<float16>(block); }

    services::Status getBlockOfColumnValues(size_t feature_idx, size_t vector_idx, size_t value_num, ReadWriteMode rwflag,
                                            BlockDescriptor<double> & block) DAAL_C11_OVERRIDE
    {
        return getTFeature<double>(feature_idx, vector_idx, value_num, rwflag, block);
    }

    services::Status getBlockOfColumnValues(size_t feature_idx, size_t vector_idx, size_t value_num, ReadWriteMode rwflag,
                                            BlockDescriptor<float> & block) DAAL_C11_OVERRIDE
    {
        return getTFeature<float>(feature_idx, vector_idx, value_num, rwflag, block);
    }

    services::Status getBlockOfColumnValues(size_t feature_idx, size_t vector_idx, size_t value_num, ReadWriteMode rwflag,
                                            BlockDescriptor<int> & block) DAAL_C11_OVERRIDE
    {
        return getTFeature<int>(feature_idx, vector_idx, value_num, rwflag, block);
    }

    services::Status releaseBlockOfColumnValues(BlockDescriptor<double> & block) DAAL_C11_OVERRIDE { return releaseTFeature<double>(block); }

    services::Status releaseBlockOfColumnValues(BlockDescriptor<float> & block) DAAL_C11_OVERRIDE { return releaseTFeature<float>(block); }

    services::Status releaseBlockOfColumnValues(BlockDescriptor<int> & block) DAAL_C11_OVERRIDE { return releaseTFeature<int>(block); }

    services::Status assign(float value) DAAL_C11_OVERRIDE { return assignImpl<float>(value); }

    services::Status assign(double value) DAAL_C11_OVERRIDE { return assignImpl<double>(value); }

    services::Status assign(int value) DAAL_C11_OVERRIDE { return assignImpl<int>(value); }

protected:
    SyclHomogenNumericTable(DictionaryIface::FeaturesEqual featuresEqual,
                            size_t nColumns, size_t nRows,
                            services::Status &st) :
        SyclNumericTable(nColumns, nRows, featuresEqual, st)
    {
        _layout = NumericTableIface::aos;

        NumericTableFeature df;
        df.setType<DataType>();
        st |= _ddict->setAllFeatures(df);
    }

    SyclHomogenNumericTable(DictionaryIface::FeaturesEqual featuresEqual,
                            const services::Buffer<DataType> &buffer,
                            size_t nColumns, size_t nRows, services::Status &st) :
        SyclHomogenNumericTable(featuresEqual, nColumns, nRows, st)
    {
        if (nColumns * nRows > buffer.size())
        {
            st |= services::Error::create(services::ErrorIncorrectSizeOfArray, services::Row,
                                          "Buffer size is not enough to represent the table");
            services::throwIfPossible(st);
        }

        if (st)
        {
            _buffer = buffer;
            _memStatus = userAllocated;
        }
    }

    SyclHomogenNumericTable(DictionaryIface::FeaturesEqual featuresEqual,
                            size_t nColumns, size_t nRows,
                            NumericTable::AllocationFlag memoryAllocationFlag,
                            services::Status &st) :
        SyclHomogenNumericTable(featuresEqual, nColumns, nRows, st)
    {
        if (memoryAllocationFlag == NumericTableIface::doAllocate)
        {
            st |= allocateDataMemoryImpl();
        }
    }

    SyclHomogenNumericTable(DictionaryIface::FeaturesEqual featuresEqual,
                            size_t nColumns, size_t nRows,
                            NumericTable::AllocationFlag memoryAllocationFlag,
                            const DataType &constValue, services::Status &st) :
        SyclHomogenNumericTable(featuresEqual, nColumns, nRows, memoryAllocationFlag, st)
    {
        st |= assignImpl<DataType>(constValue);
    }

    services::Status allocateDataMemoryImpl(daal::MemType type = daal::dram) DAAL_C11_OVERRIDE
    {
        services::Status status;

        freeDataMemoryImpl();

        // if (isCpuContext())
        // {
        //     status |= allocateDataMemoryOnCpu();
        //     DAAL_CHECK_STATUS_VAR(status);
        // }
        // else
        // {
            if (!getNumberOfColumns())
            {
                status |= services::Status(services::ErrorIncorrectNumberOfFeatures);
                return services::throwIfPossible(status);
            }

            if (getNumberOfRows())
            {
                const size_t size = getNumberOfColumns() * getNumberOfRows();

                const auto universalBuffer = oneapi::internal::getDefaultContext()
                    .allocate(oneapi::internal::TypeIds::id<DataType>(), size, &status);

                services::throwIfPossible(status);
                DAAL_CHECK_STATUS_VAR(status);

                _buffer = universalBuffer.template get<DataType>();
            }
        // }

        _memStatus = internallyAllocated;
        return status;
    }

    void freeDataMemoryImpl() DAAL_C11_OVERRIDE
    {
        _buffer.reset();
        // _cpuTable.reset();
        _memStatus = notAllocated;
    }

    services::Status setNumberOfColumnsImpl(size_t ncol) DAAL_C11_OVERRIDE
    {
        services::Status status;

        // if (isCpuTable())
        // {
        //     status |= _cpuTable->setNumberOfColumns(ncol);
        // }

        if (status && _ddict->getNumberOfFeatures() != ncol)
        {
            _ddict->resetDictionary();
            _ddict->setNumberOfFeatures(ncol);

            NumericTableFeature df;
            df.setType<DataType>();
            _ddict->setAllFeatures(df);
        }

        return status;
    }

    template <typename Archive, bool onDeserialize>
    services::Status serialImpl(Archive * archive)
    {
        NumericTable::serialImpl<Archive, onDeserialize>(archive);

        if (onDeserialize)
        {
            allocateDataMemoryImpl();
        }

        const size_t size = getNumberOfColumns() * getNumberOfRows();
        // if (isCpuTable())
        // {
        //     archive->set(_cpuTable->getArray(), size);
        // }
        // else
        // {
            archive->set(_buffer.toHost(data_management::readOnly).get(), size);
        // }

        return services::Status();
    }

    template <typename T>
    services::Status assignImpl(T value)
    {
        services::Status status;

        if (_memStatus == notAllocated)
        {
            status |= services::Status(services::ErrorEmptyHomogenNumericTable);
            services::throwIfPossible(status);
            return status;
        }

        // if (isCpuTable())
        // {
        //     return _cpuTable->assign(value);
        // }

        oneapi::internal::getDefaultContext().fill(_buffer, (double)value, &status);
        services::throwIfPossible(status);

        return status;
    }

private:
    template <typename T, typename U>
    struct BufferIO
    {
        static services::Status read(const services::Buffer<U> & buffer, BlockDescriptor<T> & block, size_t nRows, size_t nCols)
        {
            DAAL_ASSERT(buffer.size() == nRows * nCols);

            if (!block.resizeBuffer(nCols, nRows))
            {
                services::Status status(services::ErrorMemoryAllocationFailed);
                services::throwIfPossible(status);
                return status;
            }

            // TODO: Figure out how to convert the data without fallback to host
            auto hostPtr = buffer.toHost(data_management::readOnly);
            internal::VectorUpCast<U, T>()(nRows * nCols, hostPtr.get(), block.getBlockPtr());

            return services::Status();
        }

        static services::Status write(services::Buffer<U> buffer, const BlockDescriptor<T> & block, size_t nRows, size_t nCols)
        {
            DAAL_ASSERT(block.getNumberOfRows() == nRows);
            DAAL_ASSERT(block.getNumberOfColumns() == nCols);
            DAAL_ASSERT(buffer.size() == nRows * nCols);

            // TODO: Figure out how to convert the data without fallback to host
            auto hostPtr = buffer.toHost(data_management::writeOnly);
            internal::VectorDownCast<T, U>()(nRows * nCols, block.getBlockPtr(), hostPtr.get());

            return services::Status();
        }
    };

    template <typename T>
    struct BufferIO<T, T>
    {
        static services::Status read(const services::Buffer<T> & buffer, BlockDescriptor<T> & block, size_t nRows, size_t nCols)
        {
            DAAL_ASSERT(buffer.size() == nRows * nCols);

            block.setBuffer(buffer, nCols, nRows);
            return services::Status();
        }

        static services::Status write(services::Buffer<T> buffer, const BlockDescriptor<T> & block, size_t nRows, size_t nCols)
        {
            // TODO: Support case when block.getBuffer() != buffer
            // It means that user calls block.setBuffer on their side
            return services::Status();
        }
    };

    services::Buffer<DataType> getSubBuffer(size_t rowOffset, size_t nRows)
    {
        const size_t nCols  = getNumberOfColumns();
        const size_t offset = rowOffset * nCols;
        const size_t size   = nRows * nCols;
        if (size == _buffer.size())
        {
            return _buffer;
        }
        return _buffer.getSubBuffer(offset, size);
    }

    template <typename T>
    services::Status getTBlock(size_t rowOffset, size_t nRowsBlockDesired, ReadWriteMode rwFlag, BlockDescriptor<T> & block)
    {
        // if (isCpuTable())
        // {
        //     return _cpuTable->getBlockOfRows(rowOffset, nRowsBlockDesired, rwFlag, block);
        // }

        const size_t nRows = getNumberOfRows();
        const size_t nCols = getNumberOfColumns();
        block.setDetails(0, rowOffset, rwFlag);

        if (rowOffset >= nRows)
        {
            block.reset();
            return services::Status();
        }

        const size_t nRowsBlock = (rowOffset + nRowsBlockDesired < nRows) ? nRowsBlockDesired : nRows - rowOffset;

        return BufferIO<T, DataType>::read(getSubBuffer(rowOffset, nRowsBlock), block, nRowsBlock, nCols);
    }

    template <typename T>
    services::Status releaseTBlock(BlockDescriptor<T> & block)
    {
        // if (isCpuTable())
        // {
        //     return _cpuTable->releaseBlockOfRows(block);
        // }

        services::Status status;

        if (block.getRWFlag() & (int)writeOnly)
        {
            const size_t nCols     = getNumberOfColumns();
            const size_t nRows     = block.getNumberOfRows();
            const size_t rowOffset = block.getRowsOffset();

            status |= BufferIO<T, DataType>::write(getSubBuffer(rowOffset, nRows), block, nRows, nCols);
        }

        block.reset();
        return status;
    }

    template <typename T>
    services::Status getTFeature(size_t columnIndex, size_t rowOffset, size_t nRowsBlockDesired, ReadWriteMode rwFlag, BlockDescriptor<T> & block)
    {
        // if (isCpuTable())
        // {
        //     return _cpuTable->getBlockOfColumnValues(columnIndex, rowOffset, nRowsBlockDesired, rwFlag, block);
        // }

        services::throwIfPossible(services::ErrorMethodNotImplemented);
        return services::ErrorMethodNotImplemented;
    }

    template <typename T>
    services::Status releaseTFeature(BlockDescriptor<T> & block)
    {
        // if (isCpuTable())
        // {
        //     return _cpuTable->releaseBlockOfColumnValues(block);
        // }

        services::throwIfPossible(services::ErrorMethodNotImplemented);
        return services::ErrorMethodNotImplemented;
    }

    // services::Status allocateDataMemoryOnCpu()
    // {
    //     services::Status status;

    //     _cpuTable = HomogenNumericTable<DataType>::create(getNumberOfColumns(), getNumberOfRows(), NumericTableIface::doAllocate, &status);

    //     return status;
    // }

    // inline bool isCpuTable() const { return (bool)_cpuTable; }

    // static bool isCpuContext() { return oneapi::internal::getDefaultContext().getInfoDevice().isCpu; }

    services::Buffer<DataType> _buffer;
    // services::SharedPtr<HomogenNumericTable<DataType> > _cpuTable;
};
/** @} */

} // namespace interface1

using interface1::SyclHomogenNumericTable;

} // namespace data_management
} // namespace daal

#endif
