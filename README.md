# Spell Checker

Complete implementation of SpellChecker class.
Provide functionality:
 * ability to select underlined container: vector, set, unordered_set based on ContainerType value (constructor parameter)
 * load contents of dictionary to memory 
 * detect if word is valid (i.e. determine if word should be skipped during spell checking or checked against dictionary)
 * check if word exist in dictionary 

Notes:
 * dictionary is assumed to be a file containing a list of lowercase words, one per line;
 * from top to bottom, the file is sorted lexicographically, with only one word per line (each of which ends with \n). No word is longer than 45 characters, and no word appears more than once.

Word is considered invalid if:
 * exceeds maximum length for a word
 * has non alpha characters, but leading apostrophe is allowed


The list of files which are allowed to be modified:

project/spell_check.cpp
project/spell_check.h


