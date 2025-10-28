#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

int main() {
    ifstream file("google-10000-english-usa-no-wears.txt");
    if (!file) {
        cerr << "Error: could not open word list file.\n";
        return 1;
    }

    vector<string> words;
    string word;

    // Load words of reasonable length
    while (file >> word) {
        if (word.size() >= 4 && word.size() <= 8)
            words.push_back(word);
    }

    if (words.empty()) {
        cerr << "No words loaded.\n";
        return 1;
    }

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, words.size() - 1);

    cout << "Welcome to the Word Scramble Game!\n";
    cout << "Type 'quit' anytime to exit.\n\n";

    while (true) {
        string original = words[dist(rng)];
        string scrambled = original;

        // Scramble until it's different
        do {
            shuffle(scrambled.begin(), scrambled.end(), rng);
        } while (scrambled == original);

        cout << "Scrambled word: " << scrambled << endl;
        cout << "Your guess: ";

        string guess;
        getline(cin, guess);

        if (guess == "quit") {
            cout << "Goodbye!\n";
            break;
        }

        if (guess == original) {
            cout << "Correct! The word was '" << original << "'.\n\n";
        } else {
            cout << "Nope! The correct word was '" << original << "'.\n\n";
        }
    }

    return 0;
}