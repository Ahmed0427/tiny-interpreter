#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <climits>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include "stack.cpp" // User-defined implementation of Stack
#include "BST.cpp"   // User-defined implementation of Binary Search Tree

using namespace std;

unordered_map<string, int> variables;
const string throwMessage = "Invalid Syntax";

string trim(const string &s)
{
    size_t start = s.find_first_not_of(" \t\n\r\f\v");

    size_t end = s.find_last_not_of(" \t\n\r\f\v");

    if (start == string::npos)
        return "";

    return s.substr(start, end - start + 1);
}

vector<string> split(const string &s, char delimiter, int maxSplits = INT_MAX)
{
    vector<string> tokens;
    string token;

    int i = 0;
    for (; i < s.size() && maxSplits > 0; i++)
    {
        if (s[i] == delimiter)
        {
            token = trim(token);
            tokens.push_back(token);
            maxSplits--;
            token = "";
        }
        else
        {
            token += s[i];
        }
    }

    token = trim(token);
    if (token != "")
        tokens.push_back(token);

    token = "";

    while (i < s.size())
    {
        token += s[i];
        i++;
    }

    token = trim(token);
    if (token != "")
        tokens.push_back(token);

    return tokens;
}

bool isAllDigits(string &s)
{
    for (char c : s)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

bool isAllAlpha(const string &s)
{
    for (char c : s)
    {
        if (!isalpha(c))
        {
            return false;
        }
    }
    return true;
}

bool isOperator(const string &token)
{
    return token == "+" || token == "-" || token == "*" ||
           token == "/" || token == ">" || token == "<" ||
           token == ">=" || token == "<=" || token == "==" ||
           token == "!=" || token == "&&" ||
           token == "||" || token == "%";
}

int getPrecedence(const string &op)
{
    if (op == "*" || op == "/" || op == "%")
        return 3;
    if (op == "+" || op == "-")
        return 2;
    if (op == ">" || op == "<")
        return 1;
    if (op == ">=" || op == "<=")
        return 1;
    if (op == "==" || op == "!=")
        return 1;
    if (op == "&&" || op == "||")
        return 0;
    return -1;
}

vector<string> normalToRPN(vector<string> &tokens)
{
    Stack<string> st;
    vector<string> result;

    for (string &token : tokens)
    {
        if (isAllDigits(token))
        {
            result.push_back(token);
        }
        else if (isAllAlpha(token))
        {
            result.push_back(token);
        }
        else if (isOperator(token))
        {
            while (!st.empty() && getPrecedence(st.top()) >= getPrecedence(token))
            {
                result.push_back(st.top());
                st.pop();
            }
            st.push(token);
        }
        else if (token == "(")
        {
            st.push(token);
        }
        else if (token == ")")
        {
            while (!st.empty() && st.top() != "(")
            {
                result.push_back(st.top());
                st.pop();
            }
            if (st.empty())
            {
                throw invalid_argument(throwMessage);
            }
            st.pop();
        }
        else
        {
            throw invalid_argument(throwMessage);
        }
    }

    while (!st.empty())
    {
        if (st.top() == "(" || st.top() == ")")
        {
            throw invalid_argument(throwMessage);
        }
        result.push_back(st.top());
        st.pop();
    }

    return result;
}

int evaluate(string &expression)
{
    vector<string> tokens = split(expression, ' ');
    vector<string> rpn = normalToRPN(tokens);

    Stack<int> result;
    for (string token : rpn)
    {
        if (!isOperator(token))
        {
            if (isAllDigits(token))
            {
                result.push(stoi(token));
            }
            else if (isAllAlpha(token) && variables.count(token))
            {
                result.push(variables[token]);
            }
            else
            {
                throw invalid_argument("Invalid Expression");
            }
        }
        else
        {
            if (result.size() < 2)
                throw invalid_argument("Invalid Expression");

            int num2 = result.top();
            result.pop();
            int num1 = result.top();
            result.pop();

            int value = 0;
            if (token == "-")
                value = num1 - num2;
            if (token == "+" || token == "||")
                value = num1 + num2;
            if (token == "*" || token == "&&")
                value = num1 * num2;
            if (token == "/")
            {
                if (num2 == 0)
                    throw invalid_argument("Invalid Expression");
                value = num1 / num2;
            }
            if (token == "%")
            {
                if (num2 == 0)
                    throw invalid_argument("Invalid Expression");
                value = num1 % num2;
            }
            if (token == ">")
            {
                value = (num1 > num2) ? 1 : 0;
            }
            if (token == "<")
            {
                value = (num1 < num2) ? 1 : 0;
            }
            if (token == ">=")
            {
                value = (num1 >= num2) ? 1 : 0;
            }
            if (token == "<=")
            {
                value = (num1 <= num2) ? 1 : 0;
            }
            if (token == "==")
            {
                value = (num1 == num2) ? 1 : 0;
            }
            if (token == "!=")
            {
                value = (num1 != num2) ? 1 : 0;
            }

            result.push(value);
        }
    }

    if (result.size() != 1)
        throw invalid_argument("Invalid Expression");
    return result.top();
}

void executeLine(string &line)
{
    if (line[0] == '#')
        return;
    if (line.find(" = ") != string::npos)
    {
        vector<string> tokens = split(line, '=', 1);
        variables[tokens[0]] = evaluate(tokens[1]);
    }
    else if (isAllAlpha(line))
    {
        if (!variables.count(line))
            throw invalid_argument("Identifier " + line + " is undefined");

        cout << variables[line] << endl;
    }
    else
    {
        cout << evaluate(line) << endl;
    }
}

void executeWhileLoopLine(const vector<string> &lines, int &currentLineIndex)
{
    string whileLine = lines[currentLineIndex];
    size_t start = whileLine.find(' ');
    if (start == string::npos)
        throw invalid_argument("Invalid While Loop Syntax");

    string condition = trim(whileLine.substr(start + 1));
    if (condition.empty())
        throw invalid_argument("While loop missing condition");

    vector<string> loopBody;
    currentLineIndex++;
    while (currentLineIndex < lines.size() && lines[currentLineIndex][0] == ' ')
    {
        loopBody.push_back(trim(lines[currentLineIndex]));
        currentLineIndex++;
    }
    currentLineIndex--;

    int maxIterations = 1000;
    int iteration = 0;

    while (evaluate(condition) != 0)
    {
        if (++iteration > maxIterations)
            throw runtime_error("Infinite loop detected");
        for (string &line : loopBody)
        {
            executeLine(line);
        }
    }
}

void handleInputLines(vector<string> &lines)
{
    for (int lineNumber = 0; lineNumber < lines.size(); lineNumber++)
    {
        string line = lines[lineNumber];
        if (line.substr(0, 5) == "while")
        {
            executeWhileLoopLine(lines, lineNumber);
        }
        else
        {
            executeLine(line);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {

        ifstream file(argv[1]);

        if (!file)
        {
            cerr << "Error opening the file" << endl;
            return 1;
        }

        string line = "";
        vector<string> lines;
        while (getline(file, line))
        {
            if (line[0] == ' ')
            {
                lines.push_back(line);
                continue;
            }
            line = trim(line);
            if (line == "")
                continue;
            lines.push_back(line);
        }

        try
        {
            handleInputLines(lines);
        }
        catch (exception &e)
        {
            cerr << e.what() << endl;
            return 1;
        }
    }
    else
    {
        cerr << "Must provide file name" << endl;
        return 1;
    }

    return 0;
}
