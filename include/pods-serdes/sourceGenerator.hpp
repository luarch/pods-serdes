#pragma once

#include <pods-serdes/utilities.hpp>

#include <iostream>

namespace luarch::pods_serdes {

template <typename GeneratorImpl>
struct SourceGenerator
{
    template <class CharT, class Traits = std::char_traits<CharT>>
    static void Generate(const StructsMap &structs, std::basic_ostream<CharT, Traits> &oss)
    {
        oss << GeneratorImpl::Prologue() << "\n";
        for (const auto &strutPair : structs)
        {
            const auto &structName = strutPair.first;
            oss << GeneratorImpl::StructStart(structName) << "\n";
            for (size_t i = 0; i < strutPair.second.size(); ++i)
            {
                const auto &member = strutPair.second[i];
                bool isLast = i == (strutPair.second.size() - 1);
                oss << GeneratorImpl::Member(structName, member, isLast) << "\n";
            }
            oss << GeneratorImpl::StructEnd(structName) << "\n";
        }
        oss << GeneratorImpl::Epilogue() << "\n";
    }
};

} // namespace luarch::pods_serdes
