#include <regex>
#include <iostream>
#include "../headers/SimSetts.h"

#define STRING_FILE_PATH "(((?:(([^\\/\\s]*)|(\".*\"))\\/)*)((\".*\")|([^\\s]*)))"
#define NUMBER "([0-9]+)"

using std::string, std::regex, std::smatch, std::regex_search, std::cout, std::stoi;

bool parseAndExtract(const std::string& pattern, string* ret, string& line, int* grNum, int sz)
{
    const regex regexp(pattern);
    smatch match_result;

    bool success = regex_search(line, match_result, regexp);
    if (!success) {return false;}

    size_t start = match_result[0].first  - line.begin();
    size_t end   = match_result[0].second - line.begin();
    for (int i = 0; i < sz; i++) {
        ret[i] = string(match_result[*grNum].first, match_result[*grNum].second);
        grNum++;
    }

    line = string(line.begin(), line.begin()+start)+string(line.begin()+end, line.end());

    return true;
}


SimSetts parseArgs(const int argc, char* argv[])
{
    std::string all;
    for (int i = 1; i < argc; i++) {
        all += argv[i]; all += " ";
    }

    SimSetts settings{};
    std::string in_filename, out_filename, ticks, threads;
    int group = 1;


    parseAndExtract("--in-file="    STRING_FILE_PATH, &in_filename, all, &group, 1);
    parseAndExtract("--out-file="   STRING_FILE_PATH, &out_filename,all, &group, 1);
    parseAndExtract("--n-ticks="    NUMBER, &ticks,all, &group, 1);
    parseAndExtract("--n-threads="  NUMBER, &threads,all, &group, 1);

    settings.input_filename =  in_filename;
    settings.output_filename = out_filename;
    if (!ticks.empty())   settings.n_ticks = stoi(ticks);
    if (!threads.empty()) settings.n_threads = stoi(threads) - 1;
    settings.info.readFromFile(in_filename);

    return settings;
}
