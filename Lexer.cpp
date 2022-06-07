#include "Lexer.h"

Lexer::Lexer()
{
	typeMap["int8"] = INT8;
	typeMap["char"] = INT8;
	typeMap["u_int8"] = U_INT8;
	typeMap["int32"] = INT32;
	typeMap["u_int32"] = U_INT32;
	typeMap["double"] = DOUBLE_LITERAL;
	typeMap["void"] = VOID;
	typeMap["struct"] = STRUCT;
}

std::vector<Token> Lexer::Parse(const std::string &program)
{
	bool shouldBeType {false};

	curToken.lineNumber = 1;
	curToken.charIndex = 1;

	for (const char &c: program)
	{
		++curToken.charIndex;

		if (curToken.type == STRING_ESCAPE)
		{
			switch (c)
			{
				case 'n':
					curToken.text.append(1, '\n');
					break;

				case 'r':
					curToken.text.append(1, '\r');
					break;

				case 't':
					curToken.text.append(1, '\t');
					break;

				case '\\':
					curToken.text.append(1, '\\');
					break;

				default:
					throw UnknownEscapeSequence(
						std::string("Unknown escape sequence: '\\")
						+ c
						+ std::string("', on line: '")
						+ std::to_string(curToken.lineNumber)
						+ std::string(", ")
						+ std::to_string(curToken.charIndex)
						+ std::string("'.")
					);
					break;
			}

			curToken.type = STRING;

			continue;
		}
		else if (curToken.type == COMMENT_CONFUSED && c != '/') 
		{
			curToken.type = OPERATOR;

			CloseToken();

			continue;
		}

		if (std::isdigit(c))
		{
			if (curToken.type == WHITESPACE)
			{
				curToken.type = INT;
				curToken.text += c;
			}
			else if (curToken.type == DOUBLE_CONFUSED)
			{
				curToken.type = DOUBLE;
				curToken.text += c;
			}
			else
			{
				curToken.text += c;
			}
			continue;
		}

		switch (c)
		{
			case ' ' :
			case '\t':
				if (curToken.type == STRING
					|| curToken.type == COMMENT)
				{
					curToken.text += c;
				}
				else
				{
					if (shouldBeType)
					{
						size_t i{ 0 };
						bool foundType{ false };

						for (const auto& a : typeMap)
						{
							if (curToken.text == a.first)
							{
								foundType = true;
							}
						}

						if (foundType)
							curToken.type = TYPE;
						else
							curToken.type = UNKNOWN_TYPE;

						shouldBeType = false;

						CloseToken();
					}

					CloseToken();
				}
				break;

			case '\n':
			case '\r':
				if (curToken.type == STRING)
				{
					curToken.text += c;
				}
				else
				{
					CloseToken();

					++curToken.lineNumber;
					curToken.charIndex = 1;
				}
				break;

			case '.':
				if (curToken.type == WHITESPACE)
				{
					curToken.type = DOUBLE_CONFUSED;
					curToken.text += c;
				}
				else if (curToken.type == STRING
					|| curToken.type == COMMENT)
				{
					curToken.text += c;
				}
				else if (curToken.type == INT)
				{
					curToken.type = DOUBLE;
					curToken.text += c;
				}
				else
				{
					CloseToken();

					curToken.type = OPERATOR;
					curToken.text += c;

					CloseToken();
				}
				break;
			
			case '/':
				if (curToken.type == STRING
					|| curToken.type == COMMENT)
				{
					curToken.text += c;
				}
				else if (curToken.type == COMMENT_CONFUSED) 
				{
					curToken.type = COMMENT;
					curToken.text.erase();
				}
				else 
				{
					CloseToken();

					curToken.type = COMMENT_CONFUSED;
					curToken.text += c;
				}
				break;

			case ':':
				if (curToken.type == STRING
					|| curToken.type == COMMENT)
				{
					curToken.text += c;
				}
				else if (curToken.type == OPERATOR
					&& c == ':')
				{
					curToken.type = TYPE_SPECIFIER;
					curToken.text += c;

					CloseToken();

					shouldBeType = true;
				}
				else
				{
					CloseToken();

					curToken.type = OPERATOR;
					curToken.text += c;
				}
				break;

			case '{':
			case '}':
			case '(':
			case ')':
			case '+':
			case '-':
			case '*':
			case '%':
			case '<':
			case '>':
			case ';':		
			case ',':
			case '!':
				if (curToken.type == STRING
					|| curToken.type == COMMENT)
				{
					curToken.text += c;
				}
				else
				{
					CloseToken();

					curToken.type = OPERATOR;
					curToken.text += c;

					CloseToken();
				}
				break;

			case '"':
				if (curToken.type == STRING)
				{
					CloseToken();
				}
				else
				{
					CloseToken();
					curToken.type = STRING;
				}
				break;

			case '\'':
				if (curToken.type == STRING
					|| curToken.type == COMMENT)
				{
					curToken.text += c;
				}
				else if (curToken.type == CHAR)
				{
					CloseToken();
				}
				else
				{
					CloseToken();
					curToken.type = CHAR;
				}
				break;

			case '\\':
				if (curToken.type == STRING)
				{
					curToken.type = STRING_ESCAPE;
				}
				else if (curToken.type == COMMENT)
				{
					curToken.text += c;
				}
				else
				{
					CloseToken();

					curToken.type = OPERATOR;
					curToken.text += c;

					CloseToken();
				}
				break;

			default:
				if (curToken.type == WHITESPACE
					|| curToken.type == INT
					|| curToken.type == DOUBLE)
				{
					CloseToken();

					curToken.type = IDENTIFIER;
					curToken.text += c;
				}
				else
				{
					curToken.text += c;
				}
				break;
		}
	}

	return tokens;
}

void Lexer::CloseToken()
{
	if (curToken.type == COMMENT) {}
	else if (curToken.type != WHITESPACE)
	{
		tokens.push_back(curToken);
	}

	if (curToken.type == DOUBLE_CONFUSED)
	{
		if (curToken.text.compare(".") == 0)
			curToken.type = OPERATOR;
		else
			curToken.type = DOUBLE;
	}

	curToken.type = WHITESPACE;
	curToken.text.erase();
}

void Token::Info() const
{
	std::cout << "[";

	if (type == TYPE)
	{
		std::cout << TypeStrings[typeMap[text]];
	}
	else if (type == UNKNOWN_TYPE)
	{
		std::cout << "UNKNOWN_TYPE";
	}
	else
	{
		std::cout << TokenTypeStrings[type];
	}

	std::cout
		<< ", \""
		<< text
		<< "\", '"
		<< lineNumber
		<< ", "
		<< charIndex
		<< "']\n";
}