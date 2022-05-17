#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <windows.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

unsigned int getInt(unsigned char * d)
{
	return *(unsigned int *)d;
	//return (d[3]|d[2]<<8|d[1]<<16|d[0]<<24)&0xFFFFFFFF;
}
unsigned long long getLong(unsigned char *d)
{
	/*unsigned long long l = (unsigned long long)getInt(d+4);
	l<<=32;
	l |= getInt(d);
	return l;*/
	return *((unsigned long long*)d);
}
int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "C.UTF-8");
	char filename[100];
	printf("Input name:");
	scanf("%s",filename);
	FILE * fp = fopen(filename,"rb");
	if(fp==NULL)
	{
		printf("SOMETHING WENT WRONG");
	}
	unsigned int type;
	unsigned int length;
	unsigned char *data;
	while(fread(&type,4,1,fp)>0)
	{
		fread(&length,4,1,fp);
		data = calloc(length, 1);
		fread(data,1,length,fp);
//		printf("%x", '\x00doM');
		printf("Type : %d, Length : %d, Data:",type,length);
		switch(type)
		{
			case /*'Mod\x00'://*/0x00646F4D:
				printf("FOUND signature:");
				printf("%s",data);
				break;
			case 0x7265560A:
				printf("FOUND VERSION:");
				printf("%x",getInt(data));
				for(int i=0;i<length;i++)
				{
					printf("%x",data[i]);
				}
				break;
			case 0x6C69460A:
				printf("FOUND FILE NAME:");
				for(int i=0;i<length;i++)
				{
					printf("%c",data[i]);
				}
				break;
			case 0x7A69530A:
				printf("FOUND FILE SIZE:");
				printf("%x",getInt(data));
				break;
			case 0x7263430A:
				printf("FOUND CRC value:%x",getInt(data));
				break;
			case 0x6176530A:
				printf("FOUND general purpose save area:");
				break;
			case 0x7473540A:
				printf("FOUND TIME STAMP:");
				unsigned long long timestamp = getLong(data);
				FILETIME ft =*(FILETIME*)&timestamp;
				SYSTEMTIME st;
				//ft.dwHighDateTime = (timestamp>>32) &0x7FFFFFFF;
				//ft.dwLowDateTime = timestamp & 0x7FFFFFFF;
				//printf("%d %d",ft.dwHighDateTime, ft.dwLowDateTime); 
				FileTimeToSystemTime(&ft, &st);
				//struct tm  ts =  *localtime(&timestamp);
				printf("%lu",timestamp);
				printf("Local Time %d-%d-%d %d:%d:%d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				break;
			case 0x61624D0A:
				printf("FOUND Memory Map; offset:%x size:%x filename:%s\n",getInt(data),getInt(data+4),data+8);
				break;
			case 1987269898:
				printf("FOUND COMMENT:");
				printf("%x", getInt(data));
				printf("%s",data+4);
				break;
			case 0x646E450A:
				printf("END OF DATA");
			default:
				break;
		}
		printf("\n");
		
	}
	system("pause");
	return 0;
}
