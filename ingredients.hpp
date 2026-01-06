#ifndef INGREDIENTS_HPP
#define INGREDIENTS_HPP

#include <string>
#include <vector>

using namespace std;

struct Note {
    string step;
    int octave;
    int duration;
    string type;
};

struct Measure {
    int number;
    int divisions = 4;
    int key = 0;
    int units = 4;
    int unit_value = 4;
    string sign;
    int sign_line;
    vector<Note> notes = {};
};

struct Part {
    string id;
    string name;
    vector<Measure> measures = {};
};

struct MusicData {
    string title = "Unknown Title";
    string composer = "Unknown Composer";
    vector<Part> parts = {};
};

#endif