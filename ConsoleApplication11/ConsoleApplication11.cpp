#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class SimpleStack {
private:
    char* arr;
    int capacity;
    int top;

public:
    SimpleStack(int size) : capacity(size), top(-1) {
        arr = new char[capacity];
    }

    ~SimpleStack() {
        delete[] arr;
    }

    void push(char c) {
        if (top < capacity - 1) {
            arr[++top] = c;
        }
    }

    char pop() {
        if (top >= 0) {
            return arr[top--];
        }
        return '\0';
    }

    char peek() {
        if (top >= 0) {
            return arr[top];
        }
        return '\0';
    }

    bool isEmpty() {
        return top == -1;
    }
    void saveToFile(const string& filename) {
        ofstream outFile(filename, ios::binary);
        if (!outFile) {
            cout << "Error opening file for writing!" << endl;
            return;
        }
        outFile.write(reinterpret_cast<char*>(&capacity), sizeof(capacity));
        outFile.write(reinterpret_cast<char*>(&top), sizeof(top));
        outFile.write(arr, top + 1);
        outFile.close();
    }
    void loadFromFile(const string& filename) {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            cout << "Error opening file for reading!" << endl;
            return;
        }
        inFile.read(reinterpret_cast<char*>(&capacity), sizeof(capacity));
        inFile.read(reinterpret_cast<char*>(&top), sizeof(top));
        delete[] arr; 
        arr = new char[capacity];
        inFile.read(arr, top + 1);  
        inFile.close();
    }
    void display() {
        for (int i = 0; i <= top; ++i) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }
};

bool isOpeningBracket(char c) {
    return c == '(' || c == '{' || c == '[';
}

bool isClosingBracket(char c) {
    return c == ')' || c == '}' || c == ']';
}

bool isMatchingPair(char opening, char closing) {
    return (opening == '(' && closing == ')') ||
        (opening == '{' && closing == '}') ||
        (opening == '[' && closing == ']');
}

string checkBrackets(const string& str) {
    SimpleStack bracketStack(str.length());

    for (size_t i = 0; i < str.length(); ++i) {
        char current = str[i];

        if (isOpeningBracket(current)) {
            bracketStack.push(current);
        }
        else if (isClosingBracket(current)) {
            if (bracketStack.isEmpty()) {
                return "Error: No matching opening bracket for " + string(1, current) + " at position " + to_string(i);
            }
            char top = bracketStack.pop();
            if (!isMatchingPair(top, current)) {
                return "Error: Mismatched brackets at position " + to_string(i);
            }
        }
        else if (current == ';') {
            break;
        }
    }
    if (!bracketStack.isEmpty()) {
        return "Error: Unmatched opening bracket found.";
    }
    return "Correctly placed brackets.";
}

int main() {
    string input;
    cout << "Enter a string (end with ';'): ";
    getline(cin, input);
    if (!input.empty() && input.back() == ';') {
        input.pop_back();
    }

    string result = checkBrackets(input);
    cout << result << endl;
    SimpleStack stack(input.length());
    for (char c : input) {
        stack.push(c);
    }
    stack.saveToFile("stack_data.dat");
    cout << "Stack saved to file." << endl;
    SimpleStack loadedStack(input.length());
    loadedStack.loadFromFile("stack_data.dat");
    cout << "Loaded stack from file: ";
    loadedStack.display();

    return 0;
}
