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
        auto appendToStream = [&oss](std::string_view str) {
            if (!str.empty())
            {
                oss << str << "\n";
            }
        };

        appendToStream(GeneratorImpl::Prologue());
        for (const auto &strutPair : structs)
        {
            const auto &structName = strutPair.first;
            appendToStream(GeneratorImpl::StructStart(structName));
            for (size_t i = 0; i < strutPair.second.size(); ++i)
            {
                const auto &member = strutPair.second[i];
                bool isLast = i == (strutPair.second.size() - 1);
                appendToStream(GeneratorImpl::Member(structName, member, isLast));
            }
            appendToStream(GeneratorImpl::StructEnd(structName));
        }
        appendToStream(GeneratorImpl::Epilogue());
    }
};

} // namespace luarch::pods_serdes
