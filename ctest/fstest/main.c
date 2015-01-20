#include <stdio.h>
#include "ccp_fs_api.h"

#include <sys/time.h>

REDIS pFs;
char* mykey= "iamsetkey";
struct timeval before,after;

int main(int argc, char** argv){

	int ret;
	char** allmember;

	pFs = ccp_fs_connect("127.0.0.1", 6379, 10000);	
	
    if( pFs == NULL){
		printf("fs_connect Fail\n");
		exit(1);
	}

	gettimeofday(&before,NULL);
	ret = ccp_fs_smembers( pFs,  mykey,  &allmember );
	gettimeofday(&after,NULL);
	
	printf(" call smember result is %d %s\n", ret, allmember[7999] );
	printf("running time[ms] : %ld\n",  
	     1000*(after.tv_sec-before.tv_sec) +  
		 (after.tv_usec-before.tv_usec)/1000);
    	
}
