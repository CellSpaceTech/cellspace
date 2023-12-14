
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <algorithm>


#include "BST.cpp"
#include <regex>
using namespace std;


void searchKeyWord(string user_input);
vector<string> split(string s, string delim);
LinkedValues getResult(string token);
LinkedValues getResult(string token1, string token2);
LinkedValues getResult(string token1, string token2, string token3);
LinkedValues getResult(string token1, string token2, string token3, string token4);
bool isPOS(string token);

unordered_map<string, LinkedValues> umap;
int main(int argc, char* argv[])
{
	int keywords = 0, definitions = 0, counter = 1;

	string filePath = "./Data.CS.SFSU.txt";
	cout << "! Opening data file..." << filePath << endl;
	ifstream infile(filePath);
	//Read data to bst
	if (infile.is_open()) {
		//Read data to bst
		cout << "!loading data...." << endl;
		string line;
		string keyDelimiter = "|", definitionDelimiter = "-=>>";

		while (getline(infile, line)) {
			string key, pos, def, tokens;
			LinkedValues values;
			vector<string> firstPass, secondPass;
			firstPass = split(line, keyDelimiter);
			int i = 0, k;
			for (auto it : firstPass) {
				if (i == 0) {
					//This is the Keyword
					key = it;
					keywords++;
				}
				else {

					secondPass = split(it, definitionDelimiter);
					//This is the part of  speech
					pos = secondPass[0];
					pos = std::regex_replace(pos, std::regex(" +$"), "");
					//this is the  definition
					def = secondPass[1];
					for (k = 2; k < secondPass.size(); k++)
						def.append(secondPass[k]);

					values.insert(pos, def);
					definitions++;
				}
				i++;
			}

			umap[key] = values;
		}
		cout << "!loading completed...." << endl;
		cout << "! Closing data file.... " << filePath << endl;
		infile.close();
	}
	else {
		cout << "<!>ERROR<!> ===> File could not be opened. " << endl;
		if (filePath != "/Data.CS.SFSU.txt") {
			cout << "<!>ERROR<!> ===> Provided file path: " << filePath << endl;
			cout << "<!>Enter the CORRECT data file path: ./Data.CS.SFSU.txt" << endl;
		}
	}


	//print input prompt
	cout << endl;
	cout << "========Dictionary 340 C++========" << endl;
	cout << "--------Keywords: " << keywords << endl;
	cout << "--------Definitions: " << definitions << endl;
	cout << endl;

	//Get the user input
	string userInput;
	cout << "Search [" << counter << "]: ";
	getline(cin, userInput);
	while (userInput != "!Q") {
		cout << "\t|" << endl;
		if (userInput == "!help" || userInput.empty()) {
			cout << "PARAMETER HOW-TO, please enter:\n\t"
				<< "1. A search key - then 2. An optional part of speech - then\n\t"
				<< "3. An optional 'distinct' - then 4. An optional 'reverse'" << endl;
		}
		else {
			searchKeyWord(userInput);
		}
		counter++;
		cout << "\t| " << endl;
		cout << "Search [" << counter << "]: ";
		getline(cin, userInput);
	}
	return 0;
}

void searchKeyWord(string user_input) {
	vector<string> tokens;
	transform(user_input.begin(), user_input.end(), user_input.begin(), ::tolower);
	tokens = split(user_input, " ");
	LinkedValues values;
	switch (tokens.size())
	{
	case 1:
		//One parameter
		values = getResult(tokens[0]);
		values.printAll(tokens[0]);
		break;
	case 2:
		//One parameterss
		values = getResult(tokens[0], tokens[1]);
		values.printAll(tokens[0]);
		break;
	case 3:
		//Three parameters
		values = getResult(tokens[0], tokens[1], tokens[2]);
		values.printAll(tokens[0]);
		break;
	case 4:
		//Four  parameters.
		values = getResult(tokens[0], tokens[1], tokens[2], tokens[3]);
		values.printAll(tokens[0]);
		break;
	default:
		cout << "PARAMETER HOW-TO, please enter:\n\t"
			<< "1. A search key - then 2. An optional part of speech - then\n\t"
			<< "3. An optional 'distinct' - then 4. An optional 'reverse'" << endl;
		break;
	}
}

vector<string> split(string s, string delim) {
	size_t start = 0, end, del_len = delim.length();
	string token;
	vector<string> result;

	while ((end = s.find(delim, start)) != string::npos) {
		token = s.substr(start, end - start);
		start = end + del_len;
		result.push_back(token);
	}

	result.push_back(s.substr(start));
	return result;
}

LinkedValues getResult(string token) {
	LinkedValues valueFound;
	try
	{
		valueFound = umap.at(token);
		valueFound = valueFound.getOrderedValues();
	}
	catch (const out_of_range& e)
	{
		cerr << "\t<NOT FOUND> To be considered for the next release. Thank you.\n\t|" << endl;
		cout << "\t|\n\tPARAMETER HOW-TO, please enter:\n\t"
			<< "1. A search key - then 2. An optional part of speech - then\n\t"
			<< "3. An optional 'distinct' - then 4. An optional 'reverse'" << endl;
	}
	return valueFound;
}

LinkedValues getResult(string token1, string token2) {
	LinkedValues result = getResult(token1);
	if (result.getHead()) {
		if (isPOS(token2)) {
			result = result.getValueByPoS(token2);
			if (result.getHead())
				result = result.getOrderedValues();
			else {
				cout << "\t<NOT FOUND> To be considered for the next release. Thank you.\n\t|" << endl;
				cout << "\t|\n\tPARAMETER HOW-TO, please enter:\n\t"
					<< "1. A search key - then 2. An optional part of speech - then\n\t"
					<< "3. An optional 'distinct' - then 4. An optional 'reverse'" << endl;
			}

		}
		else if (token2 == "distinct") {
			result = result.getDistinctValues();
		}
		else if (token2 == "reverse") {

			result = result.getReverseValues();
		}
		else {
			cout << "\t|" << endl;
			cout << "\t<The entered 2nd parameter '" << token2 << "' is NOT a part of speech.>";
			cout << "\t<The entered 2nd parameter '" << token2 << "' is NOT 'distinct'.>" << endl;
			cout << "\t<The entered 2nd parameter '" << token2 << "' is NOT 'reverse'.>" << endl;
			cout << "\t<The entered 2nd parameter '" << token2 << "' was disregarded.>" << endl;
			cout << "\t<The 2nd parameter should be a part of speech or 'distinct' or 'reverse'.>" << endl;
			cout << "\t|" << endl;
		}
	}
	return result;
}

LinkedValues getResult(string token1, string token2, string token3)
{
	LinkedValues result = getResult(token1, token2);
	if (result.getHead()) {
		if (token3 == "distinct") {

			result = result.getDistinctValues();
		}
		else if (token3 == "reverse") {
			result = result.getReverseValues();
		}
		else {
			cout << "\t|" << endl;
			cout << "\t<The entered 3rd parameter '" << token3 << "' is NOT 'distinct'.>" << endl;
			cout << "\t<The entered 3rd parameter '" << token3 << "' is NOT 'reverse'.>" << endl;
			cout << "\t<The entered 3rd parameter '" << token3 << "' was disregarded.>" << endl;
			cout << "\t<The 3rd parameter should be 'distinct' or 'reverse'.>" << endl;
			cout << "\t|" << endl;
		}
	}
	return result;
}

LinkedValues getResult(string token1, string token2, string token3, string token4)
{
	LinkedValues result = getResult(token1, token2, token3);
	if (result.getHead()) {
		if (token4 == "reverse") {
			result = result.getReverseValues();
		}
		else {
			cout << "\t|" << endl;
			cout << "\t<The entered 4th parameter '" << token4 << "' is NOT 'reverse'.>" << endl;
			cout << "\t<The entered 4th parameter '" << token4 << "' was disregarded.>" << endl;
			cout << "\t<The 4th parameter should be 'reverse'.>" << endl;
			cout << "\t|" << endl;
		}
	}
	return result;
}
bool isPOS(string token) {

	return (token == "noun" || token == "adverb" || token == "adjective" || token == "verb" ||
		token == "pronoun" || token == "conjuction" || token == "interjection" || token == "preposition");
}