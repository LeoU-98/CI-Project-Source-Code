/*
 * IncFile1.h
 *
 * Created: 12/8/2022 6:40:45 PM
 *  Author: Mohamed Tarek
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#define REGISTER_SIZE 8
#define SET_BIT(reg,bit)	reg|=(1<<bit)
#define CLR_BIT(reg,bit)	reg&=(~(1<<bit))
#define TOG_BIT(reg,bit)	reg^=(1<<bit)
#define READ_BIT(reg,bit)	((reg&(1<<bit))>>bit)
#define IS_BIT_SET			((reg&(1<<bit))>>bit)
#define IS_BIT_CLR			!((reg&(1<<bit))>>bit)
#define ROR(reg,num)		reg=(reg<<(REGISTER_SIZE-num))|(reg>>(num))
#define ROL(reg,num)		reg=(reg<<(REGISTER_SIZE-num))|(reg<<(num))

#endif /* INCFILE1_H_ */