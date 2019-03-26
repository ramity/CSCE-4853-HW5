#include <iostream>//std::cout
#include <fstream>//ofstream
#include <time.h>//time
#include <stdlib.h>//srand, rand
#include "sha256.cpp"

std::ofstream output;
std::string username;
std::string password;
std::string lowerCaseCharacters = "abcdefghijklmnopqrstuvwxyz";
std::string upperCaseCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
std::string numericCharacters = "0123456789";
std::string specialCharacters = "!@#$%^&*()-_=+[{}]:;,<.>/?";
std::string salt;
std::string hash;
std::string saltCharacters;

std::string generateSalt(int bytes)
{
    std::string output(bytes, ' ');

    for(int z = 0;z < bytes;z++)
    {
        output[z] = saltCharacters[rand() % saltCharacters.length()];
    }

    return output;
}

int main()
{
    srand(time(NULL));

    std::cout << "Please enter your username" << std::endl;
    std::getline(std::cin, username);

    std::cout << "Please enter your password" << std::endl;
    std::getline(std::cin, password);

    char lowerCaseCharactersAnswer;
    std::cout << "Should the salt contain lower case characters? (y/n)" << std::endl;
    std::cin >> lowerCaseCharactersAnswer;

    char upperCaseCharactersAnswer;
    std::cout << "Should the salt contain upper case characters? (y/n)" << std::endl;
    std::cin >> upperCaseCharactersAnswer;

    char numericCharactersAnswer;
    std::cout << "Should the salt contain numeric characters? (y/n)" << std::endl;
    std::cin >> numericCharactersAnswer;

    char specialCharactersAnswer;
    std::cout << "Should the salt contain special characters? (y/n)" << std::endl;
    std::cin >> specialCharactersAnswer;

    if(lowerCaseCharactersAnswer == 'y')
    {
        saltCharacters += lowerCaseCharacters;
    }

    if(upperCaseCharactersAnswer == 'y')
    {
        saltCharacters += upperCaseCharacters;
    }

    if(numericCharactersAnswer == 'y')
    {
        saltCharacters += numericCharacters;
    }

    if(specialCharactersAnswer == 'y')
    {
        saltCharacters += specialCharacters;
    }

    salt = generateSalt(8);
    hash = sha256(password + salt);

    output.open("./pwd.txt");
    output << "[" << username << ", " << salt << ", " << hash << "]\n";
    output.close();
}
