#include "math.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#define CPAC (*((volatile uint32_t *)0xE000ED88))
void SystemInit()
{
	CPAC |= 0X00F00000;
}
#include "UART.h" 
#include "led.h"
#include "distance.h"
#include "delay.h"
#include "lcd_init.h"
#include "lcd_cmd_data.h"
#include "lcd_tracking.h"
#include "constants_phase1.h"
#define  PI  3.141592654
#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08

float coords_obtained[200][2];
uint16_t indx1=0;
//converting a float to string function
void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}
  
// Converts a given integer x to string str[]. 
// d is the number of digits required in the output. 
// If d is more than the number of digits in x, 
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
  
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
  
    reverse(str, i);
    str[i] = '\0';
    return i;
}
  
// Converts a floating-point/double number to a string.
void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
  
    // Extract floating part
    float fpart = n - (float)ipart;
  
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
  
    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot
  
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter 
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
  
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
//read the char enterd from the pc keyboard through uart0
char read_char()
{
	char c;
	c = UART0_Read();
	return c;
}
// print character by character
void print_char(uint8_t c)
{
	UART0_Write(c);
}
// print on serial window
void print_string(char *string)
{
	while(*string)
		{
			print_char(*(string++));
		}
}
//parse the coords
void read_coords()
{
	char all_data[90],parsed_values[12][50];//we have 12 values for our data 
	char *ptr ;
	char *lattt;
	char *longgg;
	const char delimiter[2]=",";
	uint32_t deg = 0,t=0;
	float lat = 0.0, longt = 0.0, mint = 0.0, parsed_lat = 0.0, parsed_longt = 0.0;
	char a,b,c,d,e,f,g,h;
	
	uint8_t indx=0;
	//while((UART7_FR_R & 0x10)!=0);
	a = UART5_Read();//(UART7_DR_R&0xFF);
	//I will use $GPRMC – Essential GPS pvt (position, velocity, time) data
	//here we will check for $GPRMC, and get the values after it
	if(a == '$')
	{
		//while((UART7_FR_R & 0x10)!=0);
		b = UART5_Read();//(UART7_DR_R&0xFF);
 		if (b == 'G')
		{
			//while((UART7_FR_R & 0x10)!=0);
			c = UART5_Read();//(UART7_DR_R&0xFF);
			if (c == 'P')
			{
				//while((UART7_FR_R & 0x10)!=0);
				d = UART5_Read();//(UART7_DR_R&0xFF);
				if (d == 'R')
				{
					//while((UART7_FR_R & 0x10)!=0);
					e = UART5_Read();//(UART7_DR_R&0xFF);
					if (e == 'M')
					{
						//while((UART7_FR_R & 0x10)!=0);
						f = UART5_Read();//(UART7_DR_R&0xFF);
						if (f == 'C')
						{
							//while((UART7_FR_R & 0x10)!=0);
							g = UART5_Read();//(UART7_DR_R&0xFF);
							if (g == ',')
							{
								//while((UART7_FR_R & 0x10)!=0);
								h = UART5_Read();//(UART7_DR_R&0xFF);
								
								while(h != '*')//getting all useful values from gps until we reah the *
								{
									all_data[indx] = h; 
									
									//while((UART7_FR_R & 0x10)!=0);
									h = UART5_Read();//(UART7_DR_R&0xFF);
									indx++;
								}
								//excluding all the commas from our array all_data
								indx = 0;
								ptr = strtok(all_data,delimiter);
								while(ptr != NULL)
								{
									strcpy(parsed_values[indx],ptr);
									ptr = strtok(NULL,delimiter);
									indx++;
								}
								//if the parsed_value[1] == A the data was read succefully if it was V then something went wrong
								if(strcmp(parsed_values[1] , "A")==0)
								{
									
									lat = atof(parsed_values[2]);//DDMM.MMMM
									longt = atof(parsed_values[4]);
									
									//transform the lattiude to the form used in distance function
									deg = lat/100;
									mint = lat - (float) (deg*100);
									mint = mint/60;
									parsed_lat = deg+ mint;
									
									//transform the longitude to the form used in distance function
									deg = longt/100;
									mint = longt - (float) (deg*100);
									mint = mint/60;
									parsed_longt = deg+ mint;
									//save the whole coordinates obtained in the 2d coords_obtained[] array
									/*ftoa(parsed_lat,lattt,10);
									ftoa(parsed_lat,longgg,10);
									do{
										print_char(lattt[t]);
										print_char(longgg[t]);
										t++;
									}while(t<13);*/
									coords_obtained[indx1][0] = parsed_lat;
									coords_obtained[indx1][1] = parsed_longt;
									indx1++;
								}
								/*else
								{
									printf("GPS still reading");
								}*/
							}
						}
					}
				}
			}
		}
	}
}

int main(){
		long double walked_dist;
		volatile unsigned long delay;
		char out;
		uint32_t word_count ;
	  int i=0,j=0;
		portf_init();
		UART5_Init();//for GPS 
		UART0_Init();//for PC
		lcd_init();
		LCD_Cmd(Function_set_8bit); /* Select 8-bit Mode of LCD */
		SysTick_Wait_1ms(10);
		LCD_Cmd(DisplayOn);  /* Enable Display and cursor blinking */
		SysTick_Wait_1ms(10);
		LCD_Cmd(0x01); /* clear whatever is written on display */
		while(1)
		{
			read_coords();
			i++;
			if(i>198)
				break;
		//	out = UART5_Read();
		//	print_char(out);
			//SysTick_Wait_1ms(1000);
		/*	if((GPIO_PORTF_DATA_R & 0x11) == 0x10 || (GPIO_PORTF_DATA_R & 0x11) == 0x01) //when switch 1 or 2 are pressed we save the data
		{
			// ENABLE THE CLOCK FOR THE EEPROM to save my array of coords
			SYSCTL_RCGCEEPROM_R = 0X1;
			delay = 1;
			delay = 2;
			delay = 3;
			SysTick_Wait_1ms(1000);
			// INITIALIZATION OF EEPROM
			//CHECKING IF THE EEPROM IS NOT WORKING on another operation
			while(!((EEPROM_EEDONE_R & 0x1) == 0)) ;
			//checking if any errors ocurred and handle them 
			if((EEPROM_EESUPP_R&0X2)==1 || (EEPROM_EESUPP_R & 0x4)==1 || (EEPROM_EESUPP_R & 0x8) == 1)
				EEPROM_EESUPP_R |= 0x1;
			//mass erase the whole eeprom
			EEPROM_EEDBGME_R |= 0xE37B0000;
			EEPROM_EEDBGME_R |= 0x1;			
			SysTick_Wait_1ms(20);
			//CHECKING IF THE EEPROM finished the erase
			while(!((EEPROM_EEDONE_R & 0x1) == 0)) ;
			//we will adjust the block and offset 
			EEPROM_EEBLOCK_R = 0x0;
			EEPROM_EEOFFSET_R = 0x0;
			word_count = 400;//float 2d array of 200 row and 2 colums
			while(word_count)
			{
				EEPROM_EERDWRINC_R = coords_obtained[i][0];
				SysTick_Wait_1ms(20);
				while(!((EEPROM_EEDONE_R & 0x1) == 0)) ;
				EEPROM_EERDWRINC_R = coords_obtained[i][1];
				SysTick_Wait_1ms(20);
				while(!((EEPROM_EEDONE_R & 0x1) == 0)) ;
				i++;
				word_count-=2;
				if(word_count && (EEPROM_EEOFFSET_R == 0))
				{
					EEPROM_EEBLOCK_R +=1;
				}
			}
			while(!((EEPROM_EEDONE_R & 0x1) == 0)) ;
			GPIO_PORTF_DATA_R = GREEN;
			break;
		}*/
		}
		for( j=0;j<200;j+=2){
			walked_dist = distance(coords_obtained[j][1],coords_obtained[j+1][1],coords_obtained[j][0],coords_obtained[j+1][0]);// walked distance in meters
		}
			if(walked_dist>=100){
			lcd_tracking1(walked_dist);
			//blue led
			led_on();
			}
			else
				{
					GPIO_PORTF_DATA_R = RED;
				}
	
	}
