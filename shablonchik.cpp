#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <algorithm>
#include <sqlite3.h>
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

using namespace std;

void wordchecker(const string& word, const string& trial) {  // функция для проверки букв в слове
    string answer = "_____";

    for (int i = 0; i < word.size(); i++) {
        if (trial[i] == word[i]) {
            answer[i] = '+';  // если буква есть в слове, то заменяем прочерк на +
        }
    }

    for (int i = 0; i < word.size(); i++) {
        if (answer[i] == '+' || word.find(trial[i]) == string::npos) {  // проверка есть ли вообще буква в слове
            continue;
        }

        for (int j = 0; j < word.size(); j++) {
            if (trial[i] == word[j] && answer[j] == '_') {
                answer[i] = '-';  // если буквы нет, то заменяем на -
                break;
            }
        }
    }

    cout << answer << endl;
}

bool timechecker() { // функция для добавления времени
    return 0;
}

int main() {
    string word = "actor";  // наше слово из 5 букв
    string trial;  // попытка пользователя

    cout << "Приветствую, игрок! Отгадай слово из 5 букв" << endl;

    while (true) {
        cout << "Введи слово: ";
        cin >> trial;

        if (trial.size() != word.size()) {
            cout << "Слово должно быть из 5 букв!" << endl;
            continue;
        }

        wordchecker(word, trial);

        if (trial == word) {
            cout << "Поздравляю, ты угадал!" << endl;
            break;
        }
    }
}