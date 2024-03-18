#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
//create a function which checks for the pattern matching
bool isMatch(string s, string p) {
    int m = s.length();
    int n = p.length();
    //create a 2D vector to store the values
    vector<vector<bool>> dp(m+1, vector<bool>(n+1, false));
    //set the first value to true
    dp[0][0] = true;
    //check for the pattern matching
    for(int i = 1; i <= n; i++){
        if(p[i-1] == '*'){
            dp[0][i] = dp[0][i-1];
        }
    }
    //check for the pattern matching
    for(int i = 1; i <= m; i++){
        for(int j = 1; j <= n; j++){
            if(p[j-1] == '*'){
                dp[i][j] = dp[i-1][j] || dp[i][j-1];
            }
            else if(p[j-1] == '?' || s[i-1] == p[j-1]){
                dp[i][j] = dp[i-1][j-1];
            }
        }
    }
    //return the value
    return dp[m][n];
}

int main()
{
   // input and output files variables 
   

}int main()
{
    // Input and output file paths
    string inputFilePath = "input.txt";
    string outputFilePath = "output.txt";

    // Open input file for reading
    ifstream inputFile(inputFilePath);
    if (!inputFile)
    {
        cerr << "Failed to open input file: " << inputFilePath << endl;
        return 1;
    }

    // Open output file for writing
    ofstream outputFile(outputFilePath);
    if (!outputFile)
    {
        cerr << "Failed to open output file: " << outputFilePath << endl;
        return 1;
    }

    // Read input from file
    string s, p;
    inputFile >> s >> p;

    // Call the isMatch function
    bool result = isMatch(s, p);

    // Write output to file
    outputFile << "Pattern match result: " << (result ? "true" : "false") << endl;

    // Close input and output files
    inputFile.close();
    outputFile.close();

    return 0;
}
