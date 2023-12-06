#include "StringUtils.h"
#include <cctype>

namespace nc
{
    unsigned int StringUtils::uniqueCounter = 0; //defines the static variable uniqueCounter to an unsigned int and = to 0

    string StringUtils::ToUpper(const string& input) {
        string result = input; //creates a local string variable names result using the input parameter
        for (char& u : result) { // starts a for loop to iterate through each character in the result string
            u = toupper(u); // sets u = to the converted u using the toupper function
        }
        return result; // returns the result
    }

    string StringUtils::ToLower(const string& input) {
        string result = input; //creates a local string variable names result using the input parameter
        for (char& l : result) { // starts a for loop to iterate through each character in the result string
            l = tolower(l); // sets l = to the converted l using the toupper function
        }
        return result; // returns the result
    }

    bool StringUtils::IsEqualIgnoreCase(const string& str1, const string& str2) {
        if (str1.length() != str2.length()) { // compares two strings by their length to see if they are not the same
            return false; // returns false if they are not the same
        }

        for (size_t i = 0; i < str1.length(); ++i) { // start of for loop that iterates though each character in string 1
            if (tolower(str1[i]) != tolower(str2[i])) { // if lowercase string 1 does not equal lowercase string 2
                return false;  //returns false if the two strings are not the same (case insensitive)
            }
        }

        return true; // returns true if strings are the same
    }

    string StringUtils::CreateUnique(const string& input) {
        string uniqueString = input + to_string(uniqueCounter++); // creates variable uniqueString and sets it equal to the input param + the uniqueCounter int from above to a string and iterates through each string inputed (++)
        return uniqueString; // returns the concatination from above
    }

    int StringUtils::GetTrailingNumber(const std::string& str)
    {
        // if the string is empty return -1
        if (str.empty()) return -1;
        // check from the end of the string for characters that are digits, add digit to strNumber
        // if character is not digit, then exit
        std::string strNumber;
        for (size_t i = str.size() - 1; i >= 0; i--)
        {
            if (std::isdigit(str[i])) strNumber = str[i] + strNumber;
            else break;
        }
        // convert strNumber to a number if not empty
        return (!strNumber.empty()) ? std::stoi(strNumber) : -1;
    }

    std::string StringUtils::RemoveTrailingNumber(const std::string& str)
    {
        if(str.empty()) return "";
        // set result string to str parameter
        // start at the end of the string and remove any characters that are digits
        // if character is not a digit, then exit
        std::string result = str;
        for (size_t i = str.size() - 1; i >= 0; i--)
        {
            if (std::isdigit(str[i])) result.pop_back();
            else break;
        }
        // return string without trailing number
        return result;
    }

    }

