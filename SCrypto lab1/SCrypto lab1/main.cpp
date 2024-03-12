#include <iostream>
#include "entropy.h"
#include <windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "");

    Tools tool(true); 
    tool.OpenFile("../input.txt");
    
    tool.ProcessTextFile();
    tool.CountLetters();
    tool.CalculateLetterProbabilities();

    // Output the results
    std::wcout << L"\nLetter Frequencies:\n";
    for (const auto& pair : tool.getLetterCounts()) { 
        std::wcout << pair.first << L": " << pair.second << L"\n";
    }

    std::wcout << L"\nLetter Probabilities:\n";
    for (const auto& pair : tool.getLetterProbabilities()) { 
        std::wcout << pair.first << L": " << pair.second << L"\n";
    }

    double entropy1 = tool.CalculateEntropyH1();
    std::wcout << L"\nEntropy: " << entropy1 << L"\n";

    tool.CountBigrams(1); 
    tool.CalculateBigramsProbabilities();

    std::wcout << L"\nBigram Frequencies:\n";
    for (const auto& pair : tool.getBigramsCounts()) {
        std::wcout << pair.first << L": " << pair.second << L"\n";
    }

    std::wcout << L"\nBigram Probabilities:\n";
    for (const auto& pair : tool.getBigramsProbabilities()) {
        std::wcout << pair.first << L": " << pair.second << L"\n";
    }

    double entropy2 = tool.CalculateEntropyH2();
    std::wcout << L"\nEntropy: " << entropy2 << L"\n";

    tool.CloseFile();

    return 0;
}