#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <unordered_map>

std::vector<std::string> loadWordList(const std::string& filename) {
    std::vector<std::string> wordList;
    std::ifstream file(filename);
    std::string word;

    if (file.is_open()) {
        while (getline(file, word)) {
            std::string cleanedString;
            for (char ch : word) {
                if (std::isalpha(ch)) {
                    cleanedString += ch;
                }
            }
            wordList.push_back(cleanedString);
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << "\n";
    }

    return wordList;
}

std::string generatePassword(const std::vector<std::string>& wordList, int numWords, int numCaps, int numNumbers, int numSymbols) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> wordDistribution(0, wordList.size() - 1);
    std::uniform_int_distribution<int> numberDistribution(0, 9);
    // Define the symbols
    std::string symbols = "!@#$%^&*()-_=+[{]}\\|;:'\",<.>/?";
    std::uniform_int_distribution<int> symbolDistribution(0, symbols.size());

    std::vector<std::string> password;
    
    for (int i = 0; i < numWords; ++i) {
        int randomIndex = wordDistribution(generator);
        password.push_back(wordList[randomIndex]);
    }
    std::uniform_int_distribution<int> indexDistribution(0, password.size() - 1);

    for (int i = 0; i < numCaps; ++i) {
        int randomIndex = indexDistribution(generator);
        password[randomIndex][0] = toupper(password[randomIndex][0]);
    }

    indexDistribution = std::uniform_int_distribution<int>(0, password.size());
    for (int i = 0; i < numNumbers; ++i) {
        int randomIndex = indexDistribution(generator);
        password.insert(password.begin() + randomIndex, std::to_string(numberDistribution(generator)));
    }

    indexDistribution = std::uniform_int_distribution<int>(0, password.size());
    for (int i = 0; i < numSymbols; ++i) {
        char randomSymbol = symbols[symbolDistribution(generator)];
        int randomIndex = indexDistribution(generator);
        password.insert(password.begin() + randomIndex, std::string(1,randomSymbol));
    }

    std::string result;

    for (std::string str : password) {
        result += str;
    }
    return result;
}

int main(int argc, char* argv[]) {
    std::srand(std::time(0));
    int words = 4;
    int caps = 0;
    int numbers = 0;
    int symbols = 0;

    // Map to store the argument options and their corresponding values
    std::unordered_map<std::string, int*> optionsMap{
            {"-w", &words},
            {"--words", &words},
            {"-c", &caps},
            {"--caps", &caps},
            {"-n", &numbers},
            {"--numbers", &numbers},
            {"-s", &symbols},
            {"--symbols", &symbols}
    };

    // Parse the command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);

        if (optionsMap.count(arg)) {
            if (i + 1 < argc) {
                std::istringstream ss(argv[++i]);
                ss >> *(optionsMap[arg]);
                if (ss.fail() || *(optionsMap[arg]) < 0) {
                    std::cerr << "Invalid argument for " << arg << ". Expected a non-negative integer.\n";
                    return 1;
                }
            } else {
                std::cerr << "Missing argument for " << arg << ".\n";
                return 1;
            }
        } else if (arg == "-h" || arg == "--help") {
            std::cout << "usage: xkcdpwgen [-h] [-w WORDS] [-c CAPS] [-n NUMBERS] [-s SYMBOLS]\n"
                      << "\n"
                      << "Generate a secure, memorable password using the XKCD method\n"
                      << "\n"
                      << "optional arguments:\n"
                      << "  -h, --help            show this help message and exit\n"
                      << "  -w WORDS, --words WORDS\n"
                      << "                        include WORDS words in the password (default=4)\n"
                      << "  -c CAPS, --caps CAPS  capitalize the first letter of CAPS random words\n"
                      << "                        (default=0)\n"
                      << "  -n NUMBERS, --numbers NUMBERS\n"
                      << "                        insert NUMBERS random numbers in the password\n"
                      << "                        (default=0)\n"
                      << "  -s SYMBOLS, --symbols SYMBOLS\n"
                      << "                        insert SYMBOLS random symbols in the password\n"
                      << "                        (default=0)\n";
            return 0;
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            return 1;
        }
    }

    if (words < caps) {
        std::cerr << "Invalid arguments. More caps/numbers/symbols requested than words.\n";
        return 1;
    }

    // Load the word list from file
    std::vector<std::string> wordList = loadWordList("words.txt");
    if (wordList.empty()) {
        return 1;
    }

    // Generate the password
    std::string password = generatePassword(wordList, words, caps, numbers, symbols);
    
    std::cout << "Generated Password: " << password << "\n";

    return 0;
}
