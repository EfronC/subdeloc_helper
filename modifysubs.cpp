#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <cstring>
#include <ass/ass.h>
#include <json/json.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

using InnerMostMap = unordered_map<string, vector<string>>;
using InnerMap = unordered_map<string, InnerMostMap>;
using OuterMap = unordered_map<string, InnerMap>;

size_t find_nth(const string& master, const string& word) {
    // Split the master string by commas
    size_t s = count(master.begin(), master.end(), ',');

    // Split the word into at most s parts
    vector<string> sep;
    string temp;
    stringstream ss(word);

    // Collect the first s-1 parts
    while (getline(ss, temp, ',') && sep.size() < s - 1) {
        sep.push_back(temp);
    }

    // Get the remaining part of the word (after the s-1 splits)
    if (ss.tellg() != -1) {
        sep.push_back(word.substr(ss.tellg()));
    } else {
        sep.push_back(""); // If tellg() is invalid, add an empty string
    }

    // If the split size is smaller than expected, return npos
    if (sep.size() <= s - 1) {
        return string::npos;
    }

    // Calculate nth position
    size_t nth = word.size() - sep.back().size();

    return nth;
}

void printMap(const OuterMap& cpp_dict) {
    for (const auto& outerPair : cpp_dict) {
        cout << "Outer Key: " << outerPair.first << endl;
        
        const InnerMap& innerMap = outerPair.second;
        for (const auto& innerPair : innerMap) {
            cout << "  Inner Key: " << innerPair.first << endl;
            
            const InnerMostMap& innerMostMap = innerPair.second;
            for (const auto& innerMostPair : innerMostMap) {
                cout << "    Inner Most Key: " << innerMostPair.first << endl;
                
                const vector<string>& vec = innerMostPair.second;
                cout << "      Values: ";
                for (const string& value : vec) {
                    cout << value << " ";
                }
                cout << endl;
            }
        }
    }
}

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

string overwriteSubs(const string& subfile, const string& replacementFile) {
    // Initialize libass
    ASS_Library* ass_library = ass_library_init();
    if (!ass_library) {
        throw runtime_error("Failed to initialize libass");
    }

    ASS_Renderer* ass_renderer = ass_renderer_init(ass_library);
    if (!ass_renderer) {
        cerr << "Failed to initialize ASS renderer." << endl;
        return "";
    }

    // Load the SSA file
    ifstream inputFile(subfile, ios::binary | ios::ate);
    if (!inputFile.is_open()) {
        cerr << "Could not open the input file: " << subfile << endl;
        ass_renderer_done(ass_renderer);
        return "";
    }
    
    streamsize size = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    string buffer(size, '\0');
    if (!inputFile.read(&buffer[0], size)) {
        cerr << "Error reading the input file." << endl;
        inputFile.close();
        ass_renderer_done(ass_renderer);
        return "";
    }
    inputFile.close();

    ASS_Track* track = ass_read_memory(ass_library, &buffer[0], buffer.size(), nullptr);
    if (!track) {
        cerr << "Failed to parse the ASS file." << endl;
        ass_renderer_done(ass_renderer);
        return "";
    }

    // Define outfile name
    string nfilename = "[Delocalized] " + subfile;
    size_t lastDotIndex = nfilename.find_last_of('.');
    if (lastDotIndex != string::npos) {
        nfilename = nfilename.substr(0, lastDotIndex) + ".ass";
    }

    map<string, string> WORDS = loadReplacementWords(replacementFile);

    // Build the new buffer with modifications
    ostringstream modifiedBuffer;

    // Separate the buffer into sections
    istringstream originalStream(buffer);
    string line;
    string master_format;
    bool inEventsSection = false;

    while (getline(originalStream, line)) {
        if (line.find("[Events]") != string::npos) {
            inEventsSection = true;
            modifiedBuffer << line << "\n";
            continue;
        }

        if (inEventsSection && line.find("Format:") != string::npos) {
            size_t pos = line.find("Format:");
            cout << pos << endl;
            master_format = line.substr(pos + 7);
            modifiedBuffer << line << "\n";
            continue;
        }

        if (inEventsSection && line.find("Dialogue:") != string::npos) {
            // Process dialogue lines
            size_t pos = line.find("Dialogue:");
            if (pos != string::npos) {
                string dialogueLine = line.substr(pos + 9); // Extract the dialogue part
                size_t textLinePos = find_nth(master_format, dialogueLine);
                if (textLinePos != string::npos) {
                    string modifiedText = dialogueLine.substr(textLinePos);
                    for (const auto& word : WORDS) {
                        modifiedText = regex_replace(modifiedText, regex(word.first, regex_constants::icase), word.second);
                    }
                    modifiedBuffer << "Dialogue:" << dialogueLine.substr(0, textLinePos) << modifiedText << "\n";
                    continue;
                }
            }
        } else if (line.find("[") != string::npos && line.find("[Events]") == string::npos) {
            // If we encounter another section start, stop processing events
            inEventsSection = false;
        }

        // Append original or modified line
        modifiedBuffer << line << "\n";
    }

    // Save the modified ASS file
    ofstream outputFile(nfilename);
    if (!outputFile.is_open()) {
        cerr << "Could not open the output file: " << nfilename << endl;
        ass_free_track(track);
        ass_renderer_done(ass_renderer);
        return "";
    }

    outputFile << modifiedBuffer.str();
    outputFile.close();

    // Clean up
    ass_free_track(track);
    ass_renderer_done(ass_renderer);
    ass_library_done(ass_library);

    return nfilename;
}

string find_key_by_string(const unordered_map<string, unordered_map<string, unordered_map<string, vector<string>>>>& dictionary,
                          const string& target_string,
                          const string& search_array) {
    auto it = dictionary.find("honorifics");
    if (it != dictionary.end()) {
        const auto& honorifics = it->second;
        for (const auto& pair : honorifics) {
            const auto& key = pair.first;
            const auto& nested_dict = pair.second;
            
            auto arr_it = nested_dict.find(search_array);
            if (arr_it != nested_dict.end()) {
                const auto& arr = arr_it->second;
                if (find(arr.begin(), arr.end(), target_string) != arr.end()) {
                    return key;
                }
            }
        }
    }
    return "";
}