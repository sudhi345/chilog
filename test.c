#include "include/chilog.h"

int main()
{

	struct ChiLOG txt, csv;
	setNames(&txt,".phytec-CAN-log-info","phytec-CAN-log-","txt");
	setNames(&csv,".phytec-csv-log-info","phytec-csv-log-","csv");
	for (int i=0;i<10;i++)
	{
		chilog(&txt, "STD_CAN_FRAME", "12345678 aa ab ac ad ae af 10 11");
		chilogCSV(&csv, 0, 1000, 0, 96, 100, 60, 40);
	}
	return 0;
}
