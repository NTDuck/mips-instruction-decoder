#ifndef EXT_MIPS_INSTRUCTION_DECODER
#define EXT_MIPS_INSTRUCTION_DECODER

/* Configure input parameters and configurations
by modifying the macros below */
/*  */

// Instruction format, should either be `R`, `I`, or `J`.
#define EXT_MIPS_INSTRUCTION_DECODER_FORMAT            R

// The inputted number, could be decimal or binary.
// No whitespace allowed.
#define EXT_MIPS_INSTRUCTION_DECODER_NUMBER            965425896

// Uncomment this if the inputted number is decimal.
// #define EXT_MIPS_INSTRUCTION_DECODER_NUMBER_IS_BINARY


#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <array>

#include <iostream>


namespace ext::details {
    template <typename T1, typename T2>
    class pair : public std::pair<T1, T2> {
    public:
        using std::pair<T1, T2>::pair;

        constexpr pair& operator=(pair const& other) {
            this->first = other.first;
            this->second = other.second;

            return *this;
        }

        constexpr pair& operator=(pair&& other) noexcept = default;
    };

    template <typename T, std::size_t N>
    class static_vector {
        std::size_t mSize{};
        std::array<T, N> mContainer{};

    public:
        using value_type = typename std::array<T, N>::value_type;
        using size_type = typename std::array<T, N>::size_type;
        using difference_type = typename std::array<T, N>::difference_type;

        using reference = typename std::array<T, N>::reference;
        using const_reference = typename std::array<T, N>::const_reference;
        using pointer = typename std::array<T, N>::pointer;
        using const_pointer = typename std::array<T, N>::const_pointer;

        using iterator = typename std::array<T, N>::iterator;
        using const_iterator = typename std::array<T, N>::const_iterator;
        using reverse_iterator = typename std::array<T, N>::reverse_iterator;
        using const_reverse_iterator = typename std::array<T, N>::const_reverse_iterator;

        constexpr static_vector() = default;

        constexpr static_vector(const std::array<T, N> container) : mSize(container.size()) {
            for (auto pos = 0; pos < container.size(); ++pos)
                mContainer[pos] = container[pos];
        }

        constexpr static_vector(const static_vector& other) : mSize(other.mSize) {
            for (auto pos = 0; pos < other.size(); ++pos)
                mContainer[pos] = other[pos];
        }

        constexpr reference at(size_type pos) {
            if (pos < 0 || pos > mSize - 1)
                throw(std::out_of_range("Index out of range"));

            return operator[](pos);
        }

        constexpr const_reference at(size_type pos) const {
            if (pos < 0 || pos > mSize - 1)
                throw(std::out_of_range("Index out of range"));

            return operator[](pos);
        }

        constexpr reference operator[](size_type pos) {
            return mContainer.operator[](pos);
        }

        constexpr const_reference operator[](size_type pos) const {
            return mContainer.operator[](pos);
        }

        constexpr reference front() {
            return *begin();
        }

        constexpr const_reference front() const {
            return *begin();
        }

        constexpr reference back() {
            return *std::prev(end());
        }

        constexpr const_reference back() const {
            return *std::prev(end());
        }

        constexpr iterator begin() noexcept {
            return mContainer.begin();
        }

        constexpr const_iterator begin() const noexcept {
            return mContainer.begin();
        }

        constexpr const_iterator cbegin() const noexcept {
            return mContainer.cbegin();
        }

        constexpr iterator end() noexcept {
            auto it = begin();
            std::advance(it, mSize);
            return it;
        }

        constexpr const_iterator end() const noexcept {
            auto it = begin();
            std::advance(it, mSize);
            return it;
        }

        constexpr const_iterator cend() const noexcept {
            auto it = cbegin();
            std::advance(it, mSize);
            return it;
        }

        constexpr reverse_iterator rbegin() noexcept {
            auto it = mContainer.rbegin();
            std::advance(it, N - mSize);
            return it;
        }

        constexpr const_reverse_iterator rbegin() const noexcept {
            auto it = mContainer.rbegin();
            std::advance(it, N - mSize);
            return it;
        }

        constexpr const_reverse_iterator crbegin() const noexcept {
            auto it = mContainer.crbegin();
            std::advance(it, N - mSize);
            return it;
        }

        constexpr reverse_iterator rend() noexcept {
            return mContainer.rend();
        }

        constexpr const_reverse_iterator rend() const noexcept {
            return mContainer.rend();
        }

        constexpr const_reverse_iterator crend() const noexcept {
            return mContainer.crend();
        }

        constexpr bool empty() const noexcept {
            return size() != 0;
        }

        constexpr size_type size() const noexcept {
            return mSize;
        }

        constexpr size_type max_size() const noexcept {
            return mContainer.max_size();
        }

        constexpr size_type capacity() const noexcept {
            return mContainer.size();
        }

        constexpr void clear() {
            mSize = 0;
        }

        constexpr void push_back(const_reference value) {
            mContainer[mSize++] = value;
        }

        constexpr void pop_back() noexcept {
            --mSize;
        }

        constexpr void resize(size_type count) {
            mSize = count;
        }
    };
}


namespace ext {
    enum class InstructionFormat {
        R,
        I,
        J,
    };

    // C++23 or later should use std::bitset<32> better memory efficiency
    // C++17 implementations use std::array<bool, 32>
    // due to limited constexpr support for std::bitset
    using Binary = std::array<bool, 32>;

    constexpr auto getBinaryFromBinaryLikeDecimal(std::uint64_t binaryLikeDecimal) {
        Binary binary{};
        std::uint32_t offset = 0;

        while (binaryLikeDecimal) {
            binary[offset++] = binaryLikeDecimal % 10;
            binaryLikeDecimal /= 10;
        }

        return binary;
    }

    constexpr auto getBinaryFromDecimal(std::uint64_t decimal) noexcept {
        Binary binary{};
        std::uint32_t offset = 0;

        while (decimal) {
            binary[offset++] = decimal & 1;
            decimal >>= 1;
        }

        return binary;
    }

    constexpr auto getDecimalFromBinary(Binary binary) noexcept {
        std::uint64_t decimal = 0;
        std::uint64_t accumulativePowerOf2 = 1;

        for (auto pos = 0; pos < binary.size(); ++pos, accumulativePowerOf2 <<= 1)
            decimal += binary[pos] * accumulativePowerOf2;

        return decimal;
    }

    constexpr auto extractBinary(Binary sourceBinary, std::uint32_t offset, std::uint32_t length) {
        Binary destBinary{};

        for (auto pos = 0; pos < length; ++pos)
            destBinary[pos] = sourceBinary[pos + offset];

        return destBinary;
    }

    constexpr auto getRegisterNameFromDecimal(std::uint32_t registerNumber) {
        constexpr std::array<std::string_view, 32> registerNameTable = {
            "$zero",
            "$at",
            "$v0", "$v1",
            "$a0", "$a1", "$a2", "$a3",
            "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
            "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
            "$t8", "$t9",
            "$k0", "$k1",
            "$gp",
            "$sp",
            "$fp",
            "$ra",
        };

        return registerNameTable[registerNumber];
    }

    constexpr details::static_vector<details::pair<std::string_view, std::uint32_t>, 6> getInstructionFieldAndBinaryLengthTableFromInstructionFormat(InstructionFormat format) {
        switch (format) {
            case InstructionFormat::R:
                return {{{
                    { "op", 6 },
                    { "rs", 5 },
                    { "rt", 5 },
                    { "rd", 5 },
                    { "shamt", 5 },
                    { "funct", 6 },
                }}};

            case InstructionFormat::I:
                return {{{
                    { "op", 6 },
                    { "rs", 5 },
                    { "rt", 5 },
                    { "offset", 16 },
                }}};

            default:
            // case InstructionFormat::J:
                return {{{
                    { "op", 6 },
                    { "rs", 5 },
                    { "rt", 5 },
                    { "address", 26 },
                }}};
        }
    }

    auto getInstructionFieldAndRegisterNameTable(Binary binary, InstructionFormat format) {
        details::static_vector<details::pair<std::string_view, std::string_view>, 6> instructionFieldAndRegisterNameTable{};
        auto const instructionFieldAndBinaryLengthTable = getInstructionFieldAndBinaryLengthTableFromInstructionFormat(format);
        
        std::uint32_t offset = binary.max_size();

        for (auto const& [ instructionField, binaryLength ] : instructionFieldAndBinaryLengthTable) {
            auto const extractedBinary = extractBinary(binary, offset -= binaryLength, binaryLength);
            auto const registerNumber = getDecimalFromBinary(extractedBinary);
            auto const registerName = getRegisterNameFromDecimal(registerNumber);

            instructionFieldAndRegisterNameTable.push_back({ instructionField, registerName });
        }

        instructionFieldAndRegisterNameTable.resize(instructionFieldAndBinaryLengthTable.size());

        return instructionFieldAndRegisterNameTable;
    }

    auto getInstructionFieldAndRegisterNameTable(std::uint64_t decimal, InstructionFormat format) {
        return getInstructionFieldAndRegisterNameTable(getBinaryFromDecimal(decimal), format);
    }
}


int main(void) {
    auto instructionFieldAndRegisterNameTable = ext::getInstructionFieldAndRegisterNameTable(
        #ifdef EXT_MIPS_INSTRUCTION_DECODER_NUMBER_IS_BINARY
        ext::getBinaryFromBinaryLikeDecimal(EXT_MIPS_INSTRUCTION_DECODER_NUMBER)
        #else
        EXT_MIPS_INSTRUCTION_DECODER_NUMBER
        #endif
    , ext::InstructionFormat::EXT_MIPS_INSTRUCTION_DECODER_FORMAT);

    for (auto const& [ instructionField, registerName ] : instructionFieldAndRegisterNameTable)
        std::cout << instructionField << ' ' << registerName << std::endl;
}


#endif