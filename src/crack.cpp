#include <iostream>//std::cout
#include <fstream>//ofstream
#include <math.h>//pow
#include <ctime>//std::time
#include "sha256.cpp"

std::ifstream input;
std::string username;
std::string salt;
std::string hash;
std::string searchCharacters;
std::string lowerCaseCharacters = "abcdefghijklmnopqrstuvwxyz";
std::string upperCaseCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
std::string numericCharacters = "0123456789";
std::string specialCharacters = "!@#$%^&*()-_=+[{}]:;,<.>/?";
bool useSalt = false;
int minimumCharacters;
int maximumCharacters;
bool cracked = false;

void parseInput(std::string line)
{
    bool foundUsername;
    bool foundSalt;
    bool foundHash;

    for(int z = 0;z < line.length();z++)
    {
        if(!foundUsername)
        {
            if(line[z] == '[')
            {
                continue;
            }

            if(line[z] == ',')
            {
                foundUsername = true;
                continue;
            }

            username += line[z];
            continue;
        }

        if(!foundSalt)
        {
            if(line[z] == ' ')
            {
                continue;
            }

            if(line[z] == ',')
            {
                foundSalt = true;
                continue;
            }

            salt += line[z];
            continue;
        }

        if(!foundHash)
        {
            if(line[z] == ' ')
            {
                continue;
            }

            if(line[z] == ']')
            {
                foundHash = true;
                break;
            }

            hash += line[z];
            continue;
        }
    }
}

void crack()
{
    int * characters;

    int low = (minimumCharacters != -1) ? minimumCharacters : 1;
    int high = (maximumCharacters != -1) ? maximumCharacters : 16;
    int searchCharactersLength = searchCharacters.length();

    int totalIterations = 0;
    std::time_t startTime = std::time(0);
    std::time_t endTime;

    //size of search string
    for(int size = low;size < high;size++)
    {
        std::cout << "searching size: " << size <<std::endl;

        characters = new int[size];

        //init characters array
        for(int key = 0;key < size;key++)
        {
            characters[key] = 0;
        }

        //outputing less than needed
        int iterations = pow(searchCharactersLength, size);

        std::cout << "iterations required: " << iterations << std::endl;

        for(int key = 0;key < iterations;key++)
        {
            totalIterations++;

            std::string searchString;

            for(int subKey = 0;subKey < size;subKey++)
            {
                searchString += searchCharacters[characters[subKey]];
            }

            if(useSalt)
            {
                searchString += salt;
            }

            if(sha256(searchString) == hash)
            {
                endTime = std::time(0);
                std::cout << "FOUND" << std::endl;
                std::cout << "password + hash: " << searchString << std::endl;
                goto end;
            }

            //increment searchString
            for(int subKey = 0;subKey < size;subKey++)
            {
                if(characters[subKey] == searchCharactersLength - 1)
                {
                    characters[subKey] = 0;
                }
                else
                {
                    characters[subKey] += 1;
                    break;
                }
            }
        }
    }

    end:
        std::cout << "Total iterations: " << totalIterations << std::endl;
        std::cout << "Seconds required: " << (endTime - startTime) << std::endl;
}

int main()
{
    char lowerCaseCharactersAnswer;
    std::cout << "Should the system check for password + salt containing lower case characters? (y/n)" << std::endl;
    std::cin >> lowerCaseCharactersAnswer;

    char upperCaseCharactersAnswer;
    std::cout << "Should the system check for password + salt containing upper case characters? (y/n)" << std::endl;
    std::cin >> upperCaseCharactersAnswer;

    char numericCharactersAnswer;
    std::cout << "Should the system check for password + salt containing numeric characters? (y/n)" << std::endl;
    std::cin >> numericCharactersAnswer;

    char specialCharactersAnswer;
    std::cout << "Should the system check for password + salt containing special characters? (y/n)" << std::endl;
    std::cin >> specialCharactersAnswer;

    int minimumCharactersAnswer;
    std::cout << "Does the password require a specific minimum number of characters?" << std::endl;
    std::cout << "Yes: [input minimum number]" << std::endl;
    std::cout << "No: -1 for default value of 1" << std::endl;
    std::cin >> minimumCharactersAnswer;

    int maximumCharactersAnswer;
    std::cout << "Does the password require a specific minimum maximum of characters?" << std::endl;
    std::cout << "Yes: [input minimum maximum]" << std::endl;
    std::cout << "No: -1 for default value of 16" << std::endl;
    std::cin >> maximumCharactersAnswer;

    char knownSaltAnswer;
    std::cout << "Should the system use the stored salt? (y/n)" << std::endl;
    std::cin >> knownSaltAnswer;

    if(lowerCaseCharactersAnswer == 'y')
    {
        searchCharacters += lowerCaseCharacters;
    }

    if(upperCaseCharactersAnswer == 'y')
    {
        searchCharacters += upperCaseCharacters;
    }

    if(numericCharactersAnswer == 'y')
    {
        searchCharacters += numericCharacters;
    }

    if(specialCharactersAnswer == 'y')
    {
        searchCharacters += specialCharacters;
    }

    if(knownSaltAnswer == 'y')
    {
        useSalt = true;
    }

    minimumCharacters = minimumCharactersAnswer;
    maximumCharacters = maximumCharactersAnswer;

    std::string line;
    input.open("./pwd.txt");
    getline(input, line);
    parseInput(line);
    input.close();

    crack();
}
