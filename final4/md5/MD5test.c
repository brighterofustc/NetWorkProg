#include <stdio.h>
int main(int argc, char* argv[])
{
	char md5_sum[MD5_LEN + 1];
	if(!CalcFileMD5("./MD5C.c", md5_sum))
		puts("Error occured!");
	else
		printf("Success! MD5 sum is :%s \n", md5_sum);
}