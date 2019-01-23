/******************************************************************************
(С) ООО "ЛМТ", Санкт-Петербург, Россия, 2002.
http://lmt.cs.ifmo.ru, tel.: +7-812-233-3096,
mailto: lmt@d1.ifmo.ru

Данный файл является свободно распространяемым примером работы
с устройствами учебного стенда SDK-1.1 на языке Си (компилятор
Keil C фирмы KEIL ELEKTRONIK GmbH) и предназначен для иллюстрации
использования функций других модулей в составе данного пакета, а
также для иллюстрации работы с клавиатурой и акустическим
пьезоизлучателем стенда.
Данный программный модуль не подвергался тщательному
тестированию и может содержать ошибки. Авторы не несут 
ответственности за потерю информации и иные возможные вредные
последствия использования данного программного модуля в составе
программных проектов.
Данный файл может подвергаться любым изменениям, но не может
распространяться в коммерческих целях и в составе коммерческих
программных продуктов.


Файл: main.c
Версия: 1.0.0
Автор: LAN
Описание: Набор демонстрационного программного обеспечения для стенда
SDK-1.1. Примеры использования аппаратуры стенда. Работа с
клавиатурой и акустическим пьезоизлучателем. Данный модуль 
использует другие модули в составе распространяемого пакета.

Изменения:
-------------------------------------------------------------------------------
N Дата Версия Автор Описание 
-------------------------------------------------------------------------------
1 10.04.02 1.0.0 LAN Создан
******************************************************************************/

#include "ADuC812.h"
#include "sio.h"
#include "max.h"
#include "lcd.h"
#include "eeprom.h"
#include "rtc.h"
#include "parallel.h"
#include "buzz.h"
#include "kb.h"

/*----------------------------------------------------------------------------
Переменные и флаги
-----------------------------------------------------------------------------*/
unsigned char xdata buffer[50]; //Буфер для тестирования EEPROM

TIME xdata time = {0, 1, 2, 1}; //Структура для установки и получения текущего
//времени. Описание см. в файле rtc.h

/*----------------------------------------------------------------------------
Функции
-----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------
main()
-------------------------------------------------------------------------------
Демонстрация использования некоторых функций, описанных в остальных файлах
пакета. Используются не все функции. См. комментарии по ходу текста.

Вход: нет
Выход: нет 
Результат: нет
----------------------------------------------------------------------------- */
void main(void)
{
unsigned short i, p;
unsigned char ch, leds;

//----------Инициализация-----------
InitLCD(); 
InitSIO(S9600, 0);
Type("Hello!\r\n");

//----------Пьезоизлучатель---------
Buzz();


//----------EEPROM, ЖКИ-------------
for(i = 0; i < 50; i++) //Подготовка буфера для записи в
buffer[i] = (unsigned char)(~i);//EEPROM

LCD_Clear();
LCD_Type(" EEPROM ");
LCD_GotoXY(0,1);

if( WriteBlockEEPROM(0, buffer, 29) ) //Попытка записи
LCD_Type("Write error!"); 
else
LCD_Type("Write OK");

for(i = 0; i < 65000; i++); //Задержка для интерпретации изображения на
//ЖКИ

LCD_GotoXY(0,1);
if( ReadBlockEEPROM(0, buffer, 29) ) //Попытка чтения
LCD_Type("Read error!");
else
{
for(i = 0; i < 29; i++) //Чтение успешно - проверка
if( buffer[i] != (unsigned char)(~i) )
{
LCD_Type("Check error!"); 
break;
}

if(i == 29) LCD_Type("Check OK"); 
}

for(i = 0; i < 60000; i++) //Задержка, при необходимости прерываемая
if(RSioStat()) //нажатием клавиши в терминале ПК
{
RSio(); break;
}


//----------Часы реального времени--

if(SetTime(&time)) //Попытка установки определенного времени
Type("Clock setting failed!\r\n");

LCD_Clear();
LCD_Type(" Clock ");

while(1) //Вывод в цикле текущего времени на ЖКИ и терминал ПК
{
if(RSioStat()) //Цикл прерывается нажатием клавиши на терминале ПК
{
RSio(); break;
}

Type("Current time: ");
LCD_GotoXY(0,1);

if( GetTime(&time) )
{
LCD_Type("Error!");
Type("failed to retrieve!\r\n");
break;
}
else
{
i = 0;
//Hours
buffer[i++] = (time.hour / 10) + '0';
buffer[i++] = (time.hour % 10) + '0';
buffer[i++] = ':';

//Minutes
buffer[i++] = (time.min / 10) + '0';
buffer[i++] = (time.min % 10) + '0';
buffer[i++] = ':';

//Seconds
buffer[i++] = (time.sec / 10) + '0';
buffer[i++] = (time.sec % 10) + '0';

buffer[i] = '\0';

Type(buffer); WSio(13);
LCD_Type(buffer);
}
} 

//----------Клавиатура, светодиоды--
Type("\r\nReading keyboard\r\n");
LCD_Clear();
leds = 0;
while(1) //Вывод в бесконечном цикле нажатых на клавиатуре SDK-1.1
{ //клавиш на ЖКИ и терминал ПК
WriteMax(LEDLINE, leds); //Единичка в соответствующем бите зажигает
//светодиод, нолик - гасит
leds <<= 1;
if( !(leds & 0x80) )
leds |= 1;

if( ScanKBOnce(&ch) )
{
WSio(ch);
LCD_Putch(ch);
}
else
for(i = 0; i < 2000; i++); //Задержка, регулирующая скорость 
//изменения состояния светодиодов
}


}
