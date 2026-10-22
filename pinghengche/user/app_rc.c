#include "app_rc.h"
#include <string.h>
#include <stdio.h>
#include "app_control.h"
#define  CMD_MAX_LEN  64
static char  intBuf[  CMD_MAX_LEN];//用于中断程序
static  char transBuf[  CMD_MAX_LEN];//缓存
static  char  procBuf[  CMD_MAX_LEN];//用于进程函数
static volatile  uint8_t   lineReceivedFlag=0;//一行字符串接收完成的标志位

static  uint16_t  intBufCursor=0;//intBuf的游标，指向下一个空白的位置

//
//对遥控器模块初始化
//

void App_RC_Init(void){
//开启GPIOB的时钟
	
RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	//初始化PB10 AF PP USART3 TX
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_10;
	a.GPIO_Mode =GPIO_Mode_AF_PP ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOB,&a);
	//初始化PB11 IPU USART3 RX
	a.GPIO_Pin =GPIO_Pin_11;
	a.GPIO_Mode =GPIO_Mode_IPU ;
	GPIO_Init (GPIOB,&a);
	
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_USART3 ,ENABLE);
	USART_InitTypeDef b={0};
	b.USART_BaudRate =9600;
	
	b.USART_Mode=USART_Mode_Rx |USART_Mode_Tx ;
	b.USART_Parity=USART_Parity_No ;
	b.USART_StopBits=USART_StopBits_1 ;
	b.USART_WordLength=USART_WordLength_8b ;
	
	USART_Init (USART3,&b);
	
	//开启USART3的RXNE中断
	
	USART_ITConfig(USART3,USART_IT_RXNE ,ENABLE);
	
	NVIC_InitTypeDef   c={0};
	c.NVIC_IRQChannel=USART3_IRQn ;
	c.NVIC_IRQChannelCmd =ENABLE;
	c.NVIC_IRQChannelPreemptionPriority =0;
	c.NVIC_IRQChannelSubPriority =0;
	NVIC_Init (&c);
	//开启总开关
	USART_Cmd (USART3,ENABLE);
	
}

//
//遥控器的进程函数
//
void App_RC_Proc(void){
if(lineReceivedFlag ==1){

strcpy (procBuf ,transBuf );
	lineReceivedFlag =0;//表示缓存数组为空
	//判断指令名称 
	
	if(strncasecmp (procBuf ,"move ",5)==0){
		
		int  turnSpeed,moveSpeed;//转向速度，移动速度
	if(	sscanf(procBuf ,"move %d %d",&turnSpeed ,&moveSpeed )==2){//把参数转给这俩变量，等于2代表成功
		//解析成功
		
		//执行
		App_Control_SetMoveSpeed(-moveSpeed*0.01f*0.7f);
		App_Control_SetTurnSpeed(-turnSpeed*0.01f*15);
		
		
		
		
	}//前五个是这个指令对这个就进行解析
		

	
}

}
}
//
//串口3中断响应函数
//
void USART3_IRQHandler(void){

if(USART_GetFlagStatus (USART3,USART_FLAG_RXNE )==SET){

uint8_t data =USART_ReceiveData (USART3);//先把字符接受进来
	
	if(data!='\n'){
	
	intBuf [intBufCursor ++]=data;
	
	}else{
  	
		//是最后一个元素'\n'  在数组中添加一个
		intBuf [intBufCursor ]='\0';
		intBufCursor =0;//因为接受到最后一个位置了，把游标移到初始值
		
		
		strcpy(transBuf ,intBuf );//把intBuf 的值移动到transBuf
		lineReceivedFlag =1;//表示已经读到字符串了，等待进程函数，把他读出来
	}
}
}