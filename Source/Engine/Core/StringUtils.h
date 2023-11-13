#pragma once
#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
using namespace std;

namespace nc
{
    class StringUtils { //creates class
    public:
        static string ToUpper(const string& input);                               // declares a static member function named ToUpper that takes a const string input
        static string ToLower(const string& input);                               // declares a static member function named ToLower that takes a const string input
        static bool IsEqualIgnoreCase(const string& str1, const string& str2);    // declares a static bool function named IsEqualIgnoreCase that takes a cont string str1
        static string CreateUnique(const string& input);                          // declares a static member function named CreateUnique that takes a const string input

    private:
        static unsigned int uniqueCounter; // creates a static int variable names uniqueCounter
    };

    #endif // STRINGUTILS_H
}
