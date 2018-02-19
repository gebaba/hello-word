#ifndef  _CAL_8302_DATA_H_
#define  _CAL_8302_DATA_H_

	#ifndef	CAL8302TEXT
		#define CAL8302TEXT extern
	#else
		#define CAL8302TEXT 
	#endif
		
#include "math.h"		
#include "Global.h"
		
#define UdivN 0X11 
#define IdivN 0X22
#define IdivN4 0X33
#define PdivN 0X44

#define I_N_ZERO  ((u16)20)//N�����<20mA
//////��ֵ����//////
typedef  struct{
	u16 adds;   //��spi�Ĵ����еĵ�ַ
	u8 curIndex;//��ǰINST_TYPE�������е�������������ӦVECTOR_BIT_TYPE�����ÿ�����ݵ�����
	u8 Sum;//��ǰINST_TYPE����ĳ���
	s32* eleAdds;// RAM��ַpo, 
	u8 SpiLen;	 // 	SPI ���ݳ�
	u8 eleLen;  // ��׼645���ݳ��ȶ��������λһ��
	u16 mul;    //
	u32 div;
	u8 signedF;
	u32 minV;
	u32 maxV;
	u32 defaultV;
	u8 signNo;// ԭʼ���ݵķ���λ��0:1�ֽڣ�1:2�ֽ�  2:3�ֽ�3:28λ 4:4�ֽ�
}TN8302_S;

extern const TN8302_S Tn8302d[];
extern const u32 signPos[5];
extern const u32 signPosM[5];
extern const u32 signPosByte[5];

void UpdateSign(void);//���·���
s32 Read8302Data(u16 n);//����˲ʱ��
void InstanceProc(void);
void GetOtherIns(void);



#endif
