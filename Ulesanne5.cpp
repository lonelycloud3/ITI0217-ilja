// Ulesanne5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <iomanip>

using namespace std;

vector<int>* books;
unordered_map<int, int>* people;
int max_person_id;
int max_book_id = 0;

void initStuff(const char* filename) {
    string line;
    ifstream file(filename);

    books = new vector<int>[100000];
    people = new unordered_map<int, int>[100000];


    getline(file, line); // skipping first line

    int person_id;
    int book_id;

    char comma;

    while (getline(file, line)) {
        stringstream ss(line);
        ss >> person_id;
        ss >> comma; // skipping comma
        ss >> book_id;
        if (book_id > max_book_id) {
            max_book_id = book_id;
        }
        books[book_id].push_back(person_id);
        people[person_id].insert(pair<int, int>(book_id, 1));
    }
    max_person_id = person_id;

    file.close();
}

// 5.a
int findMutualReaders(int book_id_1, int book_id_2) {
    int result = 0;

    for (int& id : books[book_id_1]) {
        if (people[id].find(book_id_2) != people[id].end()) {
            result++;
        }
    }

    return result;
}

// 5.b
void constructTable(const char* filename) {

    ofstream file(filename);

    for (int i = 0; i < max_book_id - 1; i++) {
        cout << i << endl;
        for (int j = i + 1; j < max_book_id; j++) {
            int readers = findMutualReaders(i, j);
            if (readers > 0) {
                file << "Raamat" << i << "," << "Raamat" << j << "," << readers << "\n";
            }
        }
    }
    file.close();
}

// 5.c
double conditionalProbability(int book_id_1, int book_id_2) {
    int result = 0;

    if (books[book_id_1].size() == 0) {
        return 0;
    }

    for (int& id : books[book_id_1]) {
        if (people[id].find(book_id_2) != people[id].end()) {
            result++;
        }
    }

    return ((double)result / (double)books[book_id_1].size()) * 100;
}

void constructTableProbs(const char* filename) {
    ofstream file(filename);

    file << setprecision(2);

    for (int i = 0; i < max_book_id - 1; i++) {
        cout << i << endl;
        for (int j = i + 1; j < max_book_id; j++) {
            int prob = conditionalProbability(i, j);
            if (prob > 0) {
                file << "NENDEST kes ostsid raamatut " << i << ", ostsid ka raamatut " << j << " " << prob << "% inimestest." << endl;
            }
        }
    }
    file.close();
}


int main()
{
    initStuff("retail_data.txt");

    time_t start, end;

    cout << "Finished with initialization!" << endl;
    cout << "Max book id: " << max_book_id << endl;

    // 5.a
    int temp = findMutualReaders(39, 89);
    cout << "Books 39 and 89 have " << temp << " mutual readers";


    
    
    // 5.b
    //time(&start);
    //constructTable("table.txt");
    //time(&end);
    //double time_taken = double(end - start);
    //cout << "Time taken by program is : " << time_taken << " sec";
    
    // 5.c
    //int temp2 = findMutualReaders(1, 2);
    //int total = books[1].size();
    //double temp = conditionalProbability(1, 2);
    //cout << setprecision(2);
    //cout << temp << "%" << endl;

    //time(&start);
    //constructTableProbs("table_probs.txt");
    //time(&end);
    //double time_taken = double(end - start);
    //cout << "Time taken by program is : " << time_taken << " sec";

    delete[] books;
    delete[] people;

    return 0;
}