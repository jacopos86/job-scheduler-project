#include <iostream>
#include <vector>

using std::cout; using std::cin;
using std::endl; using std::string;
using std::vector;

class Pair {
    int x, y;
public:
    Pair() = default;
    Pair(int a, int b) : x(a), y(b) {}

    void printPair() const {
        cout << "x: " << x << endl << "y: " << y << endl;
    }
};
int main(){
    Pair *arr = new Pair[4];

    for (int i = 0; i < 4; ++i) {
        arr[i] = Pair(i * 2, i * i);
    }

    for (int i = 0; i < 4; ++i) {
        arr[i].printPair();
    }

    delete [] arr;
    return EXIT_SUCCESS;
}

