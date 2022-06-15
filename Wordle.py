import math

NUM_LETTERS = 5
NUM_GUESSES = 6
LENGTH_OF_WORDS = 5757

def generate_conditions(pattern_of_letters):
    conditions = []
   
    for i in range(NUM_LETTERS):
        if (pattern_of_letters[i] == "R"):
            conditions.append(letter_not_in_word(i))
        elif (pattern_of_letters[i] == "Y"):
            conditions.append(letter_in_word(i))
        elif (pattern_of_letters[i] == "G"):
            conditions.append(letters_match(i))
    return conditions

def letter_in_word(letter):
    return lambda this_word, word: this_word[letter] in word
def letter_not_in_word(letter):
    return lambda this_word, word: this_word[letter] not in word
def letters_match(letter):
    return lambda this_word, word: word[letter] == this_word[letter]


def sigmoid(x):

    if x >= 0:
        z = math.exp(-x)
        sig = 1 / (1 + z)
        return sig
    else:
        z = math.exp(x)
        sig = z / (1 + z)
        return sig

def read_words(): 
    with open("words_by_freq.txt", "r") as f:
        words = f.read().split()
    return words[::]
def pattern_can_occur(reduced_words, this_word, pattern):
    pass
def information_gained_times_likelyhood(reduced_words, this_word, pattern):
    size_before = len(reduced_words)
    words_with_pattern = 0
    for word in reduced_words:
        for letter in range(5):
            if pattern[letter] == "R":
                if this_word[letter] in word:
                    break
            elif pattern[letter] == "Y":
                if this_word[letter] not in word:
                    break
            elif pattern[letter] == "G":
                if this_word[letter] != word[letter]:
                    break
            if letter == 4:
                words_with_pattern += 1
        
    return 0 if words_with_pattern == 0 else\
    math.log(size_before/words_with_pattern, 2)*(words_with_pattern/size_before)

def expected_information(reduced_words, this_word):
    EV_info = 0
    for letter0 in ["R", "Y", "G"]:
        for letter1 in ["R", "Y", "G"]:
            for letter2 in ["R", "Y", "G"]:
                for letter3 in ["R", "Y", "G"]:
                    for letter4 in ["R", "Y", "G"]:
                        pattern = letter0 + letter1 + letter2 + letter3 + letter4
                        EV_info += information_gained_times_likelyhood(reduced_words, this_word, pattern)
    return EV_info

def suggest_word(words, reduced_words):
    max_info_word = ("", 0)
    for word in words:
        info = expected_information(reduced_words, word) 
        if info > max_info_word[1]:
            max_info_word = (word, info)
    return reduced_words if max_info_word[0] == "" else max_info_word[0]
#checks if there is two of one letter in a word
def is_repeat_letter(word):
    for i in range(5):
        for j in range(i+1, 5):
            if word[i] == word[j]:
                return True
    return False


def reduce_words(words, this_word, pattern):
    reduced_words = []
    for word in words:
        for i in range(5):      
            if pattern[i] == "R":
                if this_word[i] in word and this_word[i] not in this_word[i:]:
                    break
            elif pattern[i] == "Y":
                if this_word[i] not in word:
                    break
            elif pattern[i] == "G":
                if this_word[i] != word[i]:
                    break
            if i == 4:
                reduced_words += [word]
    return reduced_words

def main():
    words = read_words()
    reduced_words = words.copy()
    print("crane")
    guess = "crane"
    for i in range(NUM_GUESSES-1):
        input1 = input().upper()
        if input1 == "GGGGG":
            print("You win!")
            return
        reduced_words = reduce_words(reduced_words, guess, input1)
        guess = suggest_word(words, reduced_words)
      
        print(guess)
   

if __name__ == "__main__":
    main()
