#include <system_timer.h>
#include <sio.h>
#include <aduc812.h>
#include <vect.h>
#include <datatypes.h>
#include <lcd.h>
#include <kb.h>
#include <system_timer.h>
#include <help.h>
#include <max.h>



u16 cnt = 0;
time cur_ms;
void init_t();
int read_keyboard();

void T2_ISR( void ) __interrupt ( 2 );

void initialize_system_timer() {
	init_t();

	cur_ms = 0;
	cnt = 0;
	SetVector(0x202B, (void *)T2_ISR);
	TH2 = (-1000) >> 8; // TH2 (highest 8 bits of timer 2 counting register) = 1111 1100
	TL2 = (-1000) & 0xFF; // TL2 (lowest 8 bits of timer 2 counting register) = 0001 1000
	// temporarily store the contents of the counter register
	RCAP2H = (-1000) >> 8; // RCAP2H (highest 8 bits of timer 2 capture register) = 1111 1100
	RCAP2L = (-1000) & 0xFF; // RCAP2L (lowest 8 bits of timer 2 capture register) = 0001 1000
	ET2=1;	// enable (interrupt from) timer 2
	TR2=1;  // start timer 2
}
time get_ms(void){
	return cur_ms;
}
time get_ms_after(time t0){
	return cur_ms-t0;
}
void delay_ms(time t){
	time now = get_ms();
	while(get_ms_after(now) < t){}
}

float t, target_t;

void init_t() {
	t = 20.0;
	target_t = 5.0;
}

void T2_ISR( void ) __interrupt ( 2 ) {
	int i;
	u8 dips;
	char string[10];
	cur_ms++;

	// trash :)
	// TH2 = 0xFF;
	// TL2 = 0xFF;
	// RCAP2H = 0xFF;
	// RCAP2L = 0xFF;



	// each second:
	if (cur_ms % 1000 == 0) {
		// type("TIMER 2\n");

		// 1. изменить температуру t -> target_t
		// 2. вывести температуру t
		// 3. проверить dip-ы
		// 4. если включен dip, то ждем ввода с клавиатуры,
		//    иначе ничего не делаем
		// 5. пользователь вводит 2 цифры -- число град.
		// 6. запоминаем новый таргет. Продолжаем работу.

		// 1.
		if (t < target_t)
	        t += 1.0;
	    else
	        t -= 1.0;

	    // 2.1
	    int_to_string(t, string, 10);
	    type(string);
	    type("\n");

	    // 2.2
		LCD_Clear();
		LCD_Type(string);

		for (i = 0; i < 6; ++i);



	    // 3.
	    dips = ReadMax(EXT_LO);
	    print_byte(dips);
	    if (dips & 1)
	    	return;
	    print_byte(dips);

	    // 4.
	    // 5.
	    target_t = (float) read_keyboard();
	}
}


int read_keyboard() {
	// int i;
	char string[10];
	int result, digit;
	char ch;
	bit sign;
	bit new;

	type("read_keyboard()\n");

	LCD_Clear();
	LCD_Type("Please,");
	LCD_GotoXY(0, 1);
	LCD_Type("enter temperature");


	result = 0;
	sign = 0; // +
	new = 0;

	while (1) {
		if( ScanKBOnce(&ch) ) {
        	// Была нажата какая-то клавиша
			wsio(ch);

			if ('0' <= ch && ch <= '9') {
            	// Была нажата цифра

            	// Интерпретируем символ как цифру:
				digit = ch - '0';
				result *= 10;
				result += (int) digit;

				// Вывести на экран текущий символ
				// LCD_Putch(ch);
				new = 1;
			}
			else if (ch == '#') {
				if (sign == 1)
					sign = 0;
				else
					sign = 1;

				new = 1;
			}
			else if (ch == 'C') {
				sign = 0;
				result = 0;
				new = 1;
			}
			else if (ch == 'A') {
            	// Ввод завершен
				type("Reading keyboard completed\n");
				break;
			}
		}

		if (new) {
			new = 0;
			// Вывести на экран текущую строку:
			int_to_string(sign ? -result : result, string, 10);
			LCD_Clear();
			LCD_Type(string);
		}
	}

	if (sign)
		result *= -1;

	return result;
}