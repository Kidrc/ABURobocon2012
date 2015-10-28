#include "SetSpeed.h"

/*	��������Wheel_Ctrl
*	
*	2012/03/01
*	���ܣ���Ϊ�ϲ���Ƴ����ٶȡ���̬�͵ײ������ٶȵĽӿڡ�
*		����ģ�͡�
*	������vx,vy,w, ��Ϊ���͡��Ի�������������Ϊԭ�㣬ԭ����ڵ�2��������Ϊy������w�����y��xȡС�Ƕȡ�
*	����ֵ����
*	Version 1.0
*/
void Wheel_Ctrl(double vx, double vy, double w)
{ 
	const double R = 190;
    const double L1 = 330;
    const double L2 = 260;

		SetSpeed(1, (vx + L1*w)*1.3);
		SetSpeed(4, vy - R*w);
		SetSpeed(3, (L2*w - vx)*1.3);
		SetSpeed(2, -R*w - vy);
}
/*
*	��������SetSpeed
*
*	2011/12/2
*	ɾ��û�õ�ע�ͣ�1.1�汾�Ĳ��Խ�����á�
*	��itoa��Ϊ�Լ�����İ汾����ҪGeneralDef.h֧�֡�
*	Version 1.2
*
*	2011/11/17
*	ɾȥ���ٶȲ���������Ч����
*	�������������ڵ��node��8λ�޷���������ȡֵ��[0, 254]�У�0��ʾ�޽ڵ�;
*		  �ٶ�speed, 16λ����
*	Version 1.1
*
*	2011/11/12
*	���ܣ���ָ�����ٶ�����Ŀ�������ٶȣ��������������
*		  ��ҪUSART1��PA9ʹ��
*	����ֵ����
*
*	Version 1.0
*/
void SetSpeed(uint8_t node, int16_t speed)
{
	uint8_t speed_Len = 0;
	char speed_Buf[20];	
	
	if (node != 0)
	{
		itoa(speed_Buf, node, 1);
		
		for (speed_Len = 0; speed_Buf[speed_Len]; speed_Len++);
	}	
	
	speed_Buf[speed_Len++] = 'v';
	
	itoa(speed_Buf + speed_Len, speed, 5);

	USART_Transmit(USART3, speed_Buf);
}

