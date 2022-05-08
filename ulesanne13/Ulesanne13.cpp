#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <queue>

using namespace std;

struct stock {
    double cor;
    string name;
};


vector<string> stock_names;
unordered_map<string, vector<double>> closed_diffs;
unordered_map<string, double> corrs;

unordered_map<string, vector<stock>> top3_map;
vector<string> names_used_in_tree;

void readData(string filename)
{
    string line;
    string name = "";
    ifstream myfile(filename);

    double previous = 0;
    bool first = true;


    getline(myfile, line);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            try {
                stringstream ss(line);
                string token;
                getline(ss, token, ',');
                getline(ss, token, ',');
                getline(ss, token, ',');
                getline(ss, token, ',');
                getline(ss, token, ',');
                double closed_price = stod(token);

                getline(ss, token, ',');
                getline(ss, token, ',');
                if (token != name) {
                    first = true;
                }
                name = token;


                double diff = closed_price - previous;
                vector<double> init;
                init.push_back(diff);
                if (!first) {
                    if (closed_diffs.find(name) == closed_diffs.end()) {
                        closed_diffs.insert(pair<string, vector<double>>(name, init));
                    }
                    else {
                        closed_diffs.at(name).push_back(diff);
                    }
                }
                else {
                    first = false;
                }


                if (std::find(stock_names.begin(), stock_names.end(), name) == stock_names.end())
                {
                    stock_names.push_back(name);
                }
                previous = closed_price;
            }
            catch (exception e) {
                continue;
            }
            
        }
        myfile.close();
    }
}


void calculateCorrelationAgaintsOther(string stockA) {
    vector<double> X = closed_diffs.at(stockA);
    double sum = 0;
    for (auto& x : X)
        sum += x;
    double mean_x = sum / X.size();

    sum = 0;
    for (auto& x : X)
        sum += (mean_x - x) * (mean_x - x);
    double var_x = sum / (X.size() - 1);
    double stdv_x = sqrt(var_x);

    for (auto& name : stock_names) {
        if (name != stockA) {
            vector<double> Y = closed_diffs.at(name);
            sum = 0;
            for (auto& y : Y)
                sum += y;
            double mean_y = sum / Y.size();

            sum = 0;
            for (auto& y : Y)
                sum += (y - mean_y)*(y - mean_y);
            double var_y = sum / (Y.size() - 1);
            double stdv_y = sqrt(var_y);

            int size = min(X.size(), Y.size());

            sum = 0;
            for (int i = 0; i < size; i++) {
                sum += (X[i] - mean_x) * (Y[i] - mean_y);
            }
            double cov = sum / (size - 1);

            double cor = cov / (stdv_x * stdv_y);

            corrs.insert(pair<string, double>(name, cor));

            //cout << name << " " << cor << endl;

        }
    }


}

void saveResults(string filename) {

    ofstream file(filename);

    if (file.is_open()) {
        for (auto& name : stock_names) {
            if (corrs.find(name) != corrs.end()) {
                file << name << "," << corrs.at(name) << endl;
            }
        }
    }

    file.close();


}

void getCorrs(string filename) {


    string line;

    ifstream file(filename);

    char delim = ',';

    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss (line);
            string name;
            string cor_s;

            getline(ss, name, delim);
            getline(ss, cor_s, delim);

            double cor = stod(cor_s);

            corrs.insert(pair<string, double>(name, cor));
            if (std::find(stock_names.begin(), stock_names.end(), name) == stock_names.end())
            {
                stock_names.push_back(name);
            }
        }
        file.close();
    }
}

vector<string> findtop3(string filename, string stockA) {
    vector<double> top3{ 0, 0, 0 };
    vector<string> result{ "", "", "" };

    for (auto& name : stock_names) {
        if (name != stockA) {
            for (int i = 0; i < 3; i++) {
                if (corrs.at(name) > top3[i]) {
                    top3[i] = corrs.at(name);
                    result[i] = name;
                    break;
                }
            }
        }
    }

    ofstream file(filename);
    vector<stock> stocks{ {top3[0], result[0]}, {top3[1], result[1]}, {top3[2], result[2]} };
    top3_map.insert(pair<string, vector<stock>>(stockA, stocks));
 
    if (std::find(names_used_in_tree.begin(), names_used_in_tree.end(), stockA) == names_used_in_tree.end())
    {
        names_used_in_tree.push_back(stockA);
    }

    for (string& name : result) {
        if (std::find(names_used_in_tree.begin(), names_used_in_tree.end(), name) == names_used_in_tree.end())
        {
            names_used_in_tree.push_back(name);
        }
    }

    if (file.is_open()) {
        for (int i = 0; i < 3; i++) {
            file << result[i] << "," << top3[i] << endl;
        }
    }

    file.close();

    return result;
}

void reccurentCorrs(string first) {

    queue<string> q;
    vector<string> visited;

    q.push(first);

    int count = 0;

    while (count < 27) {
        string target = q.front();
        q.pop();

        if (std::find(visited.begin(), visited.end(), target) != visited.end())
        {
            count++;
            continue;
        }

        corrs.clear();
        calculateCorrelationAgaintsOther(target);
        string filename1 = "corrs_" + target + ".csv";
        saveResults(filename1);
        string filename2 = "top3_" + target + ".csv";
        vector<string> top3 = findtop3(filename2, target);

        for (auto& name : top3) {
            q.push(name);
        }

        visited.push_back(target);
        count++;
    }

}

void transformDataForPast(string filename) {

    ofstream file(filename);

    if (file.is_open()) {
        file << ". ";
        for (string& name : names_used_in_tree) {
            file << name << " ";
        }
        file << endl;
        for (string& i : names_used_in_tree) {
            if (top3_map.find(i) != top3_map.end())
            {
                file << i << " ";
                vector<stock> top3_i = top3_map.at(i);
                for (string& j : names_used_in_tree) {
                    vector<stock> top3_j;
                    bool reverseAlso = false;
                    if (top3_map.find(j) != top3_map.end()) {
                        top3_j = top3_map.at(j);
                        reverseAlso = true;
                    }
                    bool contains = false;
                    for (stock& k : top3_i) {
                        if (k.name == j) {
                            file << k.cor << " ";
                            contains = true;
                            break;
                        }
                    }
                    if (!contains && reverseAlso) {
                        for (stock& k : top3_j) {
                            if (k.name == i) {
                                file << k.cor << " ";
                                contains = true;
                                break;
                            }
                        }
                    }
                    if (!contains) {
                        file << "0 ";
                    }
                }
                file << endl;
            }
        }
        file.close();

    }

}

int main()
{
    /*readData("all_stocks_5yr.csv");
    cout << "Data reading complete" << endl;
    calculateCorrelationAgaintsOther("VFC");
    saveResults("corrs_vfc.csv");
    findtop3("top3_vfc.csv", "VFC");
    cout << "Results saved" << endl;*/

    /*getCorrs("corrs_vfc.csv");
    findtop3("top3_vfc.csv", "VFC");*/

    readData("all_stocks_5yr.csv");
    cout << "Data reading complete" << endl;
    reccurentCorrs("NKE");
    transformDataForPast("pastdata2.txt");

    //vector<double> test = closed_diffs.at("AAL");

    //for (double& kyrka : test) {
    //    cout << kyrka << " ,";
    //}

}

