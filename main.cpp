#include "mbed.h"
#include "AnalogIn.h"
#include "DigitalOut.h"
#include "PinNameAliases.h"
#include "PinNames.h"
#include "ThisThread.h"
#include "Thread.h"
#include "cmsis_os2.h"


// ALIAS
#define Ds   D4
#define STCP D3
#define SHCP D2

#define tiempo  500ms

//PINES Y PUERTOS 
DigitalOut serial_data(Ds);     // correr datos
DigitalOut serial_clk(SHCP);    //reloj correr datos
DigitalOut latchclk(STCP);      // reloj del latch de registro
DigitalOut led(LED1);
//PROTOTIPOS
void shift_data(int data);
void load_data(int data);
void clear_data(void);
void send_data(void);   //funcion del hilo


// HILOS Y HERRAMIENTAS DEL SISTEMA
Thread T_send_data(osPriorityNormal, 4096, NULL, NULL);

// VARIABLES GLOBALES

int patron[12 ] = {0x555555, 0xAAAAAA,0x333333,0xCCCCCC, 0X0F0F0F,0xF0F0F0, 0x00FF00,0XFF00FF,0x000FFF,0xFFF000,0x000000,0xFFFFFF};
int seceuncia1[12]={0x001800,0x002400,0x004200,0x008100,0x010080,0x020040,0x040020,0x080010,0x100008,0x200004,0x400002,0x800001};
int main()
{
    //inicializar
    serial_data = 0;
    serial_clk  = 0;
    latchclk    = 0;
    clear_data();
    wait_us(1e6);
    //encender hilos
    T_send_data.start(send_data);
    while (true)
    {
    }
}

void shift_data(int data)
{
    for (int i=0; i<24; i++)
    {
        serial_data = ((data & 0x01) == 0x01) ? 1:0;
        
        serial_clk = 0;
        wait_us(1);
        serial_clk = 1;
        wait_us(1);
        data = data >> 1;
       
    }
}

void load_data(int data)
{
    latchclk = 0;
    shift_data(data);
    latchclk = 1;
   
}

void clear_data(void)
{
    load_data(0x000000);
}

void send_data(void)
{

    while (true) {

  for ( int h=0; h<1; h++)
    { 
  int data= 0x800000;
  for ( int x=0; x<23; x++)
    { 
        data = data >> 1;
      // data = ~data;
        load_data(data);
      

        wait_us(5e5);
    }
   
   for ( int x=0; x<23; x++)
    { 
      // data = ~data;
        data = data << 1;
        load_data(data);
        wait_us(5e5);
    }
    }
   for ( int k=0; k<1; k++)
   {

   int data= 0x800800;
   for ( int x=0; x<11; x++)
    { 
        data = data >> 1;
      // data = ~data;
        load_data(data);
        wait_us(5e5);

    }
     for ( int x=0; x<11; x++)
    { 
      // data = ~data;
     data = data << 1;
     load_data(data);
     wait_us(5e5);
    }
   }
   for ( int P=0; P<12; P++)
    { 
        int data = seceuncia1[P];
        load_data(data);
        wait_us(5e5);
    } 
    for ( int Ph=11; Ph>=0; Ph--)
    { 
        int data = seceuncia1[Ph];
        
        load_data(data);
        wait_us(5e5);
    }
     for ( int P=0; P<12; P++)
    { 
        int data = patron[P];
        data = ~data;
        load_data(data);
        wait_us(1e6);
    }
    }     
}
