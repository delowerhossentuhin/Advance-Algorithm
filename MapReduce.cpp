#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <mutex>
#include <thread>
#include <fstream> 
using namespace std;
using namespace std::chrono;
mutex mtx;
vector<pair<string, int>> mapFunction(const string& text) {
    vector<pair<string, int>> keyValuePairs;
    stringstream ss(text);
    string word;
    while (ss >> word) {
        keyValuePairs.push_back({word, 1}); 
    }
    return keyValuePairs;
}
void reduceFunction(map<string, int>& result, const vector<pair<string, int>>& keyValuePairs) {
    lock_guard<mutex> lock(mtx);
    for (const auto& pair : keyValuePairs) {
        result[pair.first] += pair.second;
    }
}
string readFile(const string& fileName) {
    ifstream file(fileName);
    if (!file) {
        cerr << "Error: Unable to open file " << fileName << endl;
        exit(1);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
int main() {

    string file1 = "TestFile1.txt";
    string file2 = "TestFile2.txt";

    string content1 = readFile(file1);
    string content2 = readFile(file2);

    vector<string> inputData = {content1, content2};

    vector<thread> threads;
    vector<vector<pair<string, int>>> mappedResults;
    map<string, int> reducedResult;
    auto start = high_resolution_clock::now();

    for (const auto& text : inputData) {
        threads.emplace_back([&mappedResults, &text]() {
            auto result = mapFunction(text);
            lock_guard<mutex> lock(mtx); 
            mappedResults.push_back(result);
        });
    }
    for (auto& thread : threads) {
        thread.join(); 
    }
    threads.clear();
    vector<pair<string, int>> shuffledData;
    for (const auto& result : mappedResults) {
        shuffledData.insert(shuffledData.end(), result.begin(), result.end());
    }
    for (const auto& kvPair : shuffledData) {
        threads.emplace_back([&reducedResult, &kvPair]() {
            reduceFunction(reducedResult, {kvPair});
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }
    auto end = high_resolution_clock::now();
    auto durationMilli = duration_cast<milliseconds>(end - start);
    cout << "Time: " << durationMilli.count() << " milliseconds" << endl;
    cout << "Word Counts:" << endl;
    for (const auto& entry : reducedResult) {
        cout << entry.first << ": " << entry.second << endl;
    }
    return 0;
}
