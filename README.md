# Word_Game_C

A simple game implemented in C. The computer selects two words from a dictionary. The player has to find a path to get from the one word to the other while in each turn changing only one letter. In each turn the user can select all the words that are in the dictionary and differ from the word of the previous turn by one letter. The user has specific number of available turns to reach the destination.

The program uses a list to save the words in the current path, a list to save all the words of a specific length, a list to save all the available words for selection in the next turn. All those lists are contents of a struct.

More information in project2014.pdf

Test cases can be found in project2014files

How to run:
1. Compile
2. ./project LENGTH STEPS DICTIONARY

