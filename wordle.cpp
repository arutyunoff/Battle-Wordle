#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <clocale>
#include <locale>
#include <fstream>
#include <sstream>
#include <algorithm>
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::pair;
using std::sort;
using std::min;
using std::rand;
using std::srand;
using std::time;
using std::stol;
using std::ios;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::cerr;

// функция для проверки букв в слове и возврата текущего состояния
string wordchecker(const string& word, const string& trial) {
    string answer = "_____";  // изначально все буквы скрыты

    for (int i = 0; i < word.size(); i++) {
        if (trial[i] == word[i]) {
            answer[i] = word[i];  // открываем угаданную букву
        }
    }

    for (int i = 0; i < word.size(); i++) {
        if (answer[i] != '_') {
            continue;  // пропускаем уже угаданные буквы
        } if (word.find(trial[i]) != string::npos) {
            answer[i] = '-';  // буква есть в слове, но не на этом месте
        }
    }

    return answer;  // возвращаем текущее состояние слова
}

// функция для вывода состояния слова с цветом
void Colors(const string& word) {
    for (char ch : word) {
        if (ch == '_') {
            cout << RED << ch << RESET;  // подсвечиваем "_" красным
        } else if (ch == '-') {
            cout << YELLOW << ch << RESET;  // подсвечиваем "-" желтым
        } else {
            cout << GREEN << ch << RESET;  // подсвечиваем правильные буквы зеленым
        }
    }
    cout << endl;
}

// функция для записи результата в csv-файл
void save_results_csv(const string& playerName, long time_seconds) {
    ofstream file("results.csv", ios::app);  // открываем файл для добавления данных
    if (file.is_open()) {
        file << playerName << "," << time_seconds << endl;
        file.close();
    } else {
        cerr << "ошибка при открытии файла для записи результатов." << endl;
    }
}

// функция для чтения и вывода топ-5 результатов из csv-файла
void top_results() {
    ifstream file("results.csv");
    if (not file.is_open()) {
        cerr << "файл с результатами не найден." << endl;
        return;
    }

    vector<pair<string, long>> results;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name;
        string timeStr;
        getline(ss, name, ',');
        getline(ss, timeStr);
        long time_seconds = stol(timeStr);
        results.push_back({name, time_seconds});
    }
    file.close();

    // сортируем результаты по времени (от меньшего к большему)
    sort(results.begin(), results.end(), [](const pair<string, long>& a, const pair<string, long>& b) {
        return a.second < b.second;
    });

    // выводим топ-5 результатов
    cout << "топ-5 лучших результатов:" << endl;
    for (size_t i = 0; i < min(results.size(), size_t(5)); ++i) {
        cout << i + 1 << ". " << results[i].first << " - " << results[i].second << " секунд" << endl;
    }
}

// функция для запроса начала игры
bool start_question(const string& language) {
    string answer;
    if (language == "russian") {
        cout << "хотите начать игру? (да/нет): ";
    } else {
        cout << "do you want to start the game? (yes/no): ";
    }
    cin >> answer;

    if (language == "russian") {
        if (answer == "да") {
            return true;
        } else {
            return false;
        }
    } else {
        if (answer == "yes") {
            return true;
        } else {
            return false;
        }
    }
}

// функция для проведения раунда игры
long playRound(const string& word, int round, const string& playerName, const string& language) {
    string trial;
    int attempts = 5;
    auto start = high_resolution_clock::now();
    string shablon = "_____";  // начальное состояние слова

    if (language == "russian") {
        cout << "у вас есть 5 попыток, чтобы угадать слово." << endl;
    } else {
        cout << "you have 5 attempts to guess the word." << endl;
    }

    while (attempts > 0) {
        if (language == "russian") {
            cout << playerName << ", ваше слово: ";
        } else {
            cout << playerName << ", your word: ";
        }

        Colors(shablon);  // выводим слово с цветами
        cout << endl;

        if (language == "russian") {
            cout << "введи слово: ";
        } else {
            cout << "enter the word: ";
        }
        cin >> trial;

        if (trial.size() != word.size()) {
            if (language == "russian") {
                cout << "слово должно быть из 5 букв!" << endl;
            } else {
                cout << "the word must be 5 letters long!" << endl;
            }
            continue;
        }

        shablon = wordchecker(word, trial);  // обновляем состояние слова
        attempts--;

        if (trial == word) {
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<seconds>(end - start);
            long time_seconds = duration.count();

            if (language == "russian") {
                if (time_seconds >= 60) {
                    long minutes = time_seconds / 60;
                    long seconds = time_seconds % 60;
                    cout << "поздравляю, " << playerName << "! ты угадал слово за " << minutes << " минут " << seconds << " секунд!" << endl;
                } else {
                    cout << "поздравляю, " << playerName << "! ты угадал слово за " << time_seconds << " секунд!" << endl;
                }
            } else {
                if (time_seconds >= 60) {
                    long minutes = time_seconds / 60;
                    long seconds = time_seconds % 60;
                    cout << "congratulations, " << playerName << "! you guessed the word in " << minutes << " minutes " << seconds << " seconds!" << endl;
                } else {
                    cout << "congratulations, " << playerName << "! you guessed the word in " << time_seconds << " seconds!" << endl;
                }
            }

            // сохраняем результат в csv-файл
            save_results_csv(playerName, time_seconds);
            return time_seconds;
        }

        if (attempts > 0) {
            if (language == "russian") {
                cout << "осталось попыток: " << attempts << endl;
            } else {
                cout << "attempts left: " << attempts << endl;
            }
        }
    }

    if (language == "russian") {
        cout << "попытки закончились. загаданное слово было: " << word << endl;
    } else {
        cout << "no attempts left. the word was: " << word << endl;
    }
    return -1;  // возвращаем -1, если слово не было угадано
}

// функция для вывода правил игры
void Rules(const string& language) {
    if (language == "russian") {
        cout << "Правила игры:" << endl;
        cout << "1. Вам нужно угадать слово из 5 букв." << endl;
        cout << "2. У вас есть 5 попыток." << endl;
        cout << "3. Если буква в слове есть, но не на своем месте, она будет отмечена " << YELLOW << "-" << RESET << "." << endl;
        cout << "4. Если буква на своем месте, она будет подсвечена " << GREEN << "зеленым" << RESET << "." << endl;
        cout << "5. Если буквы нет в слове, она останется " << RED << "_" << RESET << "." << endl;
        cout << "6. Вводить русские слова надо английскими буквами." << endl;
        cout << "Пример для слова \"slovo\":" << endl;
        cout << "Ввод: stolb" << endl;
        cout << "Результат: " << GREEN << "s" << RED << "_" << GREEN << "o" << YELLOW << "-" << RED << "_" << RESET << endl;
        cout << "Объяснение:" << endl;
        cout << GREEN << "s" << RESET << " - правильная буква на своем месте." << endl;
        cout << RED << "t" << RESET << " - буквы t нет в слове." << endl;
        cout << GREEN << "o" << RESET << " - правильная буква на своем месте." << endl;
        cout << YELLOW << "l" << RESET << " - буква есть в слове, но не на своем месте." << endl;
        cout << RED << "b" << RESET << " - буквы b нет в слове." << endl;
    } else {
        cout << "Game rules:" << endl;
        cout << "1. You need to guess a 5-letter word." << endl;
        cout << "2. You have 5 attempts." << endl;
        cout << "3. If a letter is in the word but not in the correct position, it will be marked with " << YELLOW << "-" << RESET << "." << endl;
        cout << "4. If a letter is in the correct position, it will be highlighted in " << GREEN << "green" << RESET << "." << endl;
        cout << "5. If a letter is not in the word, it will remain " << RED << "_" << RESET << "." << endl;
        cout << "Example for the word \"apple\":" << endl;
        cout << "Input: eagle" << endl;
        cout << "Result: " << YELLOW << "-" << YELLOW << "-" << RED << "_" << GREEN << "l" << GREEN << "e" << RESET << endl;
        cout << "Explanation:" << endl;
        cout << YELLOW << "e" << RESET << " - letter is in the word but not in this position." << endl;
        cout << YELLOW << "a" << RESET << " - letter is in the word but not in this position." << endl;
        cout << RED << "_" << RESET << " - letter g is not in the word." << endl;
        cout << GREEN << "l" << RESET << " - correct letter in the correct position." << endl;
        cout << GREEN << "e" << RESET << " - correct letter in the correct position." << endl;
    }
}


int main() {
    vector<string> englishWords = {
    "apple", "table", "chair", "pizza", "bread", "world", "light", "phone", "money", "water",
    "plant", "cloud", "night", "watch", "dream", "heart", "smile", "glass", "dance", "child",
    "stone", "queen", "field", "dress", "brush", "block", "fruit", "grass", "party", "paper",
    "movie", "green", "floor", "sugar", "color", "tooth", "place", "badge", "sweet", "stick",
    "radio", "peach", "fight", "juice", "crane", "beast", "ghost", "sheep", "earth", "sharp",
    "peace", "paint", "truth", "space", "train", "cream", "nurse", "punch", "shape", "snore",
    "wheel", "horse", "brand", "flower", "metro"
};
    vector<string> russianWords = {
    "domik", "stolb", "veter", "bokal", "kamin", "dymok", "surok", "vagon", 
    "tuman", "rojok", "kloun", "utyug", "styul", "yubka", "sosal", "dymka",
    "povar", "mesto", "obyem", "sadik", "cveti", "lesok", "veter", "plita", 
    "ryvok", "forma", "otriv", "uroky", "kluch", "vklad", "vyvod", "yazyk",
    "zamok", "gorod", "stvol", "zmeiy", "bilet", "vagon", "pokaz", "metro",
    "rybka", "slovo", "griby", "metro"
};

    srand(time(nullptr));  // инициализация генератора случайных чисел

    string language1, language2;
    cout << "игрок 1, выберите язык / player 1, choose language (english/русский): ";
    cin >> language1;

    if (language1 == "русский") {
        language1 = "russian";
    } else if (language1 != "english") {
        cout << "неверный выбор языка. по умолчанию выбран английский. / invalid language choice. defaulting to english." << endl;
        language1 = "english";
    }

    string playerName1;
    if (language1 == "russian") {
        cout << "игрок 1, введите свое имя: ";
    } else {
        cout << "player 1, enter your name: ";
    }
    cin >> playerName1;

    cout << "игрок 2, выберите язык / player 2, choose language (english/русский): ";
    cin >> language2;

    if (language2 == "русский") {
        language2 = "russian";
    } else if (language2 != "english") {
        cout << "неверный выбор языка. по умолчанию выбран английский. / invalid language choice. defaulting to english." << endl;
        language2 = "english";
    }

    string playerName2;
    if (language2 == "russian") {
        cout << "игрок 2, введите свое имя: ";
    } else {
        cout << "player 2, enter your name: ";
    }
    cin >> playerName2;

    // вывод правил игры (только один раз, если языки совпадают)
    if (language1 == language2) {
        Rules(language1);
    } else {
        Rules(language1);
        Rules(language2);
    }

    // спрашиваем о запуске игры
    if (not start_question(language1)) {
        if (language1 == "russian") {
            cout << "Игра завершена. До свидания!" << endl;
        } else {
            cout << "Game over. Goodbye!" << endl;
        }
        return 0;
    }

    vector<string> words1;
    if (language1 == "russian") {
        words1 = russianWords;
    } else {
        words1 = englishWords;
    }

    vector<string> words2;
    if (language2 == "russian") {
        words2 = russianWords;
    } else {
        words2 = englishWords;
    }

    int wins1 = 0, wins2 = 0;
    int round = 1;

    while (wins1 < 3 && wins2 < 3) {
        cout << "раунд " << round << "!" << endl;

        string word1 = words1[rand() % words1.size()];  // случайное слово для игрока 1
        long time1 = playRound(word1, round, playerName1, language1);

        string word2 = words2[rand() % words2.size()];  // случайное слово для игрока 2
        long time2 = playRound(word2, round, playerName2, language2);

        if (time1 != -1 && time2 != -1) {
            if (time1 < time2) {
                wins1++;
                if (language1 == "russian") {
                    cout << playerName1 << " выиграл этот раунд!" << endl;
                } else {
                    cout << playerName1 << " won this round!" << endl;
                }
            } else if (time1 > time2) {
                wins2++;
                if (language2 == "russian") {
                    cout << playerName2 << " выиграл этот раунд!" << endl;
                } else {
                    cout << playerName2 << " won this round!" << endl;
                }
            } else {
                if (language1 == "russian") {
                    cout << "ничья в этом раунде!" << endl;
                } else {
                    cout << "it's a tie in this round!" << endl;
                }
            }
        } else if (time1 != -1) {
            wins1++;
            if (language1 == "russian") {
                cout << playerName1 << " выиграл этот раунд!" << endl;
            } else {
                cout << playerName1 << " won this round!" << endl;
            }
        } else if (time2 != -1) {
            wins2++;
            if (language2 == "russian") {
                cout << playerName2 << " выиграл этот раунд!" << endl;
            } else {
                cout << playerName2 << " won this round!" << endl;
            }
        } else {
            if (language1 == "russian") {
                cout << "ничья в этом раунде!" << endl;
            } else {
                cout << "it's a tie in this round!" << endl;
            }
        }

        round++;
    }

    if (wins1 > wins2) {
        if (language1 == "russian") {
            cout << playerName1 << " выиграл игру со счетом " << wins1 << ":" << wins2 << "!" << endl;
        } else {
            cout << playerName1 << " won the game with a score of " << wins1 << ":" << wins2 << "!" << endl;
        }
    } else if (wins1 < wins2) {
        if (language2 == "russian") {
            cout << playerName2 << " выиграл игру со счетом " << wins2 << ":" << wins1 << "!" << endl;
        } else {
            cout << playerName2 << " won the game with a score of " << wins2 << ":" << wins1 << "!" << endl;
        }
    } else {
        if (language1 == "russian") {
            cout << "игра закончилась вничью со счетом " << wins1 << ":" << wins2 << "!" << endl;
        } else {
            cout << "the game ended in a draw with a score of " << wins1 << ":" << wins2 << "!" << endl;
        }
    }

    // вывод топ-5 лучших результатов
    top_results();
}