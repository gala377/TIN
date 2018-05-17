//
// Created by koshachok on 5/16/18.
//

#include "FileReader.h"


void FileReader::write(const std::string file_name, Package *package) {
    std::ofstream file(file_name, std::ios_base::app);
    file << package->toStr() << std::endl;
    file.close();
}

void FileReader::remove(std::string file_name, Package *package) {
    std::ifstream file_in(file_name);
    std::ofstream file_out(TEMP_NAME);

    std::string line, line_to_delete(package->toStr());
    while (getline(file_in, line)) {
        auto some = line.find(line_to_delete);
        if (some < line.length())
            line.replace(some, line_to_delete.length(), "");
        file_out << line << std::endl;
    }

    file_in.close();
    file_out.close();
    std::remove(file_name.c_str());
    std::rename(TEMP_NAME, file_name.c_str());
}

std::list<Package *> FileReader::getAll(const std::string &file_name) {
    std::list<Package *> res;
    std::ifstream file(file_name);
    std::string line;

    while (getline(file, line)) {
        if (line[0]!=' ' && line[0]!='\n' && line[0]!='\0')
            res.push_back(new Package(line));
    }

    file.close();
    std::remove(file_name.c_str());

    return res;
}
