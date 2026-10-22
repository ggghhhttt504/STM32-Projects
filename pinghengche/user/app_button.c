#include "app_button.h"
#include "button.h"
#include "app_pwm.h"
#include "app_motor.h"
#include "app_control.h"
//#include "app_motor.h"
static  Button_TypeDef userKey;//用户按钮
static  void OnUserKey_Clicked(uint8_t clicks);
//
//@简介：按钮初始化
//
void App_Button_Init(void){

	Button_InitTypeDef  a={0};
	a.GPIOx=GPIOA;
	a.GPIO_Pin =GPIO_Pin_11;
	
My_Button_Init (&userKey,&a);
	
	My_Button_SetClickCb (&userKey ,OnUserKey_Clicked);//设置按钮点击时的回调函数
//每当按钮按下调用回调函数
}
void App_Button_Proc(void){

   My_Button_Proc (&userKey );

}
static  uint8_t pwm_on=0;//0表示电机休眠，1表示电机活动  记录的
//
//@简介：按钮点击的回调函数
//clicks 表示按钮按下的次数
static  void OnUserKey_Clicked(uint8_t clicks){
    if(clicks==1){
	//反转
			App_Control_Reset();//控制系统复位
       if(pwm_on==0){
           pwm_on =1;
          }
			 else{
      pwm_on =0;
       }
      App_Motor_Cmd(pwm_on);
}

}