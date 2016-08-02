#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// Removing duplicate elements in a collection requires
// the sort - unique - erase idiom
int main() {
    vector<int> v { 1, 2, 3, 4, 5, 5, 2, 3, 4 };

    sort(v.begin(), v.end());
    auto newEnd = unique(v.begin(), v.end());

    cout << "Elements are unique up to newEnd (position "
	 << distance(v.begin(), newEnd) << ")" << endl;
    for (auto it : v) {
        cout << it << endl;
    }

    v.erase(newEnd, v.end());

    cout << "New List after removing" << endl;
    for (auto it : v) {
        cout << it << endl;
    }

    return 0;
}
