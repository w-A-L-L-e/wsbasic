#include <iostream>

using namespace std;

int main(){
	double b;
	for( double a=0; a < 2000000; a=a+1 ){
		b=a*2;
	}

	cout << "b=" << b << endl;

	return 0;
}
