#include "main.h"

void demo_API_Vehicle_vh_GetGNSS(){
	GNSSInfo_s gn;
	printf("%d\n",vh_GetGNSS (&gn));
	printf("vh_GetGNSS:gn.longitude:%f\n",gn.longitude);
	printf("vh_GetGNSS:gn.latitude:%f\n",gn.latitude);
	printf("vh_GetGNSS:gn.latitude:%f\n",gn.height);
	printf("vh_GetGNSS:gn.speed:%f\n",gn.speed);
	printf("vh_GetGNSS:gn.heading:%f\n",gn.heading);
}

void write_uart(void)
{
	int	fd = open("/dev/ttymxc4", O_RDWR | O_NOCTTY ); 
	if(fd < 0)
		printf("open uart fail!\n");
	char buf[] = "hello nebulalink!"; 
	while(1)
	{
		int len = write(fd, buf, sizeof(buf)); 
		if (len < 0) 
			printf("write data to serial failed!\n");
		sleep(1);
	}
}

int main(){
	demo_API_Vehicle_vh_GetGNSS();
	write_uart();
	return 1;
}

