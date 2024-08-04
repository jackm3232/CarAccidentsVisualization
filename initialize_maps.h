#include "ordered_map.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>

//will later pass in map and hashmap as parameters
std::vector<float> initialize_maps(const std::string &filename, OrderedMap<std::string, OrderedMap<std::string,
                     OrderedMap<std::vector<std::string>, std::vector<std::string>>>>& mapStruct,
                     std::unordered_map<std::string, std::unordered_map<std::string,
                     std::unordered_map<std::string,std::vector<std::string>>>>& hashMapStruct){

    std::vector<float> times;
    std::chrono::duration<float> mapTime;
    std::chrono::duration<float> hashMapTime;

    std::ifstream data_file(filename);

    if (!data_file.is_open()) {
        std::cerr << "Cannot open " << filename << "!" << std::endl;
        return times;
    }

    std::string line;
    std::getline(data_file, line);


    std::chrono::duration<float> mapDuration;
    std::chrono::duration<float> hashMapDuration;


    while (std::getline(data_file, line)) {
        std::stringstream line_stream(line);
        std::string section;
        std::vector<std::string> sections_of_line;

        while (std::getline(line_stream, section, ',')) {
            sections_of_line.push_back(section);
        }

        std::string entire_date = sections_of_line[1];
        std::string year = entire_date.substr(0, 4);

        std::string month = entire_date.substr(5, 2);
        if (std::stoi(month) < 10) {
            month = month[1];
        }

        std::string day = entire_date.substr(8, 2);
        if (std::stoi(day) < 10) {
            day = day[1];
        }

        std::string time = entire_date.substr(11, 5);
        int hour = std::stoi(time.substr(0, 2));
        if (hour == 12) {
            time = "- " + std::to_string(12) + time.substr(2, 3) + " PM";
        }
        else if (hour == 0) {
            time = "- " + std::to_string(12) + time.substr(2, 3) + " AM";
        }
        else if (hour > 12) {
            hour = hour - 12;
            time = "- " + std::to_string(hour) + time.substr(2, 3) + " PM";
        }
        else {
            time = "- " + std::to_string(hour) + time.substr(2, 3) + " AM";
        }

        std::string formatted_date = month + "/" + day + "/" + year + " " + time;

        std::vector<std::string> coords;
        coords.push_back(sections_of_line[2]);
        coords.push_back(sections_of_line[3]);

        std::vector<std::string> accident_info;
        accident_info.push_back(formatted_date);
        accident_info.push_back(sections_of_line[4]);
        accident_info.push_back(sections_of_line[5]);
        accident_info.push_back(sections_of_line[6]);

        auto mapTimeStart = std::chrono::high_resolution_clock::now();
        mapStruct[sections_of_line[0]][month][coords] = accident_info;
        auto mapTimeEnd = std::chrono::high_resolution_clock::now();

        mapDuration = std::chrono::duration<float, std::milli>(mapDuration + mapTimeEnd - mapTimeStart);

        auto hashMapTimeStart = std::chrono::high_resolution_clock::now();
        hashMapStruct[sections_of_line[0]][month][coords[0] + "x" + coords[1]] = accident_info;
        auto hashMapTimeEnd = std::chrono::high_resolution_clock::now();


        hashMapDuration = std::chrono::duration<float, std::milli>(hashMapDuration + hashMapTimeEnd - hashMapTimeStart);
    }

    data_file.close();
    times.push_back(mapDuration.count());
    times.push_back(hashMapDuration.count());

    return times;
}
