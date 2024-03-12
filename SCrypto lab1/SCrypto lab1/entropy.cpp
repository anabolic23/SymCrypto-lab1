#include "entropy.h"
#include <iostream>

Tools::Tools(bool WithSpaces) {
    this->WithSpaces = WithSpaces;
	alphabet = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    if (WithSpaces) {
        alphabet += L' ';
    }
    for (auto c : alphabet) {
        letterCounts[c] = 0;
        letterProbabilities[c] = 0;
    }
}

void Tools::OpenFile(const std::string& inputFileName) {
    this->inputFileName = inputFileName; // Store input file name
    inputFile.open(this->inputFileName);

    inputFile.imbue(std::locale(inputFile.getloc(),
        new std::codecvt_utf8<wchar_t, 0x10ffff, std::little_endian>));


    if (!inputFile) {
        std::cerr << "Error: Unable to open the file '" << this->inputFileName << "'\n";
    }
    else {
        std::cout << "File " << this->inputFileName << " opened successfully!\n";
    }
}

void Tools::CloseFile() {
    inputFile.close();
    std::cout << "File closed successfully!\n";
}

wchar_t Tools::ToLowerCase(wchar_t c) {
    if (c >= L'А' && c <= L'Я') {
        return c + (L'а' - L'А');
    }
    
    else if (c == L'Ё') {
        return L'е'; // Special case for 'Ё'
    }
    
    return c;
}


std::wstring Tools::ProcessLine(const std::wstring& line) {
    std::wstring result;
    for (auto c : line) {
        c = ToLowerCase(c); // Convert to lowercase first

        if (c == L'ъ') {
            c = L'ь';
        }
        if (c == L'ё') {
            c = L'е';
        }

        // Check if the character is in the modified alphabet or it's a space
        if (alphabet.find(c) != std::wstring::npos) {
            result += c;
        }
    }
    if (this->WithSpaces) {
        std::wregex multipleSpaces(L"\\s+");
        result = std::regex_replace(result, multipleSpaces, L" ");
    }

    return result;
}

void Tools::ProcessTextFile() {
    if (this->WithSpaces) {
        outputFile.open("with_spaces.txt");
    }
    else {
        outputFile.open("without_spaces.txt");
    }
    outputFile.imbue(std::locale(inputFile.getloc(),
        new std::codecvt_utf8<wchar_t, 0x10ffff, std::little_endian>));
    if (!outputFile) {
        std::cerr << "Error: Unable to create the file 'output.txt'\n";
        return;
    }

    std::wstring line;
    while (std::getline(inputFile, line)) {
        std::wstring processedLine = ProcessLine(line);
        std::wcout << processedLine;
        outputFile << processedLine; 
        if (!(outputFile << processedLine)) {
            std::cerr << "Error writing to the output file\n";
            // Handle the error as needed
        }
    }
    inputFile.close();
    outputFile.close(); // Close the output file when done
}


void Tools::CountLetters() {
    if (this->WithSpaces) {
        inputFile.open("with_spaces.txt");
    }
    else {
        inputFile.open("without_spaces.txt"); 
    }
    inputFile.imbue(std::locale(inputFile.getloc(),
        new std::codecvt_utf8<wchar_t, 0x10ffff, std::little_endian>));
    if (!inputFile) {
        std::wcerr << "Error: Unable to create the file 'output.txt'\n";
        return;
    }

    std::wstring line;
    while (std::getline(inputFile, line)) {
        for (auto c : line) {
            c = ToLowerCase(c);
            if (alphabet.find(c) != std::wstring::npos) {
                letterCounts[c]++;
            }
        }
    }
    inputFile.close();
}

void Tools::CalculateLetterProbabilities() {
    long long totalLetters = 0;
    for (auto& pair : letterCounts) {
        totalLetters += pair.second;    
    }

    for (auto& pair : letterCounts) {
        letterProbabilities[pair.first] = static_cast<double>(pair.second) / totalLetters;
    }
}

double Tools::CalculateEntropyH1() const {
    double entropy1 = 0.0;
    for (const auto& pair : letterProbabilities) {
        double probability = pair.second;
        if (probability > 0) {
            entropy1 += probability * log2(probability);
        }
    }
    return -entropy1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tools::CountBigrams(int step) {
    if (step != 1 && step != 2) {
        std::wcerr << L"Invalid step. Use 1 for overlapping bigrams or 2 for non-overlapping bigrams.\n";
        return;
    }

    if (this->WithSpaces) {
        inputFile.open("with_spaces.txt");
    }
    else {
        inputFile.open("without_spaces.txt");
    }

    inputFile.imbue(std::locale(inputFile.getloc(),
        new std::codecvt_utf8<wchar_t, 0x10ffff, std::little_endian>));
    if (!inputFile) {
        std::wcerr << L"Error: Unable to open the file for reading bigrams.\n";
        return;
    }

    std::wstring line;
    while (std::getline(inputFile, line)) {
        for (size_t i = 0; i < line.length() - 1; i += step) {
            std::wstring bigram = line.substr(i, 2);
            if (bigram.length() == 2) { // Ensure it's a valid bigram
                bigramCounts[bigram]++;
            }
        }
    }

    inputFile.close();

    this->bigramCounts = bigramCounts;
}

void Tools::CalculateBigramsProbabilities() {
    long long totalBigrams = 0;
    for (const auto& pair : bigramCounts) {
        totalBigrams += pair.second;
    }

    for (const auto& pair : bigramCounts) {
        bigramProbabilities[pair.first] = static_cast<double>(pair.second) / totalBigrams;
    }
}

double Tools::CalculateEntropyH2() const {
    double entropy2 = 0.0;
    for (const auto& pair : bigramProbabilities) {
        double probability = pair.second;
        if (probability > 0) {
            entropy2 += (probability * log2(probability))/2;
        }
    }
    return -entropy2;
}