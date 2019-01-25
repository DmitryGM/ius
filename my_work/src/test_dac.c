/****************************************************************************

test_sio.c - тест драйверов ЦАП и АЦП
для учебного стенда SDK-1.1

(C) test_sio.c, Ключев А.О. 2007 г.

Это свободная программа; вы можете повторно распространять ее и/или
модифицировать ее в соответствии с Универсальной Общественной
Лицензией GNU, опубликованной Фондом Свободного ПО; либо версии 2,
либо (по вашему выбору) любой более поздней версии.

Эта программа распространяется в надежде, что она будет полезной,
но БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ; даже без подразумеваемых гарантий
КОММЕРЧЕСКОЙ ЦЕННОСТИ или ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ. Для
получения подробных сведений смотрите Универсальную Общественную
Лицензию GNU.

Вы должны были получить копию Универсальной Общественной Лицензии
GNU вместе с этой программой; если нет, напишите по адресу: Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA

----------------------------------------------------------------------------
Россия, Санкт-Петербург, кафедра вычислительной техники СПбГУИТМО
e-mail: kluchev@d1.ifmo.ru

****************************************************************************/
#include <aduc812.h>
#include <sio.h>
#include <dac.h>
#include <adc.h>
#include <stdio.h>
#include <max.h>
#include <lcd.h>
#include <buzz.h>
#include <kb.h>
#include <vect.h>

static float v[ 2 ] = { 0.0, 0.0 };
static float current_temperature;
static int target_temperature;


//////////////////////// T0_ISR //////////////////////////////
// Обработчик прерывания от таймера 0.
// Вход: нет.
// Выход: нет.
// Результат: нет.
//////////////////////////////////////////////////////////////
void T0_ISR( void ) __interrupt ( 1 )
{
    // Do some work

    if (current_temperature < (float)target_temperature)
    {
        current_temperature += 0.1;
    }
    else
    {
        current_temperature -= 0.1;
    }

    // Для имитации будем использовать 0-й канал ЦАП
    set_voltage(current_temperature, DAC0);
}

void init_timer()
{
    // Инициализация таймера 0

    // Задаем частоту таймера
    TH0 = 0xFF;
    TL0 = 0xF0;

    TMOD = 0x01;
    // Selected timer operation (input from internal system clock)
    // 16-Bit Timer/Counter. TH0 and TL0 are cascaded; there is no prescaler.

    TCON = 0x10; // Включаем таймер 0

    // Установка вектора в пользовательской таблице
    SetVector( 0x200B, (void *)T0_ISR );

    // Разрешение прерываний от таймера 0
    ET0 = 1; EA = 1;
}




void putchar( char c ) { wsio( c ); }
char getchar( void ) { return rsio(); }

void inc_voltage( unsigned char n_dac )
{
    if( n_dac > 1 ) return;

    if( v[ n_dac ] < 4.95 )
        v[ n_dac ] += 0.1;

    set_voltage( v[ n_dac ], n_dac );

    printf_fast_f( "Set DAC%d, U = %.1fV \r\n", n_dac, v[ n_dac ] );
}


void dec_voltage( unsigned char n_dac )
{
    if( n_dac > 1 ) return;

    if( v[ n_dac ] >= 0.1 )
        v[ n_dac ] -= 0.1;

    set_voltage( v[ n_dac ], n_dac );

    printf_fast_f( "Set DAC%d, U = %.1fV \r\n", n_dac, v[ n_dac ] );
}

void print_voltage( void )
{
    float v0, v1;

    v0 = get_voltage( 0 );
    v1 = get_voltage( 1 );

    printf_fast_f( "U0 = %.3fV, U1 = %.3fV \n", v0, v1 );
}

void int_to_string(int number, char *string, int length)
{
    int i, j;
    int d_count;
    char temp;

    for (i = 0; i < length; ++i) {
        string[i] = '\0';
    }


    j = 0; d_count = 0;
    while (number > 0) {
        string[j++] = (number % 10) + '0';
        number /= 10;

        d_count ++;
    }

    // reverse:
    for (i = 0; i < d_count / 2; ++i) {
        temp = string[i];
        string[i] = string[d_count - 1 - i];
        string[d_count - 1 - i] = temp;
    }
}


void main( void )
{
    unsigned short i, p;
    unsigned char ch, leds;
    unsigned char c;
    char digit;
    char string[10];
    int string_id;

    float measured_temperature; // Измеренная температура
    int int_measured_temperature;

    init_sio( S9600 );
    init_dac( _8BIT );
    init_adc();


    switch_dac( DAC0, ON );
    switch_dac( DAC1, ON );


    printf_fast_f( "Test DAC & ADC\r\n" );


    type("1 - +voltage, DAC0\r\n");
    type("2 - -voltage, DAC0\r\n");
    type("3 - +voltage, DAC1\r\n");
    type("4 - -voltage, DAC1\r\n");

    type("r - get voltage, ADC1 & ADC2 \r\n");




    while( 1 )
    {
        if( rsiostat() )
        {
            c = rsio();

            switch( c )
            {
                case '1': inc_voltage( DAC0 ); break;
                case '2': dec_voltage( DAC0 ); break;
                case '3': inc_voltage( DAC1 ); break;
                case '4': dec_voltage( DAC1 ); break;

                case 'r': print_voltage(); break;

                default: type("\r\n"); break;
            }
        }

        break; // :D
    }

    // new code:
    Buzz();

    //----------Инициализация-----------
    InitLCD();
    init_sio(S9600);

    init_timer(); // Включаем таймер 0, имитирующий работу холодильника

    type("Hello!\r\n");

    //----------Пьезоизлучатель---------
    Buzz();

    //----------ЖКИ---------------------
    LCD_Clear();
    LCD_Type(" Hello! ");
    LCD_GotoXY(0,1);


    // Устанавливаем текущую температуру
    current_temperature = 24.0;

    // Измеренное значение температуры
    measured_temperature = 0.0;



    // Пользователь вводит значение с клавиатуры


    //----------Клавиатура, светодиоды--
    type("\r\nReading keyboard\r\n");
    LCD_Clear();
    leds = 0;

    // Пользователь устанавливает значение температуры
    target_temperature = 0;

    string_id = 0;
    for (i = 0; i < 10; ++i)
    {
        string[i] = '\0';
    }

    while(1) //Вывод в бесконечном цикле нажатых на клавиатуре SDK-1.1
    {
        // клавиш на ЖКИ и терминал ПК
        WriteMax(LEDLINE, leds); //Единичка в соответствующем бите зажигает
        //светодиод, нолик - гасит

        leds <<= 1;
        if( !(leds & 0x80) )
            leds |= 1;


        if( ScanKBOnce(&ch) )
        {
            // Была нажата какая-то клавиша
            wsio(ch);

            if ('0' <= ch && ch <= '9')
            {
                // Была нажата цифра

                // Интерпретируем символ как цифру:
                digit = ch - '0';
                target_temperature *= 10;
                target_temperature += (int) digit;

                // Записываем строку для вывода на экран.
                string[string_id] = ch;
                string_id ++;

            }
            else if (ch == 'A')
            {
                // Ввод завершен
                type("\r\nReading keyboard completed\r\n");

                break;
            }
            else {
                // Пользователь ввел что-то не то
                Buzz();
            }


            // Вывести на экран текущую строку
            LCD_Clear();
            LCD_Type(string);

            // Вывести на экран текущий символ
            // LCD_Putch(ch);
        }
        else
            for(i = 0; i < 2000; i++); //Задержка, регулирующая скорость
        //изменения состояния светодиодов
    }

    //

    while (1)
    {
        // Измеряем температуру (АЦП)
        // и выводим ее на экран

        measured_temperature = get_voltage( 0 );

        // Берем целую часть температуры
        int_measured_temperature = (int) measured_temperature;

        int_to_string(int_measured_temperature, string, 10);

        type(string); // Вывести строку в терминал
        LCD_Type(string); // Вывести строку на ЖКИ дисплей
    }




}
