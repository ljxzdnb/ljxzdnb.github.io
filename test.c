#include <stdio.h>



void test(int n){
	int num[n];
	for(int i=0;i<n;i++){
		num[i]=rand();
	}
	for(int i=0;i<n;i++){
		printf("%d\n",num[i]);
	}
}

int main(){
	int n = 2;
	int num[] = {2,5};
	test(n);

}
