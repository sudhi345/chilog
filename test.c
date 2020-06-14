#include "chilog.h"

int main()
{

	struct ChiLOG txt;
	setNames(&txt,".phytec-CAN-log-info","phytec-CAN-log-","txt");
	clogInit(&txt);
	printf("\nnum = %d\n",txt.num);
	for (int i=0;i<10;i++)
	{
		clog(&txt, "STD_CAN_FRAME", "12345678 aa ab ac ad ae af 10 11");
	}
	return 0;
}
