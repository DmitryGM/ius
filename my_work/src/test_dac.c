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
#include <datatypes.h>

#include <aduc812.h>
#include <sio.h>
#include <lcd.h>
#include <kb.h>
#include <vect.h>
#include <system_timer.h>

// #include <stdio.h>
// #include <max.h>
// #include <buzz.h>

// dac && adc DOESN'T WORK
// #include <dac.h>
// #include <adc.h>

static float v[ 2 ] = { 0.0, 0.0 };
static float current_temperature;
static int target_temperature;
static bit fridge_state;


// //////////////////////// T0_ISR //////////////////////////////
// // Обработчик прерывания от таймера 0.
// // Вход: нет.
// // Выход: нет.
// // Результат: нет.
// //////////////////////////////////////////////////////////////
void T0_ISR( void ) __interrupt ( 1 )
{
    // Тестим таймер:
    type("!");
    // char ch;

    // // Обновляем состояние холодильника
    // if( ScanKBOnce(&ch) )
    // {
    //     if (ch == '0')
    //     {
    //         fridge_state = 0;
    //     }
    //     else if (ch == '1')
    //     {
    //         fridge_state = 1;
    //     }
    //     else {
    //         // Пользователь ввел что-то не то
    //         Buzz();
    //     }
    // }

    // // Если холодильник ВЫКЛ, выходим.
    // if (!fridge_state)
    //     return;

    // if (current_temperature < (float)target_temperature)
    // {
    //     current_temperature += 0.0001;
    // }
    // else
    // {
    //     current_temperature -= 0.0001;
    // }

    // // Для имитации будем использовать 0-й канал ЦАП
    // set_voltage(current_temperature, DAC0);
}

// init timer 0
void init_timer()
{
    int i;
    u8 u8_TMOD;

    type("\n");
    type("TMOD:\n");
    // TMOD |= 0b00010000; // doesn't work ???
    // // TMOD |= 0x10;
    // u8_TMOD = TMOD;
    // // TMOD = u8_TMOD;
    // u8_TMOD |= 0x10;
    // TMOD = u8_TMOD;

    // for (i = 0; i < 8; ++i) {
    //     if (u8_TMOD & 1)
    //         type("1");
    //     else
    //         type("0");

    //     u8_TMOD >>= 1;
    // }
    // type("\n");

    // if (TMOD) {
    //     type("yes\n");
    // }
    // else {
    //     type("no\n");
    // }

    type("111");
    SetVector(0x201B, (void *)T0_ISR);
    type("222");
    // // Инициализация таймера 0

    // // Задаем частоту таймера
    // TH0 = 0xFF;
    // TL0 = 0xF0;

    // TMOD = 0x01;
    // // Selected timer operation (input from internal system clock)
    // // 16-Bit Timer/Counter. TH0 and TL0 are cascaded; there is no prescaler.

    // TCON = 0x10; // Включаем таймер 0

    // // Установка вектора в пользовательской таблице
    // SetVector( 0x200B, (void *)T0_ISR );

    // // Разрешение прерываний от таймера 0
    // ET0 = 1; EA = 1;
}

void main( void )
{
    unsigned short i, p;
    unsigned char ch, leds;
    float a;
    // unsigned char c;
    // char digit;
    // char string[10];
    // int string_id;

    // float measured_temperature; // Измеренная температура
    // int int_measured_temperature;

    


    //----------Инициализация-----------
    init_sio( S9600 );
    type("Hello world!\n");
    

    // allow all interrupts
    EA = 1;
    initialize_system_timer(); // T2

    while (1);
    // type("[START] init timer\n");
    // init_timer(); // Включаем таймер ?, имитирующий работу холодильника
    // type("[STOP] init timer\n");

    // init timer 1:
    // ET1 = 1; // allow interrupts from timer 1
    // TR1 = 1; // start timer 1


    // InitLCD(); // Забавно, НО этот код оказался не нужен

    //----------Пьезоизлучатель---------
    // Buzz();

    //----------ЖКИ---------------------
    LCD_Clear();
    LCD_Type(" Hello world! ");
    // LCD_GotoXY(0,1);

    // Тестим клавиатуру:
    while (1) {
        if( ScanKBOnce(&ch) ) {
            // Выводим нажатую клавишу
            type("\r\n!!!\r\n");
            wsio(ch);
        }
        if (ch == 'C')
            break;
    }



    // Устанавливаем текущую температуру
    // current_temperature = 24.0;

    // Измеренное значение температуры
    // measured_temperature = 0.0;

    // Состояние холодильника == ВКЛ
    // fridge_state = 1;

    // Пользователь вводит значение с клавиатуры


    //----------Клавиатура, светодиоды--
    // type("\r\nReading keyboard\r\n");
    // LCD_Clear();
    // leds = 0;

    // Пользователь устанавливает значение температуры
    // target_temperature = 0;

    // string_id = 0;
    // for (i = 0; i < 10; ++i)
    // {
        // string[i] = '\0';
    // }

    // while(1) //Вывод в бесконечном цикле нажатых на клавиатуре SDK-1.1
    // {
    //     // клавиш на ЖКИ и терминал ПК
    //     WriteMax(LEDLINE, leds); //Единичка в соответствующем бите зажигает
    //     //светодиод, нолик - гасит

    //     leds <<= 1;
    //     if( !(leds & 0x80) )
    //         leds |= 1;


    //     if( ScanKBOnce(&ch) )
    //     {
    //         // Была нажата какая-то клавиша
    //         wsio(ch);

    //         if ('0' <= ch && ch <= '9')
    //         {
    //             // Была нажата цифра

    //             // Интерпретируем символ как цифру:
    //             digit = ch - '0';
    //             target_temperature *= 10;
    //             target_temperature += (int) digit;

    //             // Записываем строку для вывода на экран.
    //             string[string_id] = ch;
    //             string_id ++;

    //         }
    //         else if (ch == 'A')
    //         {
    //             // Ввод завершен
    //             type("\r\nReading keyboard completed\r\n");

    //             break;
    //         }
    //         else {
    //             // Пользователь ввел что-то не то
    //             Buzz();
    //         }


    //         // Вывести на экран текущую строку
    //         LCD_Clear();
    //         LCD_Type(string);

    //         // Вывести на экран текущий символ
    //         // LCD_Putch(ch);
    //     }
    //     else
    //         for(i = 0; i < 2000; i++); //Задержка, регулирующая скорость
    //     //изменения состояния светодиодов
    // }

    // //

    // while (1)
    // {
    //     // Измеряем температуру (АЦП)
    //     // и выводим ее на экран

    //     measured_temperature = get_voltage( 0 );

    //     // Берем целую часть температуры
    //     int_measured_temperature = (int) measured_temperature;

    //     int_to_string(int_measured_temperature, string, 10);

    //     type(string); // Вывести строку в терминал
    //     LCD_Type(string); // Вывести строку на ЖКИ дисплей
    // }


    while (1);


}
