#include "mpu6050_test.h"
#include "app_usart2.h"
#include "app_mpu6050.h"
#include "delay.h"
#include "task.h "

void MPU6050_TEST(void){
	App_USART2_Init ();
	App_MPU6050_Init ();
	
	while(1){
		
		App_MPU6050_Update() ;
float ax=App_MPU6050_GetAx ();
			float ay=App_MPU6050_GetAy ();
			float az=App_MPU6050_GetAz ();
			float gx=App_MPU6050_GetGx ();
			float gy=App_MPU6050_GetGy ();
			float gz=App_MPU6050_GetGz ();
			float temperature =App_MPU6050_GetTemperature ();
		My_USART_Printf (USART2,"%f,%f,%f,%f,%f,%f,%f\n",ax,ay,az,temperature ,gx,gy,gz);
		Delay(100);
	}
	
}

static  void USART2_Proc(void);
//
//测试欧拉角
//

void MPU6050_EularAngleTest(){
	App_USART2_Init ();
	App_MPU6050_Init ();
	

while(1){
    App_MPU6050_Proc();
	USART2_Proc();
}

}

//
//简介：每间隔10ms把欧拉角的计算结果发送给电脑
//

static  void USART2_Proc(void){
   PERIODIC (10);
float ax=	App_MPU6050_GetAx ();
float ay=	App_MPU6050_GetAy ();
float az=	App_MPU6050_GetAz ();
float gx=	App_MPU6050_GetGx ();
float gy=	App_MPU6050_GetGy ();
float gz=	App_MPU6050_GetGz ();
float temperature=	App_MPU6050_GetTemperature ();
float yaw=	App_MPU6050_GetYaw ();
float pitch=	App_MPU6050_GetPitch ();
float roll=	App_MPU6050_GetRoll ();
	
	My_USART_Printf (USART2,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",ax,ay,az,temperature,gx,gy,gz,yaw,pitch,roll);


}



