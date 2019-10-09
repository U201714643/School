#include "stdafx.h"

char * MsToTime(char * ms,char * Src){		//转换毫秒单位的时间为分：秒制
	int s=atoi(ms)/1000;	//用秒表示
	if(s==0){	//用时过短，可能是某处出错
		sprintf_s(Src,32,"采样错误");
	}
	else{	//s/60得到分钟数，s%60得到秒数
		sprintf_s(Src,32,"%d:%d",s/60,s%60);
	}
	return Src;
}