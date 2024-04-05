#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <iterator>
#include <sstream>
#include <algorithm>

namespace csvlib {

class CSV {
public:
    CSV() {
        delimiter = ",";
        fieldnames = {};
    }

    CSV(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",") {
        this->open_file(filename);
        this->delimiter = delimiter;
        this->fieldnames = fieldnames;
    }

    CSV(const char* filename, std::string fielnames, const std::string& delimiter = ",") {
        this->open_file(filename);
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
    virtual void open_file(const char* filename) {};

    std::fstream file;
    std::string delimiter;
    std::vector<std::string> fieldnames;
};

class CSVReader : virtual CSV {
public:
    CSVReader() : CSV() {};

    CSVReader(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter ) {}

    CSVReader(const char* filename, std::string fieldnames, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {}

    void read_fieldnames() {
        auto fieldnames = this->get_next_line();

        if (!fieldnames.has_value())
            return;

        this->fieldnames = fieldnames.value();
    }

    std::optional<std::vector<std::string>> get_next_line() {
        std::string line;

        if(std::getline(file, line))
            return this->parse(line);
    }

    std::vector<std::vector<std::string>>& get_all_lines() {
        std::vector<std::vector<std::string>> result;
        std::string line;

        while(std::getline(file, line))
            result.push_back(this->parse(line));

        return result;
    }

protected:
    void open_file(const char* filename) override {
        file.open(filename, std::ios_base::in);
    }

    std::vector<std::string>& parse(std::string line) {
        auto pos = line.find(delimiter);
        std::vector<std::string> result;
        result.reserve(fieldnames.size());

        while (pos != std::string::npos) {
            result.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
            pos = line.find(delimiter);
        }

        if (!line.empty())
            result.push_back(line);

        return result;
    }
};

class CSVWriter : virtual CSV {
public:
    CSVWriter() : CSV() {};

    CSVWriter(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {}

    CSVWriter(const char* filename, std::string fieldnames, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {}

    void write_fieldnames() {
        this->write_line(this->fieldnames);
    }

    void write_line(const std::vector<std::string>& fields) {
        file << this->concantinate(fields) << std::endl;
    }

    void write_lines(const std::vector<std::vector<std::string>>& lines) {
        for(const auto& line : lines)
            this->write_line(line);
    }

protected:
    void open_file(const char* filename) override {
        file.open(filename, std::ios_base::out);
    }

    std::string& concantinate(const std::vector<std::string>& fields) {
        std::string result;
        std::ostringstream ss;
        std::copy(fields.begin(), fields.end(), std::ostream_iterator<std::string>(ss, delimiter.c_str()));
        return ss.str();
    }
};

class CSVReaderWriter : CSVReader, CSVWriter, virtual CSV {
public:
    CSVReaderWriter(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {}

    CSVReaderWriter(const char* filename, std::string fieldnames, const std::string& delimiter = ",") : CSV(filename, fieldnames, delimiter) {}

protected:
    void open_file(const char* filename) override {
        file.open(filename);
    }
};

}