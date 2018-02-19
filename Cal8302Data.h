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

#define I_N_ZERO  ((u16)20)//N相电流<20mA
//////零值处理//////
typedef  struct{
	u16 adds;   //在spi寄存器中的地址
	u8 curIndex;//当前INST_TYPE在数组中的索引，索引对应VECTOR_BIT_TYPE里面的每个数据的正负
	u8 Sum;//当前INST_TYPE数组的长度
	s32* eleAdds;// RAM首址po, 
	u8 SpiLen;	 // 	SPI 数据长
	u8 eleLen;  // 标准645数据长度对最络符号位一致
	u16 mul;    //
	u32 div;
	u8 signedF;
	u32 minV;
	u32 maxV;
	u32 defaultV;
	u8 signNo;// 原始数据的符号位置0:1字节；1:2字节  2:3字节3:28位 4:4字节
}TN8302_S;

extern const TN8302_S Tn8302d[];
extern const u32 signPos[5];
extern const u32 signPosM[5];
extern const u32 signPosByte[5];

void UpdateSign(void);//更新符号
s32 Read8302Data(u16 n);//计算瞬时量
void InstanceProc(void);
void GetOtherIns(void);



#endif
