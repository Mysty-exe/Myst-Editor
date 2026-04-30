#include "scanner.h"

LexerScanner::LexerScanner()
{
    setLexers();
    docString = false;
    currentLang = NONE;
}

void LexerScanner::setLexers()
{
    if (currentLang == PYTHON)
    {
        punctuation = {","};
        delimiters = {"(", ")", "[", "]", "{", "}", ":", ";"};
        singleOperators = {"=", ">", "<", "+", "-", "*", "/", "%", "**", "|", "@"};
        doubleOperators = {"==", "<=", ">=", "+=", "-=", "*=", "/=", "!=", "//=", "**=", "<<", ">>", "<<=", ">>=", "|=", "&=", "^=", ":="};
        specialChars = {R"(\n)", R"(\t)", R"(\r)", R"(\e)", R"(\a)", R"(\f)", R"(\v)", R"(\b)", R"(\')", R"(\")", R"(\\)"};
        keyWords = {"and", "as", "assert", "break", "class", "continue", "def", "del", "elif", "else", "except", "False", "True", "finally", "for", "from", "global", "if", "import", "in", "is", "lambda", "None", "nonlocal", "not", "or", "pass", "raise", "try", "while", "with", "return", "yield"};
        functions = {"abs", "all", "any", "ascii", "bin", "bool", "breakpoint", "bytearray",
                     "bytes", "callable", "chr", "classmethod", "compile", "complex", "delattr",
                     "dict", "dir", "divmod", "enumerate", "eval", "exec", "filter", "float",
                     "format", "frozenset", "getattr", "globals", "hasattr", "hash", "help",
                     "hex", "id", "input", "int", "isinstance", "issubclass", "iter", "len",
                     "list", "locals", "map", "max", "memoryview", "min", "next", "object",
                     "oct", "open", "ord", "pow", "print", "property", "range", "repr", "reversed",
                     "round", "set", "setattr", "slice", "sorted", "staticmethod", "str", "sum",
                     "super", "tuple", "type", "vars", "zip", "__import__"};
    }

    if (currentLang == CPP)
    {
        punctuation = {",", ";", ".", ":"};
        delimiters = {"(", ")", "[", "]", "{", "}"};
        singleOperators = {"=", ">", "<", "+", "-", "*", "/", "%", "&", "|", "!", "^", "~", "?"};
        doubleOperators = {"==", "<=", ">=", "+=", "-=", "*=", "/=", "!=", "->", ".*", "::", ">>", "<<"};
        specialChars = {R"(\n)", R"(\t)", R"(\r)", R"(\e)", R"(\a)", R"(\f)", R"(\v)", R"(\b)", R"(\')", R"(\")", R"(\\)"};
        specialWords = {"#include", "#define", "#undef", "#if", "#ifdef", "#ifndef",
                        "#elif", "#else", "#endif", "#pragma", "#error", "#line"};
        keyWords = {"bool", "int", "float", "double", "long", "char", "void", "if",
                    "else", "switch", "case", "for", "while", "do",
                    "return", "break", "continue", "class", "struct",
                    "enum", "union", "public", "private", "protected",
                    "virtual", "override", "final", "template",
                    "typename", "constexpr", "const", "static",
                    "namespace", "using", "new", "delete", "try",
                    "catch", "throw", "inline", "auto", "decltype"};
        functions = {"main"};
    }
}

void LexerScanner::setLanguage(Language language)
{
    currentLang = language;
    setLexers();
}

bool LexerScanner::isNum(string txt)
/**
Checks if every character in txt is a number

Args:
    (string) txt: Text to check
Returns:
    bool
 */

{
    if (txt.compare(0, 2, "0x") == 0 && txt.size() > 2 && txt.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos)
        return true;

    int decimal = 0;
    int exponent = 0;
    for (int x = 0; x < (int)txt.length(); x++)
    {
        if (txt[x] == '.')
        {
            decimal += 1;
        }
        else if (!isdigit(txt[x]))
        {
            if ((txt[x] == 'e' || txt[x] == 'E') && x != 0)
                exponent++;
            else
                return false;
        }
        if (decimal > 1 || exponent > 1)
            return false;
    }
    return true;
}

void LexerScanner::setDocString(bool doc)
/**
docString Setter Function

Args:
    (bool) doc = new doc bool
Returns:
    void
 */

{
    docString = doc;
}

vector<string> LexerScanner::getLexemes(string code)
/**
Gets all the lexemes in the line

Args:
    (string) code: Line to check
Returns:
    vector<string>
 */

{
    int i = 0;
    vector<string> lexemes = {};
    string currentLexeme = "";
    bool startString = false;
    bool comment = false;

    if (!docString)
    {
        for (std::string specialWord : specialWords)
        {
            if (code.starts_with(specialWord))
            {
                lexemes.push_back(code);
                return lexemes;
            }
        }
    }

    for (int x = 0; x < (int)code.length(); x++)
    {
        if (currentLang == PYTHON)
        {
            if ((code.substr(i, 3) == "\"\"\"" || code.substr(i, 3) == "'''"))
            {
                docString = not docString;
                if (!docString)
                    currentLexeme += code.substr(i, 3);
                if (currentLexeme != "")
                {
                    lexemes.push_back(currentLexeme);
                    currentLexeme = "";
                }
                if (docString)
                    currentLexeme += code.substr(i, 3);
                i += 3;

                if (i == (int)code.length())
                {
                    if (currentLexeme != "")
                    {
                        lexemes.push_back(currentLexeme);
                    }
                    break;
                }
                continue;
            }
            else if (code.substr(i, 1) == "#" && !docString)
            {
                comment = true;
                if (currentLexeme != "")
                {
                    lexemes.push_back(currentLexeme);
                    currentLexeme = "";
                }
                currentLexeme += code[i];
                i++;

                if (i == (int)code.length())
                {
                    if (currentLexeme != "")
                    {
                        lexemes.push_back(currentLexeme);
                    }
                    break;
                }
                continue;
            }
        }
        else if (currentLang == CPP)
        {
            if ((code.substr(i, 2) == "/*"))
                docString = true;
            if ((code.substr(i, 2) == "*/"))
                docString = false;

            if ((code.substr(i, 2) == "/*") || (code.substr(i, 2) == "*/"))
            {
                if (!docString)
                    currentLexeme += code.substr(i, 2);
                if (currentLexeme != "")
                {
                    lexemes.push_back(currentLexeme);
                    currentLexeme = "";
                }
                if (docString)
                    currentLexeme += code.substr(i, 2);
                i += 2;

                if (i == (int)code.length())
                {
                    if (currentLexeme != "")
                    {
                        lexemes.push_back(currentLexeme);
                    }
                    break;
                }
                continue;
            }
            else if (code.substr(i, 2) == "//" && !docString)
            {
                comment = true;
                if (currentLexeme != "")
                {
                    lexemes.push_back(currentLexeme);
                    currentLexeme = "";
                }
                currentLexeme += code[i];
                i++;

                if (i == (int)code.length())
                {
                    if (currentLexeme != "")
                    {
                        lexemes.push_back(currentLexeme);
                    }
                    break;
                }
                continue;
            }
        }

        if ((code.substr(i, 1) == "\"" || code.substr(i, 1) == "'") && !comment && !docString)
        {
            if (!startString)
            {
                startString = true;
                if (currentLexeme != "")
                {
                    lexemes.push_back(currentLexeme);
                    currentLexeme = "";
                }
                currentLexeme += code[i];
            }
            else
            {
                currentLexeme += code[i];
                lexemes.push_back(currentLexeme);
                currentLexeme = "";
                startString = false;
            }
        }
        else if (startString || comment || docString)
        {
            currentLexeme += code[i];
        }
        else if ((find(delimiters.begin(), delimiters.end(), code.substr(i, 1)) != delimiters.end() || code.substr(i, 1) == ",") && !startString && !comment)
        {
            if (currentLexeme != "")
            {
                lexemes.push_back(currentLexeme);
                currentLexeme = "";
            }
            lexemes.push_back(code.substr(i, 1));
        }
        else if (find(doubleOperators.begin(), doubleOperators.end(), code.substr(i, 2)) != doubleOperators.end() && !startString && !comment)
        {
            if (currentLexeme != "")
            {
                lexemes.push_back(currentLexeme);
                currentLexeme = "";
            }
            lexemes.push_back(code.substr(i, 2));
            i++;
        }
        else if (find(singleOperators.begin(), singleOperators.end(), code.substr(i, 1)) != singleOperators.end() && !startString && !comment)
        {
            if (currentLexeme != "")
            {
                lexemes.push_back(currentLexeme);
                currentLexeme = "";
            }
            lexemes.push_back(code.substr(i, 1));
        }
        else if (code.substr(i, 1) != " ")
        {
            currentLexeme += code[i];
        }
        else
        {
            if (currentLexeme != "")
            {
                lexemes.push_back(currentLexeme);
                currentLexeme = "";
            }
            lexemes.push_back(" ");
        }

        i++;

        if (i == (int)code.length())
        {
            if (currentLexeme != "")
            {
                lexemes.push_back(currentLexeme);
            }
            break;
        }
    }

    return lexemes;
}

vector<pair<string, Token>> LexerScanner::getTokens(string code)
/**
Gets all tokens from the lexemes in a line of code

Args:
    (string) code: Line of code to check
Returns:
    vector<pair<string, Token>>
 */

{
    vector<pair<string, Token>> lexicalizedCode;
    vector<string> lexemes = getLexemes(code);

    for (string lexeme : lexemes)
    {
        if (lexeme == " ")
        {
            lexicalizedCode.push_back(make_pair(" ", SPACE));
        }
        else if (find(delimiters.begin(), delimiters.end(), lexeme) != delimiters.end() && lexeme != ",")
        {
            lexicalizedCode.push_back(make_pair(lexeme, DELIMITER));
        }
        else if (lexeme == ",")
        {
            lexicalizedCode.push_back(make_pair(lexeme, PUNCTUATION));
        }
        else if (find(singleOperators.begin(), singleOperators.end(), lexeme) != singleOperators.end() || find(doubleOperators.begin(), doubleOperators.end(), lexeme) != doubleOperators.end())
        {
            lexicalizedCode.push_back(make_pair(lexeme, OPERATOR));
        }
        else if (std::any_of(specialWords.begin(), specialWords.end(), [&](const std::string &p)
                             { return code.starts_with(p); }))
        {
            lexicalizedCode.push_back(make_pair(lexeme, SPECIAL));
        }
        else if ((currentLang == PYTHON && (lexeme.substr(0, 3) == "\"\"\"" || lexeme.substr(0, 3) == "'''")) || (currentLang == CPP && lexeme.substr(0, 2) == "/*"))
        {
            lexicalizedCode.push_back(make_pair(lexeme, COMMENT));
        }
        else if ((currentLang == PYTHON && lexeme.size() > 2 && (lexeme.substr(lexeme.size() - 3, 3) == "\"\"\"" || lexeme.substr(lexeme.size() - 3, 3) == "'''")) || (currentLang == CPP && lexeme.size() > 1 && (lexeme.substr(lexeme.size() - 2, 2) == "*/")))
        {
            lexicalizedCode.push_back(make_pair(lexeme, COMMENT));
        }
        else if ((docString && ((currentLang == PYTHON && code.find("\"\"\"") == string::npos && code.find("'''") == string::npos) || (currentLang == CPP && code.find("/*") == string::npos))) || (lexeme[0] == '#' && currentLang == PYTHON) || (lexeme.substr(0, 2) == "//" && currentLang == CPP))
        {
            lexicalizedCode.push_back(make_pair(lexeme, COMMENT));
        }
        else if (lexeme[0] == '\'' || lexeme[0] == '"')
        {
            if (lexicalizedCode.size() > 0)
            {
                if (currentLang == PYTHON)
                {
                    if (lexicalizedCode[lexicalizedCode.size() - 1].first == "r")
                        lexicalizedCode[lexicalizedCode.size() - 1].second = STRING;
                }
                else if (currentLang == CPP)
                {
                    if (lexeme.size() > 1)
                    {
                        if (lexicalizedCode[lexicalizedCode.size() - 1].first == "R" && lexeme[1] == '(' && lexeme[lexeme.size() - 2] == ')')
                            lexicalizedCode[lexicalizedCode.size() - 1].second = STRING;
                    }
                }
            }

            string txt = "";
            int i = 0;
            for (int x = 0; x < (int)lexeme.length(); x++)
            {
                if (find(specialChars.begin(), specialChars.end(), lexeme.substr(i, 2)) != specialChars.end())
                {
                    lexicalizedCode.push_back(make_pair(txt, STRING));
                    lexicalizedCode.push_back(make_pair(lexeme.substr(i, 2), SPECIAL));
                    txt = "";
                    i++;
                }
                else
                {
                    txt += lexeme[i];
                }
                i++;

                if (i == (int)lexeme.length())
                {
                    break;
                }
            }
            if ((int)txt.length() > 0)
            {
                lexicalizedCode.push_back(make_pair(txt, STRING));
            }
        }
        else if (find(keyWords.begin(), keyWords.end(), lexeme) != keyWords.end())
        {
            lexicalizedCode.push_back(make_pair(lexeme, KEYWORD));
        }
        else if (find(functions.begin(), functions.end(), lexeme) != functions.end() && lexeme != ",")
        {
            lexicalizedCode.push_back(make_pair(lexeme, OTHER));
        }
        else if (isNum(lexeme))
        {
            lexicalizedCode.push_back(make_pair(lexeme, NUMBER));
        }
        else
        {
            if (lexeme.find('.') != std::string::npos)
            {
                string txt = "";
                for (int x = 0; x < (int)lexeme.length(); x++)
                {
                    if (lexeme[x] == '.')
                    {
                        if ((int)txt.length() > 0)
                        {
                            lexicalizedCode.push_back(make_pair(txt, TEXT));
                            txt = "";
                        }
                        lexicalizedCode.push_back(make_pair(".", PUNCTUATION));
                    }
                    else
                    {
                        txt += lexeme[x];
                    }
                }
                if ((int)txt.length() > 0)
                {
                    lexicalizedCode.push_back(make_pair(txt, TEXT));
                }
            }
            else
            {
                lexicalizedCode.push_back(make_pair(lexeme, TEXT));
            }
        }
    }

    return lexicalizedCode;
}
