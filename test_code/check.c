#include <stdio.h>
void change(int &a){
	a = 10;
}
int main(){
	int a = 90;
	printf("%d\n", a);
	change(a);
			printf("%d\n", a);
	//	cout << "first btp output" << endl;

}
