#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

enum Type
{
	INT8,
	U_INT8,
	INT32,
	U_INT32,
	DOUBLE_LITERAL,
	VOID,
	STRUCT
};

enum TokenType
{
	WHITESPACE,
	IDENTIFIER,
	OPERATOR,
	TYPE_SPECIFIER,
	TYPE,
	UNKNOWN_TYPE,
	INT,
	DOUBLE_CONFUSED,
	DOUBLE,
	STRING,
	CHAR,
	STRING_ESCAPE,
	COMMENT_CONFUSED,
	COMMENT
};

const static std::string TokenTypeStrings[]
{
	"WHITESPACE",
	"IDENTIFIER",
	"OPERATOR",
	"TYPE_SPECIFIER",
	"TYPE",
	"UNKNOWN_TYPE",
	"INT",
	"DOUBLE_CONFUSED",
	"DOUBLE",
	"STRING",
	"CHAR",
	"STRING_ESCAPE",
	"COMMENT_CONFUSED",
	"COMMENT"
};

const static std::string TypeStrings[]
{
	"INT8",
	"UINT8",
	"INT32",
	"UINT32",
	"DOUBLE_LITERAL",
	"VOID",
	"STRUCT"
};

static std::map<std::string, Type> typeMap;

class Token
{
public:
	std::string text {};
	enum TokenType type {WHITESPACE};
	size_t lineNumber {}, charIndex{};

	void Info() const;
};

class Lexer
{
public:
	Lexer();

	std::vector<Token> Parse(const std::string&);

private:
	void CloseToken();

	Token curToken;
	std::vector<Token> tokens;
};

class UnknownEscapeSequence
{
public:
	UnknownEscapeSequence(std::string _msg)
		: msg {_msg} {}

	virtual const std::string What() const noexcept 
	{
		return msg;
	}

private:
	std::string msg;
};