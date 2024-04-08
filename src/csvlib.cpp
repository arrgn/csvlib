#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <iterator>
#include <sstream>
#include <algorithm>

namespace csvlib {

void split(std::string str, const std::string& delimiter, std::vector<std::string>& result) {
    auto pos = str.find(delimiter);

    while (pos != std::string::npos) {
        result.push_back(str.substr(0, pos));
        str.erase(0, pos + 1);
        pos = str.find(delimiter);
    }

    if (!str.empty())
        result.push_back(str);
}

std::string combine(const std::vector<std::string>& fields, const std::string& delimiter) {
    if (fields.empty())
        return "";

    std::string res = "";
    for (size_t i = 0; i < fields.size() - 1; i++)
        res += fields[i] + delimiter;
    res += fields[fields.size() - 1];

    return res;
}

class CSV {
public:
    CSV() {
        delimiter = ",";
        fieldnames = {};
    }

    CSV(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",") {
        this->delimiter = delimiter;
        this->fieldnames = fieldnames;
    }

    CSV(const char* filename, std::string fielnames, const std::string& delimiter = ",") {
        this->delimiter = delimiter;
        
        auto pos = fielnames.find(delimiter);

        while (pos != std::string::npos) {
            this->fieldnames.push_back(fielnames.substr(0, pos));
            fielnames.erase(0, pos + 1);
            pos = fielnames.find(delimiter);
        }
    }

    ~CSV() {
        file.close();
    }

protected:
    virtual void open_file(const char* filename) {}

    std::fstream file;
    std::string delimiter;
    std::vector<std::string> fieldnames;
};

class CSVReader : virtual CSV {
public:
    CSVReader() : CSV() {};

    CSVReader(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter ) {
        open_file(filename);
    }

    CSVReader(const char* filename, std::string fieldnames, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {
        open_file(filename);
    }

    bool read_fieldnames() {
        std::string line;

        if (!std::getline(file, line))
            return false;

        split(line, delimiter, fieldnames);
        return true;
    }

    std::optional<std::vector<std::string>> read_next_line() {
        std::string line;

        if (std::getline(file, line))
            return this->parse(line);
    }

    std::vector<std::vector<std::string>> read_all_lines() {
        std::vector<std::vector<std::string>> result;
        std::string line;

        while (std::getline(file, line))
            result.push_back(this->parse(line));

        return result;
    }

protected:
    void open_file(const char* filename) override {
        file.open(filename, std::ios_base::in);
    }

    std::vector<std::string> parse(std::string line) {
        std::vector<std::string> result;
        result.reserve(fieldnames.size());

        split(line, delimiter, result);

        return result;
    }
};

class CSVWriter : virtual CSV {
public:
    CSVWriter() : CSV() {};

    CSVWriter(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {
        open_file(filename);
    }

    CSVWriter(const char* filename, std::string fieldnames, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {
        open_file(filename);
    }

    void write_fieldnames() {
        this->write_line(this->fieldnames);
    }

    void write_line(const std::vector<std::string>& fields) {
        file << this->concatenate(fields) << std::endl;
    }

    void write_lines(const std::vector<std::vector<std::string>>& lines) {
        for(const auto& line : lines)
            this->write_line(line);
    }

protected:
    void open_file(const char* filename) override {
        file.open(filename, std::ios_base::out);
    }

    std::string concatenate(const std::vector<std::string>& fields) {
        return combine(fields, this->delimiter);
    }
};

class CSVReaderWriter : CSVReader, CSVWriter, virtual CSV {
public:
    CSVReaderWriter(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {
        open_file(filename);
    }

    CSVReaderWriter(const char* filename, std::string fieldnames, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {
        open_file(filename);
    }

protected:
    void open_file(const char* filename) override {
        file.open(filename, std::ios_base::in | std::ios_base::out);
    }
};

class CSVDictReader : virtual CSV {
public:
    CSVDictReader() : CSV() {}

    CSVDictReader(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {
        open_file(filename);
    }

    CSVDictReader(const char* filename, std::string fieldnames, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {
        open_file(filename);
    }

    bool read_fieldnames() {
        std::string line;

        if (!std::getline(file, line))
            return false;

        split(line, delimiter, fieldnames);
        return true;
    }

    std::optional<std::map<std::string, std::string>> read_next_line() {
        std::string line;

        if (std::getline(file, line))
            return this->parse(line);
    }

    std::vector<std::map<std::string, std::string>> read_all_lines() {
        std::vector<std::map<std::string, std::string>> result;
        std::string line;

        while (std::getline(file, line))
            result.push_back(this->parse(line));
        
        return result;
    }

protected:
    void open_file(const char* filename) override {
        file.open(filename, std::ios_base::in);
    }

    std::map<std::string, std::string> parse(std::string line) {
        std::map<std::string, std::string> result;
        
        line += delimiter; // add delimiter to end of line otherwise we'll have a parse error parsing the last key-value pair
        for (const auto& key : this->fieldnames) {
            auto pos = line.find(delimiter);
            result[key] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }

        return result;
    }
};

class CSVDictWriter : virtual CSV {
public:
    CSVDictWriter() : CSV() {}

    CSVDictWriter(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {
        open_file(filename);
    }

    CSVDictWriter(const char* filename, std::string fieldnames, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {
        open_file(filename);
    }

    void write_fieldnames() {
        file << combine(fieldnames, delimiter) << std::endl;
    }

    void write_line(const std::map<std::string, std::string>& data) {
        file << concatenate(data) << std::endl;
    }

    void write_lines(const std::vector<std::map<std::string, std::string>>& data) {
        for (const auto& line : data)
            this->write_line(line);
    }

protected:
    void open_file(const char* filename) override {
        file.open(filename, std::ios_base::out);
    }

    std::string concatenate(const std::map<std::string, std::string>& data) {
        std::string result;

        for (const auto& key : this->fieldnames) {
            result += data.at(key); // data is const so we can't invoke operator[] on it
            result += delimiter;
        }

        return result;
    }
};

class CSVDictReaderWriter : CSVDictReader, CSVDictWriter, virtual CSV {
public:
    CSVDictReaderWriter(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {
        open_file(filename);
    }

    CSVDictReaderWriter(const char* filename, std::string fieldnames, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {
        open_file(filename);
    }

protected:
    void open_file(const char* filename) override {
        file.open(filename, std::ios_base::in | std::ios_base::out);
    }
};

}