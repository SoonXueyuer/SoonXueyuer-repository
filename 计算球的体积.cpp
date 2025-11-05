
#include <iostream>
#include <iomanip>
using namespace std;
int main() 
{
    double radius;
    cout << "Input the radius of the sphere: ";
    cin >> radius;
    const double PI = 3.1415926535897932384626;
    double volume = (4.0 / 3) * PI * radius * radius * radius;
    cout << fixed << setprecision(2) << "The volume of sphere is " << volume << endl;
    return 0;
}

