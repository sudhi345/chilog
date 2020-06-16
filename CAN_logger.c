#include "include/chilog.h"
#include <unistd.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
 
#include <linux/can.h>
#include <linux/can/raw.h>

#define BUF_SIZ (255)

int main(void)
{
	int s;
	int n=0;
	int nbytes ,i;
	struct sockaddr_can addr;
	struct can_frame frame;
	struct ifreq ifr;
	char buf[255];
	FILE *out = stdout;
 
	char *ifname = "can0";
	
	if((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) 
	{
		perror("Error while opening socket");
		return -1;
	}
	
	strcpy(ifr.ifr_name, ifname);
	ioctl(s, SIOCGIFINDEX, &ifr);
 
	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex; 
 
	printf("%s at index %d\n", ifname, ifr.ifr_ifindex);
 
	if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Error in socket bind");
		return -2;
	}
	
}
