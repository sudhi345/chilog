/***************************************************************************************************************
(c)2020, Team Chimera.
***************************************************************************************************************/

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
	int dir = 0, rpm = 0, err = 0, bat_v = 0, mot_i = 0, mot_t = 0, ctl_t = 0;	//direction, RPM, Error code, Bat. Voltage, 
											//Motor Current, Motor and Controller temperature. (Refere Kelly KAC 8080I manual)
	struct sockaddr_can addr;
	struct can_frame frame;
	struct ifreq ifr;
	struct ChiLOG txt,csv;								//logging related struct
	char buf[BUF_SIZ], prev_buf[BUF_SIZ] = "";
	char fType[16];									//Frame Type (EFF, SFF...)
	char *ifname = "can0";

	setNames(&txt,".phytec-CAN-log-info","phytec-CAN-log-","txt");			//log info file is hidden (starts with '.')
	setNames(&csv,".phytec-csv-log-info","phytec-csv-log-","csv");
	
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
	
	while (1) 
	{
		if ((nbytes = read(s, &frame, sizeof(struct can_frame))) < 0) 
		{
   			perror("Error while reading CAN frame ");
			return 1;
		} 
		else 
		{
			if (frame.can_id & CAN_EFF_FLAG)				//if it is an extended frame CAN (2.0B), 29 bit id
			{
       				n = snprintf(buf, BUF_SIZ, "0x%08x ", frame.can_id & CAN_EFF_MASK);
				strcpy(fType,"CAN_STD_EFF");
			}
    			else								//11 bit id standard CAN frame
			{
       				n = snprintf(buf, BUF_SIZ, "0x%03x ", frame.can_id & CAN_SFF_MASK);
				strcpy(fType,"CAN_STD_SFF");
			}
			n += snprintf(buf + n, BUF_SIZ - n, "[%d] ", frame.can_dlc);	//print data length count to buf (should be 8 always)
			for (i = 0; i < frame.can_dlc; i++)
			{ 
				n += snprintf(buf + n, BUF_SIZ - n, "%02x ", frame.data[i]);//print all data to buf
			}
			if (frame.can_id & CAN_RTR_FLAG)				//Remote Tx Req Frame
			{
        			n += snprintf(buf + n, BUF_SIZ - n, "remote request");
				strcpy(fType,"CAN_STD_RTR");
			}
		}
		//buf contains CAN frame
		chilog(&txt, fType, buf);						//save it, no matter same data or different data
		if(strcmp(prev_buf, buf))						//update CSV only when current data is different from previous data
		{
			if((frame.can_id & CAN_EFF_MASK) == 0x10F8109A)			//frame containing dir,rpm and err
			{
				dir = (int) (frame.data[0] & 0x03);			//Bit1-bit0:00--Neutral; 01--Forward; 10--Reverse; Bit2-bit7:reserved
				rpm = (((int)frame.data[1]) + (((int)frame.data[2])*256));
				err = (int) (frame.data[3]);				//This will be printed as hex value, no need to change
			}
			else if((frame.can_id & CAN_EFF_MASK) == 0x10F8108D)		//frame containing bat_v, mot_i, mot_t and ctl_t
			{
				bat_v = (((int)frame.data[0]) + (((int)frame.data[1])*256));
				mot_i = (((int)frame.data[2]) + (((int)frame.data[3])*256));
				mot_t = (((int)frame.data[4]) + (((int)frame.data[5])*256));
				ctl_t = (((int)frame.data[6]) + (((int)frame.data[8])*256));
			}
			else								//extend with else-if for other devices
			{
			}
			if(((frame.can_id & CAN_EFF_MASK) == 0x10F8109A) || ((frame.can_id & CAN_EFF_MASK) == 0x10F8108D))
			{								//save it only if the received data is from the controller
				chilogCSV(&csv, dir, rpm, err, bat_v, mot_i, mot_t, ctl_t);//save it to CSV file
			}
		}	
	}
	return 0;	
}
