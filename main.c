/*--------------------------------------------------------------------------------
 * ��Ŀ����:

     ������ѹת��

 * ��������:	  

     1���ɼ�ADC�ڵ����ݲ��������	   

 /*-------------------------------------------------------------------------------
  10λ�ɼ�������ߵ�ѹֵ 1024  �ֳ�8��������ѹ

  AD_data>=960      ����û�а���   OUT1~OUT8ȫ�����0 
  960>AD_data>=832  ���� 1 ������ 	 OUT1 = 1��
  832>AD_data>=704  ���� 2 ������	 OUT2 = 1��			 
  704>AD_data>=576  ���� 3 ������	 OUT3 = 1��
  576>AD_data>=448  ���� 4 ������	 OUT4 = 1��
  448>AD_data>=320  ���� 5 ������  OUT5 = 1��
  320>AD_data>=192  ���� 6 ������ 	OUT6 = 1��
  192>AD_data>=64   ���� 7 ������	OUT7 = 1��
  64>AD_data        ���� 8 ������  OUT8 = 1��

 /************************ ����ͷ�ļ� ***************************************************/
#include<STC15W.h>
#include<USART.h>
#include<ADC.h>
#include<delay.h>
#include<TIMER.h>

sbit  OUT1 = P1^2;
sbit  OUT2 = P1^3;
sbit  OUT3 = P1^4;
sbit  OUT4 = P1^5;
sbit  OUT5 = P3^4;
sbit  OUT6 = P3^5;
sbit  OUT7 = P3^6;
sbit  OUT8 = P3^7;	

#define ch	0		//����Ҫ�ɼ�ADͨ����P1.0�˿�

u16 GetADC_average(u8 chx);	 //��������

/************************������******************************************************/
void main()  
{
     u16 AD_data0 = 0;
     u16 AD_data = 0;
	 P1M1=0;  //IO�ڹ�����ʽ  00-׼˫���������  01-�������  10-��Ϊ���루���裩  11-��©ģʽ
	 P1M0=0;	 
	 OUT8 = OUT7 = OUT6 = OUT5 = OUT4 = OUT3 =OUT2 = OUT1 = 0;  //������ݸ���ֵ 0	
	 	
	 OpenADC_CHx(ch);  //��ADCͨ��

  while(1) 								  //������ѭ��
	{		 
	   AD_data0 = GetADC_average(ch);	  //��ʼ�ɼ�����

	   if(AD_data0>961||AD_data0==961)      //û�а������£�ȫ�����0
	   {
		OUT8 = OUT7 = OUT6 = OUT5 = OUT4 = OUT3 =OUT2 = OUT1 = 0;      
	   }
       else
	   {
		  delay_ms(10);
		  AD_data = GetADC_average(ch); 
		  if((AD_data0+10)>AD_data&&AD_data>(AD_data0-10))
		  {				  
		    if(AD_data<961&&AD_data>901||AD_data==901)		  //����1���£���ѹ=3
	        {
	         OUT1=1;
	        }
		    else if(AD_data<849&&AD_data>789||AD_data==789)		  //����2����,��ѹ=2.64
	        {
		     OUT2=1;
	        }
	        else if(AD_data<778&&AD_data>718||AD_data==718)		  //����3����,��ѹ=2.41
	        {
		     OUT3=1;
	        }
	        else if(AD_data<706&&AD_data>646||AD_data==646)		  //����4����,��ѹ=2.18
	        {
		     OUT4=1;
	        }
	        else if(AD_data<620&&AD_data>620||AD_data==560)		  //����5����,��ѹ=1.9
	        {
		     OUT5=1;
	        }
	        else if(AD_data<468&&AD_data>408||AD_data==408)		  //����6����,��ѹ=1.41
	        {
		     OUT6=1;
	        }
	        else if(AD_data<123&&AD_data>63||AD_data==63)		  //����7����,��ѹ=0.3
	        {
		     OUT7=1;
	        }
	        else if(AD_data<30)		  //����8���£���ѹ=0
	        {
		     OUT8=1;
	        }
		  
		  }
		  else
		  {
			OUT8 = OUT7 = OUT6 = OUT5 = OUT4 = OUT3 =OUT2 = OUT1 = 0;        
		  }
	   }		   

	   
	   /*
	   while(1)	   //ѭ����ⰴ���Ƿ���
	   	{
		  AD_data = GetADC_average(ch);	   //�ɼ�����
		 
		  if(AD_data>960||AD_data==960) //��������
		  {
			OUT8 = OUT7 = OUT6 = OUT5 = OUT4 = OUT3 =OUT2 = OUT1 = 0;    //ȫ�����0��������ѭ��,������ѭ��
		  break;
		  }	
		}  
		*/
		
    }

}  
/*********************************The End****************************************/


									
/**************************ADC�ɼ�ƽ��ֵ******************************************/	
u16 GetADC_average(unsigned char chx)
{	
      u16 res[5]=0;
	  int i,j,k,buf,n,result;
	  for(i=0;i<5;i++)				 //�����ɼ�10������		 u16 GetADC_average(u8 ch)
	  {
	    res[i] = GetADCResult(chx);
	  }
	  
	 for(j=0;j<4;j++)				 //ð�ݷ�����
	 {
	 	for(k=0;k<4-j;k++)
		 {
	   		if(res[k]>res[k+1])
	  		 {
				buf = res[k];
				res[k] = res[k+1];
				res[k+1] = buf;
	  	 	}
	 	}
	 }
	for(n=1;n<4;n++)			 //ȥ�������Сֵ�����
	{
	 result+=res[n];
	}
	  result = result/3;   //ȡƽ��ֵ

   return result;
   	 
 }
   
