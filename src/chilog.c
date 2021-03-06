/***************************************************************************************************************************
This code is written by Sudheendra H, Team Chimera. (c)2020, Team Chimera.
***************************************************************************************************************************/

#include "../include/chilog.h"

void setNames(struct ChiLOG *cl, char lifn[], char lfn[], char extn[])
{
	strcpy(cl->logInfo,  lifn);
	strcpy(cl->fileName,  lfn);
	strcpy(cl->extension,extn);
	chilogInit(cl);
	return;
}

void getFileName(struct ChiLOG *cl)
{
	char Num[13];
	sprintf(Num,"%012d",cl->num);	//convert num to string 
	strcat(cl->fileName,Num);
	strcat(cl->fileName,".");
	strcat(cl->fileName,cl->extension);
	return;
}

int getFileNum(char * tempName)
{
	int temp = atoi(tempName);
	return (++temp);
}

void chilogInit(struct ChiLOG *cl)
{
	char tempName[32]="";
	int seekNum = strlen(cl->extension) + 13;		//xxxxxx<12-digits><.><extension><EOF> 

	if((cl->lif = fopen(cl->logInfo,"r")) == NULL) 		//that means file does not exist.
	{
		if((cl->lif = fopen(cl->logInfo,"a+")) == NULL)	//create a file for writing.
		{
			perror("Can not create log info file ");
			exit(1);
		}
		else
		{	//write basic info and set index num to zero
			fprintf(cl->lif,"#This file was generated by ChiLOG.\n#DO NOT DELETE or EDIT this file to avoid errors.\n#");
			cl->num = 0;
		}
	}

	else	//file exists and is opened for read only.
	{	//go to last line fetch the last used file name, increment index and open for appending
		fseek(cl->lif,-(seekNum),SEEK_END);
		fgets(tempName,13,cl->lif);		//read index of last log file, 12-digits
		cl->num = getFileNum(tempName);
		fclose(cl->lif);		
		cl->lif = fopen(cl->logInfo,"a+");	//now open in write mode.
	}

	getFileName(cl);
	fprintf(cl->lif,"\n");
	fprintf(cl->lif,cl->fileName);		//write file name to log info file
	fclose(cl->lif);
	cl->bt = time(NULL);
	cl->cps = CLOCKS_PER_SEC;
	if( (!(strcmp(cl->extension,"csv"))) || (!(strcmp(cl->extension,"CSV"))) )		//if "csv" file
	{
		chilogCSVInit(cl);
		return;
	}

	else
	{
		cl->lf = fopen(cl->fileName,"a");	//open new log file for appending
		fprintf(cl->lf,"#This is a log file generated by ChiLOG. DO NOT EDIT or DELETE to avoid data loss.\n#CLOCKS_PER_SEC = %ld, Base time = %d\n#\n",cl->cps,cl->bt);
		fclose(cl->lf);
	}	
	return;
}

void chilog(struct ChiLOG *cl, char * type, char * str)
{
	cl->lf = fopen(cl->fileName,"a");
	fprintf(cl->lf,"[%012ld.%012d] ",(time(NULL) - (cl->bt)),clock());
	fprintf(cl->lf," %s : %s\n",type,str);
	fclose(cl->lf);
	return;
}

void chilogCSVInit(struct ChiLOG *cl)
{
	cl->lf = fopen(cl->fileName,"a");
	fprintf(cl->lf,"Time (sec.ticks),Direction,Speed in RPM,Error Code,Battery Voltage (V),Motor Current (A),Motor Temperature(deg. C),Controller Temperature(deg. C)\n");	//write column headings
	fclose(cl->lf);
	return;
}

void chilogCSV(struct ChiLOG *cl, int dir, int rpm, int err, int bat_v, int mot_i, int mot_t, int ctl_t)
{
	cl->lf = fopen(cl->fileName,"a");
	fprintf(cl->lf,"%ld.%d,%d,%d,%x,%d,%d,%d,%d\n",(time(NULL) - (cl->bt)), clock(), dir, rpm, err, bat_v, mot_i, mot_t, ctl_t);
	fclose(cl->lf);
	return;
}

