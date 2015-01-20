#include <stdio.h>
#include "ccp_fs_api.h"

#include <sys/time.h>

REDIS pFs;
char* mykey= "iamsetkey";
struct timeval before,after;

int main(int argc, char** argv){

	int ret;
	char** allmember;

	//pFs = ccp_fs_connect("127.0.0.1", 6379, 10000);	



	FS_IP_INFO *fsIps = malloc( sizeof(FS_IP_INFO));
    fsIps->nCount=2;
	strcpy(fsIps->iptbl[0].szIp,"127.0.0.1");
	strcpy(fsIps->iptbl[0].szIpReal,"127.0.0.1");
	fsIps->iptbl[0].nPort=6379;

	strcpy(fsIps->iptbl[1].szIp,"127.0.0.1");
	strcpy(fsIps->iptbl[1].szIpReal,"127.0.0.1");
	fsIps->iptbl[1].nPort=6380;

	//SUCCESS if not NULL
	ret= ccp_fs_internal_ha_init(26379, 2000,fsIps );
		printf(" call fs_ha_init %d \n", ret );

	//Success if not NULL
	pFs = ccp_fs_internal_ha_connect(*fsIps, 10000);

    if( pFs == NULL){
		printf("fs_connect Fail\n");
		exit(1);
	}

	gettimeofday(&before,NULL);
	ret = ccp_fs_smembers( pFs,  mykey,  &allmember );
	gettimeofday(&after,NULL);
	
	printf(" call smember result is %d\n", ret );
	//printf(" call smember result is %s\n", allmember[7999] );
	printf("running time[ms] : %ld\n",  
	     1000*(after.tv_sec-before.tv_sec) +  
		 (after.tv_usec-before.tv_usec)/1000);
    	
}
