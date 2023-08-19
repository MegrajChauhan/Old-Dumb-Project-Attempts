// the decent parser example: simple but not useful for lexer based parsers
#include <iostream>
#include <string>

class ParseError : public std::exception {};

class Parser {
public:
    Parser(const std::string& expression) : expression_(expression), pos_(0) {}
    int parse() {
        int result = parse_expression();
        if (pos_ < expression_.size()) {
            throw ParseError();
        }
        return result;
    }
private:
    std::string expression_;
    std::size_t pos_;

    int parse_expression() {
        int left = parse_term();
        std::cout<<left<<std::endl;
        while (pos_ < expression_.size() && (expression_[pos_] == '+' || expression_[pos_] == '-')) {
            char op = expression_[pos_++];
            int right = parse_term();
            if (op == '+') {
                left += right;
            } else {
                left -= right;
            }
        }
        return left;
    }

    int parse_term() {
        int left = parse_factor();
        std::cout<<left<<std::endl;
        while (pos_ < expression_.size() && (expression_[pos_] == '*' || expression_[pos_] == '/')) {
            char op = expression_[pos_++];
            int right = parse_factor();
            if (op == '*') {
                left *= right;
            } else {
                left /= right;
            }
        }
        return left;
    }

    int parse_factor() {
        if (pos_ < expression_.size() && expression_[pos_] == '(') {
            pos_++;
            int result = parse_expression();
            if (pos_ < expression_.size() && expression_[pos_] == ')') {
                pos_++;
                return result;
            } else {
                throw ParseError();
            }
        } else if (pos_ < expression_.size() && std::isdigit(expression_[pos_])) {
            int result = 0;
            while (pos_ < expression_.size() && std::isdigit(expression_[pos_])) {
                result = result * 10 + (expression_[pos_] - '0');
                pos_++;
            }
            return result;
        } else {
            throw ParseError();
        }
    }
};

int main() {
    Parser parser("2*(3+4)-5/2");
    try {
        int result = parser.parse();
        std::cout << "Result: " << result << std::endl;
    } catch (const ParseError&) {
        std::cerr << "Error parsing expression" << std::endl;
    }
    return 0;
}