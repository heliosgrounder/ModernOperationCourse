#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <time.h>
#include <random>
#include <unordered_set>
#include <algorithm>
using namespace std;


class MaxCliqueProblem
{
public:
    static int GetRandom(int a, int b)
    {
        static mt19937 generator;
        uniform_int_distribution<int> uniform(a, b);
        return uniform(generator);
    }

    void ReadGraphFile(string filename)
    {
        ifstream fin(filename);
        string line;
        int vertices = 0, edges = 0;
        while (getline(fin, line))
        {
            if (line[0] == 'c')
            {
                continue;
            }

            stringstream line_input(line);
            char command;
            if (line[0] == 'p')
            {
                string type;
                line_input >> command >> type >> vertices >> edges;
                neighbour_sets.resize(vertices);
            }
            else
            {
                int start, finish;
                line_input >> command >> start >> finish;
                // Edges in DIMACS file can be repeated, but it is not a problem for our sets
                neighbour_sets[start - 1].insert(finish - 1);
                neighbour_sets[finish - 1].insert(start - 1);
            }
        }
    }

    void Sorting(vector<int> &arr)
    {
        vector<unordered_set<int>> test(neighbour_sets.size());
        copy(begin(neighbour_sets), end(neighbour_sets), begin(test));
        sort(arr.begin(), arr.end(), [test](const int &x, const int &y) {
            return test[x].size() < test[y].size();
        });
        vector<int> colors_arr(neighbour_sets.size(), -1);
        colors_arr[arr[0]] = 0;
        for (size_t i = 1; i < arr.size(); ++i)
        {
            int color = 0;
            unordered_set<int> colors;
            for (const auto &elem : neighbour_sets[arr[i]])
            {
                colors.insert(colors_arr[elem]);
            }   
            for (size_t j = 0; j < arr.size(); ++j)
            {
                if (colors.find(color) != colors.end())
                {
                    color++;
                }
                else
                {
                    break;
                }
            }
            colors_arr[arr[i]] = color;
        }

        sort(arr.begin(), arr.end(), [colors_arr](const int &x, const int &y) {
            return colors_arr[x] > colors_arr[y];
        });

        // for (const auto &e : colors_arr)
        // {
        //     cout << e << " ";
        // }
        // cout << endl;

        // for (const auto &e : arr)
        // {
        //     cout << e << " ";
        // }
        // cout << endl;
    }

    void FindClique(int randomization, int iterations)
    {
        static mt19937 generator;
        for (int iteration = 0; iteration < iterations; ++iteration)
        {
            vector<int> clique;
            vector<int> candidates(neighbour_sets.size());
            for (int i = 0; i < neighbour_sets.size(); ++i)
            {
                candidates[i] = i;
            }
            Sorting(candidates);
            // shuffle(candidates.begin(), candidates.end(), generator);
            while (!candidates.empty())
            {
                int last = candidates.size() - 1;
                // int rnd = GetRandom(0, min(randomization - 1, last));
                // int vertex = candidates[rnd];
                int vertex = candidates[0];
                clique.push_back(vertex);
                for (int c = 0; c < candidates.size(); ++c)
                {
                    last = candidates.size() - 1;
                    int candidate = candidates[c];
                    // cout << neighbour_sets[vertex].count(candidate) << " ";
                    // cout << "Vertex: " << vertex << " Candidate: " << candidate << endl;
                    // for (const auto &e : neighbour_sets[vertex])
                    // {
                    //     cout << e << " ";
                    // }
                    // cout << "Count: " << neighbour_sets[vertex].count(candidate) << endl;
                    if (neighbour_sets[vertex].count(candidate) == 0)
                    {
                        // Move the candidate to the end and pop it
                        swap(candidates[c], candidates[last]);
                        candidates.pop_back();
                        --c;
                    }
                    // cout << "Candidates: ";
                    // for (const auto &e : candidates)
                    // {
                    //     cout << e << " ";
                    // }
                    // cout << endl;
                }
                if (!candidates.empty())
                    Sorting(candidates);
                // cout << "Candidates after: ";
                // for (const auto &e : candidates)
                // {
                //     cout << e << " ";
                // }
                // cout << endl;

                // cout << "Clique: ";
                // for (const auto &e : clique)
                // {
                //     cout << e << " ";
                // }
                // cout << endl;
                // shuffle(candidates.begin(), candidates.end(), generator);
            }
            if (clique.size() > best_clique.size())
            {
                best_clique = clique;
            }
        }
    }

    const vector<int>& GetClique()
    {
        return best_clique;
    }

    bool Check()
    {
        if (unique(best_clique.begin(), best_clique.end()) != best_clique.end())
        {
            cout << "Duplicated vertices in the clique\n";
            return false;
        }
        for (int i : best_clique)
        {
            for (int j : best_clique)
            {
                if (i != j && neighbour_sets[i].count(j) == 0)
                {
                    cout << "Returned subgraph is not a clique\n";
                    return false;
                }
            }
        }
        return true;
    }

private:
    vector<unordered_set<int>> neighbour_sets;
    vector<int> best_clique;
};

int main()
{
    int iterations;
    cout << "Number of iterations: ";
    cin >> iterations;
    int randomization;
    cout << "Randomization: ";
    cin >> randomization;

    string files_root_folder = "src/";

    vector<string> files = { 
        "brock200_1.clq",
        "brock200_2.clq",
        "brock200_3.clq",
        "brock200_4.clq",
        "brock400_1.clq",
        "brock400_2.clq",
        "brock400_3.clq",
        "brock400_4.clq",
        "C125.9.clq",
        "gen200_p0.9_44.clq",
        "gen200_p0.9_55.clq",
        "hamming8-4.clq",
        "johnson16-2-4.clq",
        "johnson8-2-4.clq",
        "keller4.clq",
        "MANN_a27.clq",
        "MANN_a9.clq",
        "p_hat1000-1.clq",
        "p_hat1000-2.clq",
        "p_hat1500-1.clq",
        "p_hat300-3.clq",
        "p_hat500-3.clq",
        "san1000.clq",
        "sanr200_0.9.clq",
        "sanr400_0.7.clq"
    };
    // vector<string> files = {
    //     "helios.clq"
    // };
    ofstream fout("clique.csv");
    fout << "File; Clique; Time (sec)\n";
    for (string file : files)
    {
        MaxCliqueProblem problem;
        problem.ReadGraphFile(files_root_folder + file);
        clock_t start = clock();
        problem.FindClique(randomization, iterations);
        if (! problem.Check())
        {
            cout << "*** WARNING: incorrect clique ***\n";
            fout << "*** WARNING: incorrect clique ***\n";
        }
        fout << file << "; " << problem.GetClique().size() << "; " << double(clock() - start) / CLOCKS_PER_SEC << '\n';
        cout << file << ", result - " << problem.GetClique().size() << ", time - " << double(clock() - start) / CLOCKS_PER_SEC << '\n';
    }
    fout.close();
    return 0;
}
