
#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 8000000ul
#include <util/delay.h>

#include "waveform.h"
#include "uart.h"

#define _CMD_START_CNT 1
#define _CMD_END_CNT   1
#define _CMD_WAVE_CNT  1
#define _CMD_AMP_CNT   3
#define _CMD_FRQ_CNT   3

#define FULL_CMD_CNT (_CMD_START_CNT +  _CMD_WAVE_CNT + _CMD_AMP_CNT + _CMD_FRQ_CNT + _CMD_END_CNT)
#define WAVE_OFFSET  (_CMD_START_CNT)
#define AMP_OFFSET   (_CMD_START_CNT +  _CMD_WAVE_CNT)
#define FREQ_OFFSET  (_CMD_START_CNT +  _CMD_WAVE_CNT + _CMD_AMP_CNT)
#define MARKER_END   (_CMD_START_CNT +  _CMD_WAVE_CNT + _CMD_AMP_CNT + _CMD_FRQ_CNT)
#define MARKER_START (0)

#define WAVEFORM_NUM 6

#define DAC_DDR  DDRB
#define DAC_PORT PORTB

typedef enum {GENERATE_WAVE, UPDATE_WAVE} states_t;

static uint8_t cmd_buffer[FULL_CMD_CNT];
static void (*waveform[WAVEFORM_NUM])(uint8_t amp, uint8_t freq);
static states_t currentState = GENERATE_WAVE;
static uint8_t amp_value = 0;
static uint8_t freq_value = 0;
static uint8_t waveform_index = WAVEFORM_NUM;

#define BAUD_RATE 9600

volatile unsigned char arr_sine[]={
	128,136,143,151,159,167,174,182,189,196,202,209,215,220,226,231,235,239,243,246,
	249,251,253,254,255,255,255,254,253,251,249,246,243,239,235,231,226,220,215,209,
	202,196,189,182,174,167,159,151,143,136,128,119,112,104,96,88,81,73,66,59,
	53,46,40,35,29,24,20,16,12,9,6,4,2,1,0,0,0,1,2,4,
	6,9,12,16,20,24,29,35,40,46,53,59,66,73,81,88,96,104,112,119
};
	
	volatile unsigned char arr_ramp[]={
		0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 
		120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 
		220, 225, 230, 235, 240, 245, 250
		};
	
	volatile unsigned char arr_sawtooh[]={
		255, 250, 245, 240, 235, 230, 225, 220, 215, 210, 205, 200, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145,
		 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 
		 10, 5, 0, };
	volatile unsigned char arr_triangle[]={
		0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 
		130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220, 225, 230, 235,
		 240, 245, 250, 255, 255, 250, 245, 240, 235, 230, 225, 220, 215, 210, 205, 200, 195, 190, 185, 180, 175, 170,
		  165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 
		  45, 40, 35, 30, 25, 20, 15, 10, 5, 0
		};
	
volatile void squareWave(uint8_t amp, uint8_t freq)
{
	volatile double time=1.0/(double)freq;
	volatile int samples=2;
	volatile double time_per_sample=time/(double) samples;
	volatile double time_per_sample_us=1000000*time_per_sample/2.3;
	uint32_t x=(uint32_t)time_per_sample_us;
	DAC_PORT=(uint32_t)(255*amp /255);
	
	for(uint32_t i=0;i<x;i++)
	{ _delay_us(1);}
	DAC_PORT=(0);
	for(uint32_t i=0;i<x;i++)
	{_delay_us(1);}

}

void staircaseWave(uint8_t amp, uint8_t freq)
{
volatile double time=1.0/(double)freq;
volatile int samples=6;
volatile double time_per_sample=time/(double) samples;
volatile double time_per_sample_us=1000000*time_per_sample/2.3;
uint32_t x=(uint32_t)time_per_sample_us;
	
    DAC_PORT = (int)(amp*0 /255);
	for(uint32_t i=0;i<x;i++)
	{_delay_us(1);}
    DAC_PORT = (int)(amp*51 /255);
		for(uint32_t i=0;i<x;i++)
	{_delay_us(1);}
    DAC_PORT = (int)(amp*102 /255);
    	for(uint32_t i=0;i<x;i++)
    {_delay_us(1);}
    DAC_PORT = (int)(amp*153 /255);
    	for(uint32_t i=0;i<x;i++)
    {_delay_us(1);}
    DAC_PORT = (int)(amp*204 /255);
    	for(uint32_t i=0;i<x;i++)
    {_delay_us(1);}
    DAC_PORT =(int)(amp*255 /255);
    	for(uint32_t i=0;i<x;i++)
    {_delay_us(1);}
}

void triangleWave(uint8_t amp, uint8_t freq)
{
volatile double time=1.0/(double)freq;
volatile int samples=104;
volatile double time_per_sample=time/(double) samples;
volatile double time_per_sample_us=1000000*time_per_sample/10;
uint32_t x=(uint32_t)time_per_sample_us;

	
	for(int Local_u8Iterator = 0 ; Local_u8Iterator < 104 ; Local_u8Iterator ++)
	{
		DAC_PORT = (int)((amp*arr_triangle[Local_u8Iterator])/255) ;
			for(uint32_t i=0;i<x;i++)
			{_delay_us(1);}
	}		

		

}

volatile void sineWave(uint8_t amp, uint8_t freq)
{
volatile uint32_t x=(uint32_t) (810.0/(double)freq);

	
		for(int Local_u8Iterator = 0 ; Local_u8Iterator < 100 ; Local_u8Iterator ++)
		{
			DAC_PORT = (int)((amp*arr_sine[Local_u8Iterator])/255) ;
				for(uint32_t i=0;i<x;i++)
				{_delay_us(1);}
		}
		
 
}
volatile void ramp(uint8_t amp, uint8_t freq)
{
volatile double time=1.0/(double)freq;
volatile int samples=51;
volatile double time_per_sample=time/(double) samples;
volatile double time_per_sample_us=1000000*time_per_sample/3.8;
uint32_t x=(uint32_t)time_per_sample_us;
	
	for(int Local_u8Iterator = 0 ; Local_u8Iterator < 51 ; Local_u8Iterator ++)
	{
		DAC_PORT = (int)((amp*arr_ramp[Local_u8Iterator])/255) ;
		for(uint32_t i=0;i<x;i++) _delay_us(1);
	}
}
volatile void sawtooth(uint8_t amp, uint8_t freq)
{
	volatile double time=1.0/(double)freq;
	volatile int samples=51;
	volatile double time_per_sample=time/(double) samples;
	volatile double time_per_sample_us=1000000*time_per_sample/3.8;
	uint32_t x=(uint32_t)time_per_sample_us;
	
	for(int Local_u8Iterator = 0 ; Local_u8Iterator < 51 ; Local_u8Iterator ++)
	{
		DAC_PORT = (int)((amp*arr_sawtooh[Local_u8Iterator])/255) ;
		for(uint32_t i=0;i<x;i++)
		{_delay_us(1);}
	}
}

void WAVE_Init(void)
{
    uint8_t i;
    
    /* Init UART driver. */
    UART_cfg my_uart_cfg;
    
    /* Set USART mode. */
    my_uart_cfg.UBRRL_cfg = (BAUD_RATE_VALUE)&0x00FF;
    my_uart_cfg.UBRRH_cfg = (((BAUD_RATE_VALUE)&0xFF00)>>8);
    
    my_uart_cfg.UCSRA_cfg = 0;
    my_uart_cfg.UCSRB_cfg = (1<<RXEN)  | (1<<TXEN) | (1<<TXCIE) | (1<<RXCIE);
    my_uart_cfg.UCSRC_cfg = (1<<URSEL) | (3<<UCSZ0);
    
    UART_Init(&my_uart_cfg);
    
    
    /* Clear cmd_buffer. */
    for(i = 0; i < FULL_CMD_CNT; i += 1)
    {
        cmd_buffer[i] = 0;
    }
    
    /* Initialize waveform array. */
    waveform[0] = squareWave;
    waveform[1] = staircaseWave;
    waveform[2] = triangleWave;
    waveform[3] = sineWave;
	waveform[4] = ramp;
	waveform[5] = sawtooth;

    /* Start with getting which wave to generate. */ 
    currentState = UPDATE_WAVE; 
	DAC_DDR =0xff;
}
int g_flag=0;
int g_flag2=0;
void WAVE_MainFunction(void)
{    
	

    // Main function must have two states,
    // First state is command parsing and waveform selection.
    // second state is waveform executing.
    switch(currentState)
    {
        case UPDATE_WAVE:
        {
            UART_SendPayload((uint8_t *)">", 1);
            while (0 == UART_IsTxComplete());

            /* Receive the full buffer command. */
			if(g_flag==0)
            {
				UART_ReceivePayload(cmd_buffer, FULL_CMD_CNT);
				g_flag=1;
				/* Pull unitl reception is complete. */
				while(0 == UART_IsRxComplete());
			}
            
            

            /* Check if the cmd is valid. */
            if((cmd_buffer[MARKER_START] == '@') && (cmd_buffer[MARKER_END] == ';'))
            {
                // Extract amplitude and freq values before sending them to the waveform generator.
                /* Compute amplitude. */
                {
                    char _buffer[_CMD_AMP_CNT];
                    for(uint8_t i = 0; i < _CMD_AMP_CNT; ++i) { _buffer[i] = cmd_buffer[AMP_OFFSET+i]; }
                    amp_value = atoi(_buffer);
                }

                /* Compute frequency. */
                {
                    char _buffer[_CMD_FRQ_CNT];
                    for(uint8_t i = 0; i < _CMD_FRQ_CNT; ++i) { _buffer[i] = cmd_buffer[FREQ_OFFSET+i]; }
                    freq_value = atoi(_buffer);
                }

                /* Compute waveform. */
                {
                    waveform_index = cmd_buffer[WAVE_OFFSET] - '0';
					g_flag2=0;
                }
            } 
            else
            {
               /* Clear cmd_buffer. */
                for(uint8_t i = 0; i < FULL_CMD_CNT; i += 1)
                {
                    cmd_buffer[i] = 0;
                }
            }

            // Trigger a new reception.
            UART_ReceivePayload(cmd_buffer, FULL_CMD_CNT);

            UART_SendPayload((uint8_t *)"\r>", 2);
            while (0 == UART_IsTxComplete());
        }
        case GENERATE_WAVE:
        {
            // Execute waveform..
            if(waveform_index < WAVEFORM_NUM)
            {
                waveform[waveform_index](amp_value, freq_value);
            }
            // Keep in generate wave if no command it received.
			if (g_flag2==0)
			{
				switch(waveform_index)
				{
					case 0: UART_SendPayload((uint8_t *)"0", 1); while (0 == UART_IsTxComplete()); break;
					case 1: UART_SendPayload((uint8_t *)"1", 1); while (0 == UART_IsTxComplete()); break;
					case 2: UART_SendPayload((uint8_t *)"2", 1); while (0 == UART_IsTxComplete()); break;
					case 3: UART_SendPayload((uint8_t *)"3", 1); while (0 == UART_IsTxComplete()); break;
					case 4: UART_SendPayload((uint8_t *)"4", 1); while (0 == UART_IsTxComplete()); break;
					case 5: UART_SendPayload((uint8_t *)"5", 1); while (0 == UART_IsTxComplete()); break;
					case 6:break;
				}
				g_flag2=1;
			}
			
            currentState = (1 == UART_IsRxComplete()) ? UPDATE_WAVE : GENERATE_WAVE;
            break;
        }
        default: {/* Do nothing.*/}
    }
}

