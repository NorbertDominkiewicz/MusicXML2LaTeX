#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <fstream>
#include "ingredients.hpp"

using namespace std;

class Parser {
public:
    static MusicData parseXML(const string &fileName);
private:
    static vector<Part> extractParts(ifstream &xmlFile, string &line);
    static vector<Measure> extractMeasures(ifstream &xmlFile, string &line);
    static vector<Note> extractNotes(ifstream &xmlFile, string &line);
};

#endif