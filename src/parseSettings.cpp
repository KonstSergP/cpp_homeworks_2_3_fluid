#include <regex>
#include <iostream>
#include "../headers/parseSettings.h"

#define DOUBLE_T "DOUBLE"
#define FLOAT_T  "FLOAT"
#define FIXED_T  "FIXED\\(([1-6][0-9]|[1-9]),\\s*([1-6][0-9]|[1-9])\\)"
#define FAST_FIXED_T "FAST_" FIXED_T
#define STRING_TYPES "(" FAST_FIXED_T "|" FIXED_T "|" FLOAT_T "|" DOUBLE_T ")"

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


int getTypeFromName(std::string& name)
{
    if (name.size() == 0) {return 0;}
    if (name == FLOAT_T)  {return FLOAT;}
    if (name == DOUBLE_T) {return DOUBLE;}
    int groups[] = {1, 2}; string numbers[2];

    if (!parseAndExtract(FAST_FIXED_T, numbers, name, groups, 2)) {
        if (!parseAndExtract(FIXED_T, numbers, name, groups, 2)) {
            return 0;
        }
        return stoi(numbers[0])*100 + stoi(numbers[1]); // FIXED
    }
    return stoi(numbers[0])*10000 + stoi(numbers[1]);   // FAST_FIXED
}


SimulatorSettings parseArgs(const int argc, char* argv[])
{
    std::string all;
    for (int i = 1; i < argc; i++) {
        all += argv[i];
    }

    SimulatorSettings settings{};
    std::string p_type_s, v_type_s, vf_type_s; int group = 1;

    parseAndExtract("--p-type="  STRING_TYPES, &p_type_s,  all, &group, 1);
    parseAndExtract("--v-type="  STRING_TYPES, &v_type_s,  all, &group, 1);
    parseAndExtract("--vf-type=" STRING_TYPES, &vf_type_s, all, &group, 1);
    settings.p_type  = getTypeFromName(p_type_s);
    settings.v_type  = getTypeFromName(v_type_s);
    settings.vf_type = getTypeFromName(vf_type_s);

    return settings;
}