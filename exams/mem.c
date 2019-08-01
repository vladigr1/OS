#include <stdio.h>

int main(){
	int arr[2];
	arr[0] =arr;
	arr[1] = &arr[1];
	printf("&arr[1] = %x &arr[0] = %x\n&arr[1] - &arr[0] = %d\n",&arr[1],&arr[0] ,&arr[1] - &arr[0] );
	printf("arr[1] = %x arr[0] = %x\narr[1] - arr[0] = %d\n",arr[1],arr[0] ,arr[1] - arr[0] );
}
