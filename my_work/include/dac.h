#ifndef __DAC__H
#define __DAC__H

#define _8BIT   1
#define _12BIT  0

#define DAC0    0
#define DAC1    1

#define ON      1
#define OFF     0

void init_dac(bit mode);
void switch_dac( bit channel, bit state );
void set_voltage( float v, bit channel );

#endif //__DAC__H
