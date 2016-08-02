#include <iostream>
#include <bitset>

using namespace std;

int GetCount(uint64_t num) {
    auto bs = bitset<sizeof(uint64_t)>(num);
    return bs.count();
}

int main() {
    cout << GetCount(16) << endl;
    cout << GetCount(15) << endl;
    cout << GetCount(14) << endl;
    cout << GetCount(1) << endl;
    cout << GetCount(2) << endl;
    cout << GetCount(8) << endl;
}
