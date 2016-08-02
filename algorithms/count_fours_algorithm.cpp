// Count all fours in a container of only 5 and 4,
// knowing that after a 4 appears, only other 4 are
// allowed

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int CountFours(const vector<int>& v) {
    auto it = lower_bound(v.rbegin(), v.rend(), 5);
    return distance(v.rbegin(), it);
}

int main() {
    cout << CountFours({ }) << endl;
    cout << CountFours({ 4 }) << endl;
    cout << CountFours({ 5 }) << endl;
    cout << CountFours({ 4, 4, 4 }) << endl;
    cout << CountFours({ 4, 4, 4, 4, 4, 4 }) << endl;
    cout << CountFours({ 5, 4, 4, 4, 4, 4 }) << endl;
    cout << CountFours({ 5, 5, 4, 4, 4, 4 }) << endl;
    cout << CountFours({ 5, 5, 5, 4, 4, 4 }) << endl;
    cout << CountFours({ 5, 5, 5, 5, 4, 4 }) << endl;
    cout << CountFours({ 5, 5, 5, 5, 5, 4 }) << endl;
    cout << CountFours({ 5, 5, 5, 5, 5, 5 }) << endl;
    return 0;
}
