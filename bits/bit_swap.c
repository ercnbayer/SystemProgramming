
#include <stdio.h>
#include <stdlib.h>
#define BIT1 1
#define BIT2 2
#define BIT3 4
#define BIT4 8
#define BIT5 16
#define BIT6 32
#define BIT7 64
#define BIT8 128
 typedef struct bitfield{
    unsigned  bit1:1;
    unsigned  bit2:1;
    unsigned  bit3:1;
    unsigned  bit4:1;
    unsigned  bit5:1;
    unsigned  bit6:1;
    unsigned  bit7:1;
    unsigned  bit8:1;
}bitfield;

typedef union num{
    bitfield bits;
    unsigned char number;
}num;
void  bit_swap(int x,int y,int *val){
	// herhangi iki biti swaplayan fonksiyon //val de ilk aldýðýmýz deðer //val is the initial value ,this is the function swaps any two bits in int number
    if( x <1 || y < 1  ||(x>64 || y >64)){
        printf("out of range\n");
        return;
    }
    int value=*val;// store val's value to local int so we dont have to deal with asterisk.
    if(y>x){// if y > x  => swap x and y  i made this function x>y based
        x=y+x; 
        y=x-y;
        x=x-y;
    }
    int bit_x=1<<(x-1);// get the BITX number
    int bit_y=1<<(y-1);// get the BITY number
    int bit_val_x=value &bit_x;//  get the bits value
    int bit_val_y=value & bit_y;// 
    value&=(~bit_x);// zero the initial value's bitx bit
    value&=(~bit_y);// zero the inital value's bity  bit
    int  orValue=bit_val_y<<(x-y)|bit_val_x >>(x-y);// get the orValue which means when we or it with zeroed value( its the same value but desired bits are 0)
    *val=value|orValue;//at the end store the new value in pointer
}
int main(){
      
      char deger=209;
      bit_swap(4,8,(int*)&deger);// little type casting
      bit_swap(3,7,(int*)&deger);
      char sonuc =29;
      printf("%d ==%d \n",deger,sonuc);
}
