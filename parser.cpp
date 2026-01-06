#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "parser.hpp"

using namespace std;

MusicData Parser::parseXML(const string &filename) {
    MusicData data;

    ifstream xmlFile(filename);
    if (!xmlFile.is_open()) {
        printf("Could not open the file %s\n", filename.c_str());
        return data;
    }

    string line;
    while (getline(xmlFile, line)) {
        int titlePos = line.find("<work-title>");
        if (titlePos != -1) {
            int start = line.find(">") + 1;
            int end = line.find("</");
            if (end != -1 && end > start) {
                data.title = line.substr(start, end - start);
            }
        }
        
        if (line.find("creator") != -1 && line.find("composer") != -1) {
            int start = line.find(">") + 1;
            int end = line.find("</", start);
            if (end != -1 && end > start) {
                data.composer = line.substr(start, end - start);
            }
        }

        if (line.find("<part-list>") != -1) {
            data.parts = extractParts(xmlFile, line);
        }

        if (line.find("<part ") != -1) {
            for (Part &part : data.parts) {
                if (line.find("id=\"" + part.id + "\"") != -1) {
                    part.measures = extractMeasures(xmlFile, line);
                    break;
                }
            }
        }
    }
    xmlFile.close();
    return data;
}

vector<Part> Parser::extractParts(ifstream &xmlFile, string &line) {
    vector<Part> parts;
    
    while (getline(xmlFile, line)) {
        if (line.find("</part-list>") != -1) {
            break;
        }
        
        if (line.find("<score-part") != -1) {
            Part part;
            int idStart = line.find("id=\"");
            
            if (idStart != -1) {
                idStart += 4;
                int idEnd = line.find("\"", idStart);
                if (idEnd != -1) {
                    part.id = line.substr(idStart, idEnd - idStart);
                }
            }
            while (getline(xmlFile, line)) {
                if (line.find("<part-name>") != -1) {
                    int nameStart = line.find(">") + 1;
                    int nameEnd = line.find("</part-name>");
                    if (nameEnd != -1) {
                        part.name = line.substr(nameStart, nameEnd - nameStart);
                    }
                    break;
                }
            }
            
            parts.push_back(part);
        }
    }
    
    return parts;
}

vector<Measure> Parser::extractMeasures(ifstream &xmlFile, string &line) {
    vector<Measure> measures;
    while (getline(xmlFile, line)) {
        if (line.find("</part>") != -1) {
            break;
        }
        
        if (line.find("<measure") != -1) {
            Measure measure;
            
            int numberStart = line.find("number=\"");
            if (numberStart != -1) {
                numberStart += 8;
                int numberEnd = line.find("\"", numberStart);
                if (numberEnd != -1) {
                    string numberStr = line.substr(numberStart, numberEnd - numberStart);
                    measure.number = stoi(numberStr);
                }
            }
            measures.push_back(measure);
        }

        if (line.find("<note>") != -1) {
            vector<Note> notes = extractNotes(xmlFile, line);
            if (!measures.empty()) {
                measures.back().notes.insert(measures.back().notes.end(), notes.begin(), notes.end());
            }
        }
    }
    return measures;
}

vector<Note> Parser::extractNotes(ifstream &xmlFile, string &line) {
    vector<Note> notes;
    while (getline(xmlFile, line)) {
        if (line.find("</note>") != -1) {
            Note note;
            while (getline(xmlFile, line)) {
                if (line.find("<step>") != -1) {
                    int start = line.find(">") + 1;
                    int end = line.find("</step>");
                    if (end != -1) {
                        note.step = line.substr(start, end - start);
                    }
                } else if (line.find("<octave>") != -1) {
                    int start = line.find(">") + 1;
                    int end = line.find("</octave>");
                    if (end != -1) {
                        note.octave = stoi(line.substr(start, end - start));
                    }
                } else if (line.find("<duration>") != -1) {
                    int start = line.find(">") + 1;
                    int end = line.find("</duration>");
                    if (end != -1) {
                        note.duration = stoi(line.substr(start, end - start));
                    }
                } else if (line.find("<type>") != -1) {
                    int start = line.find(">") + 1;
                    int end = line.find("</type>");
                    if (end != -1) {
                        note.type = line.substr(start, end - start);
                    }
                } else if (line.find("</note>") != -1) {
                    break;
                }
            }
            notes.push_back(note);
        } else if (line.find("</measure>") != -1) {
            break;
        }
    }
    return notes;
}