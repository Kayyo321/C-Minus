#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Lexer.h"

/* https://www.delftstack.com/howto/cpp/read-file-into-string-cpp/ */
std::string ReadFile(const std::string &path)
{
    std::ifstream input_file(path);

    if (!input_file.is_open())
    {
        std::cerr << "Could not open the file - '"
                  << path << "'\n";

        exit(1);
    }

    return std::string {
            (std::istreambuf_iterator<char>(input_file)),
            std::istreambuf_iterator<char>()
    };
}

int main()
{
    std::cout << "C- 0.1!\n\n";

    try
    {
        std::string path {};
        std::cout << "C- Path> ";
        std::getline(std::cin, path);

        std::string contents {ReadFile(path)};

        /* File */

        std::cout
                << "\n\n--== File ==--\n\n"
                << contents
                << "\n\n";

        /* Lexer */

        std::cout << "\n\n--== Lexer ==--\n\n";

        Lexer *lexer {new Lexer};

        std::vector<Token> tokens {lexer->Parse(contents)};

        for (const Token &token: tokens)
            token.Info();

        delete lexer;

    }
    catch (const UnknownEscapeSequence &e)
    {
        std::cerr << e.What() << "\n";

        return 1;
    }

    return 0;
}