/*--------------------------------------------------------------------------------
 * 项目名称:

     按键电压转换

 * 功能描述:	  

     1、采集ADC口的数据并输出数据	   

 /*-------------------------------------------------------------------------------
  10位采集精度最高电压值 1024  分成8个按键分压

  AD_data>=960      按键没有按下   OUT1~OUT8全部输出0 
  960>AD_data>=832  按键 1 被按下 	 OUT1 = 1；
  832>AD_data>=704  按键 2 被按下	 OUT2 = 1；			 
  704>AD_data>=576  按键 3 被按下	 OUT3 = 1；
  576>AD_data>=448  按键 4 被按下	 OUT4 = 1；
  448>AD_data>=320  按键 5 被按下  OUT5 = 1；
  320>AD_data>=192  按键 6 被按下 	OUT6 = 1；
  192>AD_data>=64   按键 7 被按下	OUT7 = 1；
  64>AD_data        按键 8 被按下  OUT8 = 1；

 /************************ 包含头文件 ***************************************************/
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

#define ch	0		//定义要采集AD通道即P1.0端口

u16 GetADC_average(u8 chx);	 //函数声明

/************************主函数******************************************************/
void main()  
{
     u16 AD_data0 = 0;
     u16 AD_data = 0;
	 P1M1=0;  //IO口工作方式  00-准双向输入输出  01-推完输出  10-仅为输入（高阻）  11-开漏模式
	 P1M0=0;	 
	 OUT8 = OUT7 = OUT6 = OUT5 = OUT4 = OUT3 =OUT2 = OUT1 = 0;  //输出数据赋初值 0	
	 	
	 OpenADC_CHx(ch);  //打开ADC通道

  while(1) 								  //进入主循环
	{		 
	   AD_data0 = GetADC_average(ch);	  //开始采集数据

	   if(AD_data0>961||AD_data0==961)      //没有按键按下，全部输出0
	   {
		OUT8 = OUT7 = OUT6 = OUT5 = OUT4 = OUT3 =OUT2 = OUT1 = 0;      
	   }
       else
	   {
		  delay_ms(10);
		  AD_data = GetADC_average(ch); 
		  if((AD_data0+10)>AD_data&&AD_data>(AD_data0-10))
		  {				  
		    if(AD_data<961&&AD_data>901||AD_data==901)		  //按键1按下，电压=3
	        {
	         OUT1=1;
	        }
		    else if(AD_data<849&&AD_data>789||AD_data==789)		  //按键2按下,电压=2.64
	        {
		     OUT2=1;
	        }
	        else if(AD_data<778&&AD_data>718||AD_data==718)		  //按键3按下,电压=2.41
	        {
		     OUT3=1;
	        }
	        else if(AD_data<706&&AD_data>646||AD_data==646)		  //按键4按下,电压=2.18
	        {
		     OUT4=1;
	        }
	        else if(AD_data<620&&AD_data>620||AD_data==560)		  //按键5按下,电压=1.9
	        {
		     OUT5=1;
	        }
	        else if(AD_data<468&&AD_data>408||AD_data==408)		  //按键6按下,电压=1.41
	        {
		     OUT6=1;
	        }
	        else if(AD_data<123&&AD_data>63||AD_data==63)		  //按键7按下,电压=0.3
	        {
		     OUT7=1;
	        }
	        else if(AD_data<30)		  //按键8按下，电压=0
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
	   while(1)	   //循环检测按键是否弹起
	   	{
		  AD_data = GetADC_average(ch);	   //采集数据
		 
		  if(AD_data>960||AD_data==960) //按键弹起
		  {
			OUT8 = OUT7 = OUT6 = OUT5 = OUT4 = OUT3 =OUT2 = OUT1 = 0;    //全部输出0，并跳出循环,返回主循环
		  break;
		  }	
		}  
		*/
		
    }

}  
/*********************************The End****************************************/


									
/**************************ADC采集平均值******************************************/	
u16 GetADC_average(unsigned char chx)
{	
      u16 res[5]=0;
	  int i,j,k,buf,n,result;
	  for(i=0;i<5;i++)				 //连续采集10个数据		 u16 GetADC_average(u8 ch)
	  {
	    res[i] = GetADCResult(chx);
	  }
	  
	 for(j=0;j<4;j++)				 //冒泡法排序
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
	for(n=1;n<4;n++)			 //去掉最大最小值并求和
	{
	 result+=res[n];
	}
	  result = result/3;   //取平均值

   return result;
   	 
 }
   
