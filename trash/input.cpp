//
// Created by Richard Hodges on 15/01/2018.
//
#include "input.hpp"
#include <iostream>

std::string get_line_with_prompt(std::istream& input_stream, std::string const& prompt)
{
    if (std::addressof(input_stream) == std::addressof(std::cin))
    {
        std::cout << prompt;
    }

    std::string result;
    std::getline(input_stream, result);
    return result;
}