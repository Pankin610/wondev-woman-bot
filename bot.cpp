#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <unordered_map>
#include <cmath>

using namespace std;

int main()
{
    int size;
    cin >> size; cin.ignore();
    int unitsPerPlayer;
    cin >> unitsPerPlayer; cin.ignore();

    // game loop
    while (1) {
        for (int i = 0; i < size; i++) {
            string row;
            cin >> row; cin.ignore();
        }
        for (int i = 0; i < unitsPerPlayer; i++) {
            int unitX;
            int unitY;
            cin >> unitX >> unitY; cin.ignore();
        }
        for (int i = 0; i < unitsPerPlayer; i++) {
            int otherX;
            int otherY;
            cin >> otherX >> otherY; cin.ignore();
        }
        int legalActions;
        cin >> legalActions; cin.ignore();
        for (int i = 0; i < legalActions; i++) {
            string atype;
            int index;
            string dir1;
            string dir2;
            cin >> atype >> index >> dir1 >> dir2; cin.ignore();
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        cout << "MOVE&BUILD 0 N S" << endl;
    }
}