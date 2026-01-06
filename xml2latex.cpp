#include <iostream>
#include <string>
#include <fstream>
#include "parser.hpp"

using namespace std;

class Xml2Latex {
private:
    ofstream texFile;
    MusicData data;
public:
    Xml2Latex(const string &filename, const MusicData &data) : data(data) {
        texFile.open(filename);
        if (!texFile.is_open()) {
            printf("Error creating LaTeX file\n");
        }
    }

    ~Xml2Latex() {
        if (texFile.is_open()) {
            texFile.close();
        }
    }

    void addPreamble() {
        texFile << "\\documentclass{article}\n";
        texFile << "\\usepackage[T1]{fontenc}\n";
        texFile << "\\usepackage[utf8]{inputenc}\n";
        texFile << "\\usepackage[polish]{babel}\n";
        texFile << "\\usepackage{tikz}\n";
    }

    void addTitlePage() {
        texFile << "\n\\begin{titlepage}\n";
        texFile << "\\begin{center}\n";
        texFile << "\\Large MusicXML 2 LaTeX\\\\[0.7em]\n";
        texFile << "\\normalsize\\textsc{C++ Converter}\n";
        texFile << "\\end{center}\n";
        texFile << "\\vfill\n";
        texFile << "\\begin{flushright}\n";
        texFile << "Norbert Dominkiewicz \\\\ \n";
        texFile << "89273, grupa II\n";
        texFile << "\\end{flushright}\n";
        texFile << "\\end{titlepage}\n";
    }

    void addCompositionData() {
        texFile << "\\section{About the sheet}\n";
        texFile << "\\begin{itemize}\n";
        texFile << "\\item[Title:]" << data.title << "\n";
        texFile << "\\item[Composer:]" << data.composer << "\n";
        texFile << "\\item[Instruments:]";
        for (const auto &part : data.parts) {
            texFile << part.name << ", ";
        }
        texFile << "\n";
        texFile << "\\end{itemize}\n";
    }

    void addSheet() {
        for (const auto &part : data.parts) {
            texFile << "\\section{Part: " << part.name << "}\n";
            for (const auto &measure : part.measures) {
                drawMeasure(measure);
            }
        }
    }

    void drawMeasure(const Measure &measure) {
        texFile << "\\begin{tikzpicture}[scale=0.6]\n";
        texFile << "\\foreach \\y in {0,0.5,1,1.5,2} {\n";
        texFile << "\\draw[line width=1pt] (0, \\y) -- (8, \\y);\n";
        texFile << "}\n";
        int x = 1;
        int y = 1; 
        for (const auto &note : measure.notes) {
            drawNote(note, x, y);
            x += 1;
        }
        texFile << "\\end{tikzpicture}\n";
    }

    void drawNote(const Note &note, int x, int y) {
        texFile << "\\filldraw (" << x << ", " << y << ") circle (0.25);\n";
    }

    void generate() {
        addPreamble();
        texFile << "\n\\begin{document}\n";
        addTitlePage();
        texFile << "\n";
        addCompositionData();
        texFile << "\n";
        addSheet();
        texFile << "\\end{document}\n";
    }
};