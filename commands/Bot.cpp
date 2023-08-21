/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 17:30:27 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/21 18:18:25 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

std::vector<std::string> getInterestingFacts()
{
    std::vector<std::string> facts;
    facts.push_back("The first computer virus was created in 1983 and was called the 'Elk Cloner'.");
    facts.push_back("Honeybees can recognize human faces.");
    facts.push_back("A group of flamingos is called a 'flamboyance'.");
    facts.push_back("Honey never spoils. Archaeologists have found pots of honey in ancient Egyptian tombs that are over 3,000 years old and still perfectly edible.");
    facts.push_back("Octopuses have three hearts. Two pump blood to the gills, and one pumps it to the rest of the body.");
    facts.push_back("The shortest war in history was between Britain and Zanzibar on August 27, 1896. Zanzibar surrendered after 38 minutes.");
    facts.push_back("Cows have best friends and can become stressed when they are separated.");
    facts.push_back("The world's oldest known recipe is for beer. It dates back to around 3,900 BC and was discovered in ancient Mesopotamia.");
    return facts;
};

std::vector<std::string> getJokes()
{
    std::vector<std::string> jokes;
    jokes.push_back("Did you hear about the restaurant on the moon? Great food, no atmosphere.");
    jokes.push_back("Why don't scientists trust atoms? Because they make up everything!");
    jokes.push_back("How do you organize a space party? You planet!");
    jokes.push_back("Why did the scarecrow win an award? Because he was outstanding in his field!");
    jokes.push_back("I told my wife she was drawing her eyebrows too high. She seemed surprised.");
    jokes.push_back("Why don't oysters donate to charity? Because they are shellfish.");
    jokes.push_back("Why couldn't the bicycle stand up by itself? Because it was two-tired!");
    jokes.push_back("Why did the math book look sad? Because it had too many problems.");
    jokes.push_back("Why don't skeletons fight each other? They don't have the guts.");
    return jokes;
};

std::vector<std::string> getSongs()
{
    std::vector<std::string> songs;
    songs.push_back("https://www.youtube.com/watch?v=9QvVi1dHeVw");
    songs.push_back("https://www.youtube.com/watch?v=Sj1GXEtTKso");
    songs.push_back("https://www.youtube.com/watch?v=k2cICPsB-ZM");
    songs.push_back("https://www.youtube.com/watch?v=MqtTeQtLQ1k");
    songs.push_back("https://www.youtube.com/watch?v=EV-h9O4DzAg");

    return songs;
    
};

std::string getRandomNumber()
{
    int randomNumber = rand() % 100;
    return std::to_string(randomNumber);
}

std::string getRandom(const std::vector<std::string> &data)
{
    if (data.empty())
    {
        return "No data available.";
    }
    return data[rand() % data.size()];
}

void commands::Bot(Client &c, Server &s)
{
    std::string botOption = getTextAfterColon(c.getTokens()[1]);
    if (botOption.empty())
    {
        sendMessage("Bot Option is empty\n Usage: BOT :<option> \n Options: \n\t- d for dad jokes \n\t- s for random song \n\t- g for guess \n\t- f to get interesting facts  \r\n", c.getFd());
        return;
    }
    if (s.isNickThere(s, c.getNickname()))
    {
        std::string response;

        switch (botOption[0])
        {
        case 'd':
            response = "Dad Joke: \n" + getRandom(getJokes()) + "\r\n";
            break;
        case 's':
            response = "Song: \n" + getRandom(getSongs()) + "\r\n";
            break;
        case 'g':
            response = "Guess: \n" + getRandomNumber() + "\r\n";
            break;
        case 'f':
            response = "Fact: \n" + getRandom(getInterestingFacts()) + "\r\n";
            break;
        default:
            response = "Invalid Bot Option\n";
            break;
        }
        sendMessage(response, c.getFd());
    }
    else
    {
        sendMessage("You are not on the server\n", c.getFd());
    }
}
