#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <sstream>

namespace csvlib {

/**
 * @brief Split a string based on a delimiter
 * 
 * @param str string to split
 * @param delimiter delimiter to split by
 * @param result vector of string to store result (writes to the end without clearing the vector)
 */
void split(std::string str, const std::string& delimiter, std::vector<std::string>& result);

/**
 * @brief Combime string based on delimiter
 * 
 * @param fields vector of string to combine
 * @param delimiter delimiter to combine with
 * @return combined string
 */
std::string combine(const std::vector<std::string>& fields, const std::string& delimiter);

/**
 * @brief CSV class to ingerit from (only constructor impleemented)
 * 
 */
class CSV {
public:
    /**
     * @brief Construct a new CSV object (plug)
     * 
     */
    CSV();

    /**
     * @brief Construct a new CSV object
     * 
     * @param filename filename
     * @param fieldnames fieldnames in csv file (optional), vector of strings
     * @param delimiter delimiter in csv file, default is ","
     */
    CSV(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",");

    /**
     * @brief Construct a new CSV object
     * 
     * @param filename filename
     * @param fielnames fieldnames in csv file as string with delimiters
     * @param delimiter delimiter in csv file, default is ","
     */
    CSV(const char* filename, std::string fielnames, const std::string& delimiter = ",");

    ~CSV();

protected:
    /**
     * @brief open csv file (not implemented)
     * 
     * @param filename filename
     */
    virtual void open_file(const char* filename) {};

    std::fstream file; // filename
    std::string delimiter; // delimiter in csv file, default is ","
    std::vector<std::string> fieldnames; // fieldnames in csv file, vector of strings, optional
};

/**
 * @brief CSV reader
 * 
 */
class CSVReader : virtual CSV {
public:
    /**
     * @brief Construct a new CSVReader object (plug)
     * 
     */
    CSVReader();

    /**
     * @brief Construct a new CSVReader object
     * 
     * @param filename filename
     * @param fieldnames fieldnames in csv file (optional), vector of strings
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVReader(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",");

    /**
     * @brief Construct a new CSVReader object
     * 
     * @param filename filename
     * @param fielnames fieldnames in csv file as string with delimiters
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVReader(const char* filename, std::string fieldnames, const std::string& delimiter = ",");

    /**
     * @brief Set fieldnames based on csv file row and delimiter (use before reading otherwise you got random data as fieldnames)
     * 
     * @return true - fieldnames are read
     * @return false - fieldnames are not read
     */
    void read_fieldnames();

    /**
     * @brief Get the next line from csv file
     * 
     * @return one row as optional variable (vector of strings if successful)
     */
    std::optional<std::vector<std::string>> read_next_line();

    /**
     * @brief Get the all lines from csv file
     * 
     * @return all data as vector of vectors of strings (empty vector if no data)
     */
    std::vector<std::vector<std::string>>& read_all_lines();

protected:
    /**
     * @brief Open csv file in read mode
     * 
     * @param filename filename
     */
    void open_file(const char* filename) override;

    /**
     * @brief Parse string with delimiter
     * 
     * @param line string to parse
     * @return parsed data as vector of strings
     */
    std::vector<std::string>& parse(std::string line);
};

/**
 * @brief CSV writer
 * 
 */
class CSVWriter : virtual CSV {
public:
    /**
     * @brief Construct a new CSVWriter object (plug)
     * 
     */
    CSVWriter();

    /**
     * @brief Construct a new CSVReader object
     * 
     * @param filename filename
     * @param fieldnames fieldnames in csv file (optional), vector of strings
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVWriter(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",");

    /**
     * @brief Construct a new CSVReader object
     * 
     * @param filename filename
     * @param fielnames fieldnames in csv file as string with delimiters
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVWriter(const char* filename, std::string fieldnames, const std::string& delimiter = ",");

    /**
     * @brief Write fielnames to csv file (invoke only with new opened file otherwise you will write this row in a radom place in the file)
     * 
     */
    void write_fieldnames();

    /**
     * @brief Write one row to csv file
     * 
     * @param fields data to write as vector of strings
     */
    void write_line(const std::vector<std::string>& fields);

    /**
     * @brief Write multiple rows to csv file
     * 
     * @param lines data to write as vector of vectors of strings
     */
    void write_lines(const std::vector<std::vector<std::string>>& lines);

protected:
    /**
     * @brief Open csv file in write mode
     * 
     * @param filename filename
     */
    void open_file(const char* filename) override;

    /**
     * @brief Concatenate fields with delimiter (to write to csv file then)
     * 
     * @param fields data to concantinate
     * @return concantinated with delimiter string
     */
    std::string& concatenate(const std::vector<std::string>& fields);
};

/**
 * @brief CSV reader and writer
 * 
 */
class CSVReaderWriter : CSVReader, CSVWriter, virtual CSV {
public:
    /**
     * @brief Construct a new CSVReaderWriter object
     * 
     * @param filename filename
     * @param fieldnames fieldnames in csv file (optional), vector of strings
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVReaderWriter(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",");

    /**
     * @brief Construct a new CSVReaderWriter object
     * 
     * @param filename filename
     * @param fielnames fieldnames in csv file as string with delimiters
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVReaderWriter(const char* filename, std::string fieldnames, const std::string& delimiter = ",");

protected:
    /**
     * @brief Open csv file in read and write mode
     * 
     * @param filename filename
     */
    void open_file(const char* filename) override;
};

/**
 * @brief CSV dictionary reader (map of strings (key - fieldname, value - fieldvalue))
 * 
 */
class CSVDictReader : virtual CSV {
public:
    /**
     * @brief Construct a new CSVDictReader object (plug)
     * 
     */
    CSVDictReader();

    /**
     * @brief Construct a new CSVDictReader object
     * 
     * @param filename filename
     * @param fieldnames fieldnames in csv file (optional), vector of strings
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVDictReader(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",");

    /**
     * @brief Construct a new CSVDictReader object
     * 
     * @param filename filename
     * @param fieldnames fieldnames in csv file as string with delimiters
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVDictReader(const char* filename, std::string fieldnames, const std::string& delimiter = ",");

    /**
     * @brief Set fieldnames based on csv file row and delimiter (use before reading otherwise you got random data as fieldnames)
     * 
     * @return true - fieldnames are read
     * @return false - fieldnames are not read
     */
    bool read_fieldnames();

    /**
     * @brief Get the next line from csv file
     * 
     * @return one row as optional variable (map of strings (key - fieldname, value - fieldvalue) if successful)
     */
    std::optional<std::map<std::string, std::string>> read_next_line();

    /**
     * @brief Get the all lines from csv file
     * 
     * @return all data as vector of maps of strings (key - fieldname, value - fieldvalue) (empty vector if no data)
     */
    std::vector<std::map<std::string, std::string>> read_all_lines();

protected:
    /**
     * @brief Open csv file in read mode
     * 
     * @param filename filename
     */
    void open_file(const char* filename) override;

    /**
     * @brief Parse string with delimiter
     * 
     * @param line string to parse
     * @return parsed data as map of strings (key - fieldname, value - fieldvalue)
     */
    std::map<std::string, std::string> parse(std::string line);
};

/**
 * @brief CSV dictionary writer (map of strings (key - fieldname, value - fieldvalue))
 * 
 */
class CSVDictWriter : virtual CSV {
public:
    /**
     * @brief Construct a new CSVDictWriter object (plug)
     * 
     */
    CSVDictWriter();

    /**
     * @brief Construct a new CSVDictReader object
     * 
     * @param filename filename
     * @param fieldnames fieldnames in csv file (optional), vector of strings
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVDictWriter(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",");

    /**
     * @brief Construct a new CSVDictReader object
     * 
     * @param filename filename
     * @param fielnames fieldnames in csv file as string with delimiters
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVDictWriter(const char* filename, std::string fieldnames, const std::string& delimiter = ",");

    /**
     * @brief Write fielnames to csv file (invoke only with new opened file otherwise you will write this row in a radom place in the file)
     * 
     */
    void write_fieldnames();

    /**
     * @brief Write one row to csv file
     * 
     * @param fields data to write as map of strings (key - fieldname, value - fieldvalue)
     */
    void write_line(const std::map<std::string, std::string>& data);

    /**
     * @brief Write multiple rows to csv file
     * 
     * @param lines data to write as vector of maps of strings (key - fieldname, value - fieldvalue)
     */
    void write_lines(const std::vector<std::map<std::string, std::string>>& data);

protected:
    /**
     * @brief Open csv file in write mode
     * 
     * @param filename filename
     */
    void open_file(const char* filename) override;

    /**
     * @brief Concatenate fields with delimiter (to write to csv file then)
     * 
     * @param fields data to concantinate
     * @return concantinated with delimiter string
     */
    std::string concatenate(const std::map<std::string, std::string>& data);
};

/**
 * @brief CSV dictionary reader and writer (maps of strings)
 * 
 */
class CSVDictReaderWriter : CSVDictReader, CSVDictWriter, virtual CSV {
public:
    /**
     * @brief Construct a new CSVDictReaderWriter object
     * 
     * @param filename filename
     * @param fieldnames fieldnames in csv file (optional), vector of strings
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVDictReaderWriter(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",");

    /**
     * @brief Construct a new CSVDictReaderWriter object
     * 
     * @param filename filename
     * @param fielnames fieldnames in csv file as string with delimiters
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVDictReaderWriter(const char* filename, std::string fieldnames, const std::string& delimiter = ",");

protected:
    /**
     * @brief Open csv file in read and write mode
     * 
     * @param filename filename
     */
    void open_file(const char* filename) override;
};

}