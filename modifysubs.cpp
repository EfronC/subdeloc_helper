#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <cstring>
#include <ass/ass.h>
#include <json/json.h> // Include the jsoncpp header file
using namespace std;

map<string, string> loadReplacementWords(const string& filename) {
    map<string, string> words;

    ifstream inputFile(filename);
    if (!inputFile) {
        throw runtime_error("Failed to open replacement words file");
    }

    Json::Value root;
    inputFile >> root;

    for (const auto& key : root.getMemberNames()) {
        string value = root[key].asString();
        words[key] = value;
    }

    return words;
}

string modifySubs(const string& subfile, const string& replacementFile) {
    try {
        // Initialize libass
        ASS_Library* assLibrary = ass_library_init();
        if (!assLibrary) {
            throw runtime_error("Failed to initialize libass");
        }

        // Load the SSA file
        char* subfilePtr = new char[subfile.length() + 1]; // Create a non-const char* variable
        strcpy(subfilePtr, subfile.c_str()); // Copy the contents of subfile to subfilePtr
        ASS_Track* track = ass_read_file(assLibrary, subfilePtr, nullptr);
        delete[] subfilePtr; // Release the allocated memory
        if (!track) {
            throw runtime_error("Failed to load SSA file");
        }

        // Define outfile name
        string nfilename = "[Delocalized] " + subfile;
        size_t lastDotIndex = nfilename.find_last_of('.');
        if (lastDotIndex != string::npos) {
            nfilename = nfilename.substr(0, lastDotIndex) + ".json";
        }

        map<string, string> WORDS = loadReplacementWords(replacementFile);

        // Modify the subtitle lines
        Json::Value jsonData;
        for (int i = 0; i < track->n_events; i++) {
            ASS_Event* event = track->events + i;
            string modifiedText = event->Text;
            for (const auto& word : WORDS) {
                modifiedText = regex_replace(modifiedText, regex(word.first, regex_constants::icase), word.second);
            }
            jsonData[to_string(i + 1)] = modifiedText;
        }

        // Save the modified subtitle to a JSON file
        ofstream outputFile(nfilename);
        if (!outputFile) {
            throw runtime_error("Failed to open output file");
        }

        Json::StreamWriterBuilder writerBuilder;
        writerBuilder["indentation"] = "  ";
        unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
        writer->write(jsonData, &outputFile);

        outputFile.close();

        // Clean up
        ass_free_track(track);
        ass_library_done(assLibrary);

        return nfilename;
    } catch (const exception& e) {
        cout << e.what() << endl;
        return "";
    }
}

int main() {
    string subfile = "Subtitle.ass";
    // modifySubs(subfile);
    return 0;
}