#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <cctype> 
#include <locale>
#include <codecvt>
#include <map>
#include <regex>

class Tools {
private:
	std::wstring alphabet;
	std::wifstream inputFile;
	std::wofstream outputFile;
	std::string inputFileName;
	std::map<wchar_t, int> letterCounts;
	std::map<wchar_t, double> letterProbabilities;
	std::map<std::wstring, int> bigramCounts;
	std::map<std::wstring, double> bigramProbabilities;
	bool WithSpaces;

public:
	Tools(bool WithSpaces);
	void OpenFile(const std::string& inputFileName);
	void CloseFile();
	void ProcessTextFile();
	wchar_t ToLowerCase(wchar_t c);
	std::wstring ProcessLine(const std::wstring& line);
	void CountLetters();
	void CalculateLetterProbabilities();

	const std::map<wchar_t, int>& getLetterCounts() const {
		return letterCounts;
	}

	const std::map<wchar_t, double>& getLetterProbabilities() const {
		return letterProbabilities;
	}

	double CalculateEntropyH1() const;

	const std::map<std::wstring, int>& getBigramsCounts() const {
		return bigramCounts;
	}
	const std::map<std::wstring, double>& getBigramsProbabilities() const {
		return bigramProbabilities;
	}
	void CountBigrams(int step);
	void CalculateBigramsProbabilities();

	double CalculateEntropyH2() const;
};	

