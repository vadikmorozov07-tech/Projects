#include <vector>
#include <stack>
#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int preora(const string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;
}

bool isOperator(const string& s) {
    return s == "+" || s == "-" || s == "*" || s == "/";
}

vector<string> tokenize(const string& s) {
    vector<string> tokens;
    for (size_t i = 0; i < s.size(); ) {
        if (s[i] == ' ') {
            ++i;
        }
        else if (s[i] >= '0' && s[i] <= '9') {
            string num;
            while (i < s.size() && s[i] >= '0' && s[i] <= '9') {
                num += s[i++];
            }
            tokens.push_back(num);
        }
        else {
            tokens.push_back(string(1, s[i++])); // строка длины 1 из символа i++
        }
    }
    return tokens;
}

vector<string> shunting_yard(const vector<string>& tokens) {
    vector<string> output; // результат
    stack<string> oper; // стек операторов и скобок

    for (const string& token : tokens) {
        if (token == "(") { // кладется (
            oper.push(token);
        }
        else if (token == ")") { // выгрузка всех операторов в output до ( и она удаляется
            while (!oper.empty() && oper.top() != "(") {
                output.push_back(oper.top());
                oper.pop();
            }
            if (!oper.empty()) {
                oper.pop();
            }
        }
        else if (isOperator(token)) { 
            while (!oper.empty() && isOperator(oper.top()) &&
                preora(oper.top()) >= preora(token)) {
                output.push_back(oper.top());
                oper.pop();
            }
            oper.push(token);
        }
        else { // токен число или переменная
            output.push_back(token);
        }
    }

    while (!oper.empty()) { // выгрузка операторов
        output.push_back(oper.top());
        oper.pop();
    }

    return output;
}

int main() {
    string str;
    getline(cin, str);

    vector<string> tokens = tokenize(str);
    vector<string> res = shunting_yard(tokens);

    for (const auto& x : res) {
        cout << x << ' ';
    }
    cout << '\n';
}

