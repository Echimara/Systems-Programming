#include <iostream>
using namespace std;
#include <string>
#include <sstream>
#include <regex>

// Code Description: This program uses regex and concatentation to transform normal words into pig latin
// Author: Chimara Okeke

// Function prototype
string pigLatin(string word);

int main()
{
    // Stores original sentence
    string sentence;
    cout << "Enter an English sentence: " << endl;
    getline(cin, sentence);
    cout << "Original Sentence: ";

    // Print original sentence
    cout << sentence << endl;

    // Traverse through each word in sentence and transform each into pig latin
    string pigLatinSentence = "";
    stringstream ss(sentence);
    string word;
    while (ss >> word)
    {
        // extracts and stores last punctuation mark from "word"
        string last_punctuation = "";

        // loop to check if last char in "word" is punctuation
        // loop ends when all punctuation has been located
        while (!isalpha(word.back()) && !isdigit(word.back()))
        {
            // ensures that punctuation retains its original position
            last_punctuation = word.back() + last_punctuation;

            // removes punct from end of "word"
            word.pop_back();
        }

        string space = " ";
        pigLatinSentence += pigLatin(word) + last_punctuation + space;
    }

    // Print pig latin version of original sentence
    cout << "Pig Latin sentence: " << pigLatinSentence << endl;
    return 0;
}

// Function definition
string pigLatin(string word)
{
    // ensure pattern is case insensitive
    regex start_with_non_vowel("^[^aeiouAEIOU]");

    // If the word starts with a consonant
    if (regex_search(word, start_with_non_vowel))
    {
        // reserve 1st letter and its capitalization
        char first_letter = word[0];
        bool is_uppercase = isupper(first_letter);

        // remove 1st letter from beginning of word
        word.erase(0, 1); // new word

        // append 1st letter to end of the new word
        word += tolower(first_letter); // transposed word

        // append "ay" to end of transposed word
        word += "ay"; // final word in pig latin
        
        // capitalize first letter originally capitalized
        if (is_uppercase) {
            word[0] = toupper(word[0]);
        }
    }
    // Else, if the word starts with a vowel
    else
    {
        // append "-way" to end of the word. Some pit latin uses "-ay"
        word += "way";
    }

    return word;
}

