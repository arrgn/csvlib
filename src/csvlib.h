#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <sstream>

namespace csvlib {

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
     */
    void read_fieldnames();

    /**
     * @brief Get the next line from csv file
     * 
     * @return one row as optional variable (vector of strings if successful)
     */
    std::optional<std::vector<std::string>> get_next_line();

    /**
     * @brief Get the all lines from csv file
     * 
     * @return all data as vector of vectors of strings (empty vector if no data)
     */
    std::vector<std::vector<std::string>>& get_all_lines();

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
     * @brief Write fielnames to csv file
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
    std::string& concantinate(const std::vector<std::string>& fields);
};

class CSVReaderWriter : CSVReader, CSVWriter, virtual CSV {
public:
    /**
     * @brief Construct a new CSVReader object
     * 
     * @param filename filename
     * @param fieldnames fieldnames in csv file (optional), vector of strings
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVReaderWriter(const char* filename, const std::vector<std::string>& fieldnames = {}, const std::string& delimiter = ",");

    /**
     * @brief Construct a new CSVReader object
     * 
     * @param filename filename
     * @param fielnames fieldnames in csv file as string with delimiters
     * @param delimiter delimiter in csv file, default is ","
     */
    CSVReaderWriter(const char* filename, std::string fieldnames, const std::string& delimiter = ",");

protected:
    /**
     * @brief Open csv file in read and writemode
     * 
     * @param filename filename
     */
    void open_file(const char* filename) override;
};

}