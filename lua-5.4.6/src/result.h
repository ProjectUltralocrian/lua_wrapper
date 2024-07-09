#pragma once

#include <variant>
#include <string>
#include <concepts>
#include <iostream>

namespace pul
{

    template <typename T>
    concept Printable = requires(T t) {
        {
            std::cout << t
        } -> std::same_as<std::ostream &>;
    };

    template <Printable SuccessType, Printable ErrorType>
    class Result
    {
    public:
        enum ResultVariant
        {
            OkVariant,
            ErrorVariant
        };
        static Result Ok(SuccessType value)
        {
            auto output = Result(std::variant<SuccessType, ErrorType>{std::in_place_index<OkVariant>, value});
            output.m_Ok = true;
            return output;
        }
        static Result Error(ErrorType value)
        {
            auto output = Result(std::variant<SuccessType, ErrorType>{std::in_place_index<ErrorVariant>, value});
            output.m_Ok = false;
            return output;
        }
        inline bool isOk() const
        {
            return m_Ok;
        }
        inline bool isError() const
        {
            return !m_Ok;
        }
        SuccessType okOrDefault(SuccessType defaultValue) const
        {
            return m_Ok ? std::get<OkVariant>(m_Data) : defaultValue;
        }
        SuccessType unwrap(std::string_view error_msg = "Success value was not OK.") const
        {
            if (isError())
            {
                throw std::bad_variant_access();
            }
            return std::get<OkVariant>(m_Data);
        }
        ErrorType getError(std::string_view error_msg = "Success value was not Error.") const
        {
            if (m_Ok)
            {
                throw std::bad_variant_access();
            }
            return std::get<ErrorVariant>(m_Data);
        }
        operator bool() const
        {
            return m_Ok;
        }

    private:
        Result(std::variant<SuccessType, ErrorType> result)
            : m_Data{result} {}
        std::variant<SuccessType, ErrorType> m_Data;
        bool m_Ok{false};
    };

}