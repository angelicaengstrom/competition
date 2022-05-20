//
// Created by angel on 2022-05-20.
//

#ifndef COMPETITION_TRIE_H
#define COMPETITION_TRIE_H

#include <string>

struct TrieNode{
    TrieNode *child[256];
    //om ordetr tar slut i denna nod
    bool end_of_string;
    // Character stored in this node - behövs inte
    char letter;
    TrieNode(){
        end_of_string = false;
        for (int i = 0; i < 256; i++) {
            child[i] = NULL;
        }
        letter = '\0';
    }
};

struct Trie{
    TrieNode root;

    void Insert (std::string str) {
        TrieNode * current = &root;
        for (size_t i = 0; i < str.size(); i++) {
            if (current->child[str.at(i)] == NULL) {
                current->child[str.at(i)] = new TrieNode;
                current->child[str.at(i)]->letter = str.at(i);
            }
            current = current->child[str.at(i)];
        }
        current->end_of_string = true;
    }

    TrieNode * Search (std::string str) {
        TrieNode * current = &root;
        for (size_t i = 0; i < str.size(); i++) {
            if (current->child[str.at(i)]) {
                current = current->child[str.at(i)];
            } else {
                current = NULL;
                break;
            }
        }
        return current;
    }

    // Print the words with the specified prefix in lexical order
    void PrintLexical (TrieNode * current, std::string prefix, std::string suffix) {
        if (current->end_of_string and suffix.size() != 0) {
            std::ofstream out(R"(C:\Users\angel\OneDrive\Dokument\Skola\Data\sortcomp\text\result.txt)", std::ios::app);
            out << prefix + suffix << " ";
            out.close();
        }
        for (int i=0; i<256; i++) {
            std::string temp = suffix;
            if (current->child[i]) {
                temp += current->child[i]->letter;
                PrintLexical(current->child[i], prefix, temp);
            }
        }
    }
};
#endif //COMPETITION_TRIE_H
