//
//  wordle-bot.c
//  Wordle-bot
//
//  Created by Vaishak Krishna on 2/9/22.
//
#include <stdio.h>
#include <math.h>
#include "wordle-bot.h"
#define NUM_LETTERS = 5;
#define NUM_GUESSES = 6;
#define LENGTH_OF_WORDS = 5757;
#define NUM_PATTERNS = 5757;

typedef struct Word_list {
    char **words;
    int num_words;
} Word_list;

char **create_all_patterns() {
    char **all_patterns = malloc(sizeof(char *) * NUM_PATTERNS);
    char[3] pattern_letters = {"R", "Y", "G"};
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            for (int k = 0; k < 2; k++){
                for (int l = 0; l < 2; l++){
                    for (int m = 0; m < 2; m++){
                        all_patterns[i + j + k + l + m] = malloc(sizeof(char) * (NUM_LETTERS + 1));
                        all_patterns[i + j + k + l + m][0] = pattern_letters[i];
                        all_patterns[i + j + k + l + m][1] = pattern_letters[j];
                        all_patterns[i + j + k + l + m][2] = pattern_letters[k];
                        all_patterns[i + j + k + l + m][3] = pattern_letters[l];
                        all_patterns[i + j + k + l + m][4] = pattern_letters[m];
                        all_patterns[i + j + k + l + m][5] = '\0';
                    }
                }
            }
        }
    }
}
char **read_words() {
    char[6] buff;
    FILE* fp = fopen("words_by_freq.txt", "r");
    for (int i = 0; i < LENGTH_OF_WORDS; i++) {
        fscanf(fp, "%s", buff);
        words[i] = malloc(6*sizeof(char));
        strcpy(words[i], buff);
    }
    fclose(fp);
    char** words = malloc(sizeof(char*) * LENGTH_OF_WORDS);
    return words;
}
int letter_in_word(char letter, char* word, char start_index) {
    for (int i = start_index; i < strlen(word); i++) {
        if (word[i] == letter) {
            return 1;
        }
    }
    return 0;
}

double info_gain_times_probability(char** reduced_words, int size_of_reduced_words, char * this_word, char * pattern) {
    double words_with_pattern = 0;
    for (int i = 0; i < size_of_reduced_words; i++) {
        char * word = reduced_words[i];
        for (int letter = 0; letter < NUM_LETTERS; letter++) {
            if (pattern[letter] == "R") {
                if (letter_in_word(this_word[letter], word, 0) {
                    break;
                }
            }
            else if (pattern[letter] == "Y") {
                if (!letter_in_word(this_word[letter], word){
                    break;
                }
            }
            else if (pattern[letter] == "G") {
                if (this_word[letter] != word[letter]){
                    break;
                }
            }
            if (letter == 4) {
                words_with_pattern++;
            }
        }
    }
    return (words_with_pattern == 0) ? 0 : (log(2)/log(size_of_reduced_words/words_with_pattern))*(words_with_pattern/size_of_reduced_words);

}

double expected_information(char **reduced_words, char *this_word) {
    double EV_info = 0;
    for (int i = 0; i < NUM_PATTERNS; i++) {
        EV_info += info_gain_times_probability(reduced_words, size_of_reduced_words, this_word, all_patterns[i]);
    }
    return EV_info;
}



char *suggest_word(char **words, char **reduced_words, int size_of_reduced_words) {
    double max_info = 0;
    char *max_word = 0;
    for (int i = 0; i < LENGTH_OF_WORDS; i++) {
        double info = expected_information(reduced_words, words[i]);
        if (info > max_info) {
            max_info = info;
            max_word = words[i];
        }
    }
    return max_word;
}

Word_list reduce_words(char **words, int size_of_words, char *this_word, char *pattern){
    int size_of_reduced_words = 0;
    char **reduced_words = malloc(sizeof(char*) * size_of_reduced_words);
    for (int i = 0; i < size_of_words; i++) {
        char *word = words[i];
        for (int letter = 0; letter < NUM_LETTERS; letter++) {
            if (pattern[letter] == "R") {
                if (letter_in_word(this_word[letter], word, 0) {
                    break;
                }
            }
            else if (pattern[letter] == "Y") {
                if (!letter_in_word(this_word[letter], word){
                    break;
                }
            }
            else if (pattern[letter] == "G") {
                if (this_word[letter] != word[letter]){
                    break;
                }
            }
            if (letter == 4) {
                reduced_words[size_of_reduced_words] = malloc(sizeof(char) * (NUM_LETTERS + 1));
                strcpy(reduced_words[size_of_reduced_words], word);
                size_of_reduced_words++;
            }
        }
    }
    free(words);
    Word_list w; 
    w = {reduced_words, size_of_reduced_words};
    return w;
}
void main(){
    char **words = read_words();
    char **reduced_words = read_words();
    char *guess = "crane";
    int size_of_reduced_words = LENGTH_OF_WORDS;
    for (int i = 0; i < NUM_GUESSES; i++){
        char *input = scanf("%s",input);
        if (input[0] == "G" && input[1] == "G" && input[2] == "G" && input[3] == "G" && input[4] == "G"){
            return;
        }
        char **reduced_words = reduce_words(reduced_words, size_of_reduced_words, guess, input);
        char *suggested_word = suggest_word(words, reduced_words, size_of_reduced_words);
        printf("%s", suggested_word);
    }
    
    
}