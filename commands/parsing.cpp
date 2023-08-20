#include "Commands.hpp"

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
            return "";
        }
    }

    if (!hasSecondChar)
    {
        return "";
    }

    return std::string(1, firstChar) + secondChar;
}

std::vector<std::string> topicParsing(const std::string &input)
{
    std::vector<std::string> result;

    size_t start = input.find('"');
    if (start == std::string::npos)
        return result;

    size_t end = input.find('"', start + 1);
    if (end == std::string::npos)
        return result;

    std::string channelName = input.substr(0, start);
    std::string topic = input.substr(start + 1, end - start - 1);

    std::cout << "{{{{{{{{{{{{{{{{{{{}}}}}}}}}}}}}}}}}}}" << input << std::endl;
    std::cout << "channelName: " << channelName << std::endl;
    std::cout << "topic: " << topic << std::endl;
    std::cout << "{{{{{{{{{{{{{{{{{{{}}}}}}}}}}}}}}}}}}}" << input << std::endl;

    size_t additionalArgs = input.find_first_not_of(" \t", end + 1);
    if (additionalArgs != std::string::npos)
        return result;

    result.push_back(channelName);
    result.push_back(topic);

    return result;
}