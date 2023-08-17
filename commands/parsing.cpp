#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>

std::vector<std::string> splitCommand(const std::string &cmd)
{
    std::vector<std::string> result;
    std::istringstream iss(cmd);
    std::string token;

    while (std::getline(iss, token, ' '))
    {
        result.push_back(token);
    }

    return result;
}

std::string parseModeOptions(const std::string &input)
{
    char firstChar = '-';
    char secondChar = ' ';
    bool hasSecondChar = false;

    for (std::string::size_type i = 0; i < input.size(); ++i)
    {
        char c = input[i];
        if (c == '+' || c == '-')
        {
            firstChar = c;
            hasSecondChar = false;
        }
        else if (c == 't' || c == 'l' || c == 'o' || c == 'k' || c == 'i')
        {
            if (hasSecondChar)
            {
                return "";
            }
            secondChar = c;
            hasSecondChar = true;
        }
        else
        {
            // If any other character is encountered, it's an error
            return "";
        }
    }

    if (!hasSecondChar)
    {
        return "";
    }

    return std::string(1, firstChar) + secondChar;
}
