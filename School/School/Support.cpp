#include "stdafx.h"

char * MsToTime(char * ms,char * Src){		//ת�����뵥λ��ʱ��Ϊ�֣�����
	int s=atoi(ms)/1000;	//�����ʾ
	if(s==0){	//��ʱ���̣�������ĳ������
		sprintf_s(Src,32,"��������");
	}
	else{	//s/60�õ���������s%60�õ�����
		sprintf_s(Src,32,"%d:%d",s/60,s%60);
	}
	return Src;
}