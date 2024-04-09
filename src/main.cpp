#include <iostream>
#include <time.h>
#include <vector>
#include "red_black_tree.hpp"

using namespace std;

int main() {
    srand(time(NULL));
    int value[] = {0
    };
    int N = sizeof(value) / sizeof(int);
    if (N == 1) {
        for (int test = 0; test < 1000; ++test) {
            int N = 1 + rand() % 10000;
            int max = 1 + rand() % 1000;
            cout << "random check " << N << " " << max << endl;
            vector<int> history;
            int init = rand() % max;
            red_black_tree<int> rbt(init, color_t::Black);
            history.emplace_back(init);
            vector<int> inserted;
            inserted.emplace_back(init);
            for (int i = 0; i < N; ++i) {
                if (!rbt.valid()) {
                    cout << "failed with valid" << endl;
                    for (int j = 0; j < history.size(); ++j) {
                        cout << history[j] << ", ";
                    }cout << endl;
                    return 0;
                }
                if (rand() % 100 < 70 || inserted.size() == 1) {
                    int v = rand() % max;
                    rbt.insert(v);
                    history.emplace_back(v);
                    inserted.emplace_back(v);
                }
                else {
                    int i = rand() % inserted.size();
                    int v = inserted[i];
                    history.emplace_back(-v);
                    inserted.erase(inserted.begin() + i);
                    red_black_tree<int>* e = rbt.extract(v);
                    if (e != nullptr) {
                        delete e;
                    }
                    else {
                        cout << "failed when finding " << v << endl;
                        for (int j = 0; j < history.size(); ++j) {
                            cout << history[j] << ", ";
                        }cout << endl;
                        return 0;
                    }
                }
            }
        }
        cout << "Passed random test" << endl;
    }
    else {
        cout << "N : " << N << endl;
        red_black_tree<int> rbt(value[0], color_t::Black);
        for (int i = 1; i < N; ++i) {
            int v = value[i];
            if (v > 0) {
                cout << "=====================" << endl;
                cout << "inserted " << v << endl;
                rbt.insert(v);
                cout << "h : " << rbt.height() << endl;
                {
                    vector<string> rs = rbt.toStrings();
                    for (int i = 0; i < rs.size(); ++i) {
                        cout << rs[i] << endl;
                    }
                    cout << rbt.toString() << endl;
                }
            }
            else {
                v = -v;
                cout << "=====================" << endl;
                red_black_tree<int>* e = rbt.extract(v);
                if (e != nullptr) {
                    cout << "found " << v << endl;
                    delete e;
                    cout << "h : " << rbt.height() << endl;
                    {
                        vector<string> rs = rbt.toStrings();
                        for (int i = 0; i < rs.size(); ++i) {
                            cout << rs[i] << endl;
                        }
                        cout << rbt.toString() << endl;
                    }
                }
                else {
                    cout << "failed " << v << endl;
                }
            }
        }
        cout << (rbt.valid() ? "valid" : "failed") << endl;
    }
    return 0;
}