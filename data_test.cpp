#include <iostream>
int main(){
    using namespace std;
    long a=-4607182418800017408;
    double *b;
    b=(double *)&a;
    cout<<*b<<endl;
}