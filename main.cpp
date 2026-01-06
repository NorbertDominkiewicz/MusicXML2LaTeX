#include <string>
#include <iostream>
#include "parser.hpp"
#include "xml2latex.cpp"

using namespace std;

int main() {
    cout << "MusicXML2LaTeX app started" << endl;

    MusicData musicData = Parser::parseXML("sheet.musicxml");
    Xml2Latex xml2latex("sheet.tex", musicData);

    xml2latex.generate();
    cout << "LaTeX generated" << endl;
    return 0;
}