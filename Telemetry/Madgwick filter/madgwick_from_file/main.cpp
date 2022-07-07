#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cstdint>
#include <regex>
#include <cassert>
#include <fstream>
#include <iterator>

#include "madgwick.hpp"

struct Record {
    double time;
    double acc[3];
    double mag[3];
    double gyr[3];
};


class line : public std::string {};

std::istream &operator>>(std::istream &is, line &l)
{
    std::getline(is, l);
    return is;
}

const std::regex reg_parse(R"((.*);(.*);(.*);(.*);(.*);(.*);(.*);(.*);(.*);(.*))");
bool parse_line(const std::string& line, Record& record) {
    std::smatch m;
    if (std::regex_search(line, m, reg_parse)) {
        assert(m.size() == 11);
        record.time   = std::stod(m[1]);
        record.acc[0] = std::stod(m[2]);
        record.acc[1] = std::stod(m[3]);
        record.acc[2] = std::stod(m[4]);
        record.gyr[0] = std::stod(m[5]);
        record.gyr[1] = std::stod(m[6]);
        record.gyr[2] = std::stod(m[7]);
        record.mag[0] = std::stod(m[8]);
        record.mag[1] = std::stod(m[9]);
        record.mag[2] = std::stod(m[10]);
    }
    return true;
}



bool parse_line2(const std::string& line, Record& record) {
    size_t count = 0;
    char const* ptr = line.data();

    record.time   = std::stod(ptr, &count);
    ptr += count + 1;
    record.acc[0] = std::stod(ptr, &count);
    ptr += count + 1;
    record.acc[1] = std::stod(ptr, &count);
    ptr += count + 1;
    record.acc[2] = std::stod(ptr, &count);
    ptr += count + 1;
    record.gyr[0] = std::stod(ptr, &count);
    ptr += count + 1;
    record.gyr[1] = std::stod(ptr, &count);
    ptr += count + 1;
    record.gyr[2] = std::stod(ptr, &count);
    ptr += count + 1;
    record.mag[0] = std::stod(ptr, &count);
    ptr += count + 1;
    record.mag[1] = std::stod(ptr, &count);
    ptr += count + 1;
    record.mag[2] = std::stod(ptr, &count);
    return true;
}

int main(int argc, char * argv[]) {
    std::cout << "Use: (1) input file name (2) output file name (3) beta" << std::endl;
    using std::istream_iterator;
    //assert(argc > 3);

    std::ifstream inputFile("..\\..\\..\\madgwick_from_file\\inp.csv");
    std::ofstream outputFile("..\\..\\..\\madgwick_from_file\\res.csv");
    float beta = 0.3;

    istream_iterator<line> begin(inputFile);
    istream_iterator<line> end;
    float last_time = 0;
    int count = 0;
    for(istream_iterator<line> it = begin; it != end; ++it) {
        if (count % 10 == 0) {
            std::cout << count++ << std::endl;
        }
        Record record;
        parse_line2(*it, record);    
        float q[4];
        MadgwickAHRSupdate(q, 
                           record.gyr[0], record.gyr[1], record.gyr[2], 
                           record.acc[0], record.acc[1], record.acc[2],
                           record.mag[0], record.mag[1], record.mag[2], 
                           record.time - last_time, 
                           beta);
        outputFile << q[0] << ';' << q[1] << ';' << q[2] << ';' << q[3] << std::endl;
        last_time = record.time;
    }

}