/******************************************************************************
(С) ООО "ЛМТ", Санкт-Петербург, Россия, 2002.
http://lmt.cs.ifmo.ru, tel.: +7-812-233-3096,
mailto: lmt@d1.ifmo.ru

Данный файл является свободно распространяемым примером на
языке Си (компилятор Keil C фирмы KEIL ELEKTRONIK GmbH) и
предназначен для иллюстрации работы с клавиатурой стенда SDK-1.1.
Данный программный модуль не подвергался тщательному
тестированию и может содержать ошибки. Авторы не несут 
ответственности за потерю информации и иные возможные вредные
последствия использования данного программного модуля в составе
программных проектов.
Данный файл может подвергаться любым изменениям, но не может
распространяться в коммерческих целях и в составе коммерческих
программных продуктов.


Файл: kb.c
Версия: 1.0.0
Автор: LAN
Описание: Набор демонстрационного программного обеспечения для стенда
SDK-1.1. Работа с клавиатурой. Данный модуль использует 
модуль работы с функциональным расширителем центрального 
процессора стенда на базе ПЛИС Altera MAX3064 (3128) 
(файлы max.c, max.h).

Изменения:
-------------------------------------------------------------------------------
N Дата Версия Автор Описание 
-------------------------------------------------------------------------------
1 10.04.02 1.0.0 LAN Создан
******************************************************************************/
#include <max.h>

/*----------------------------------------------------------------------------
Переменные и флаги
-----------------------------------------------------------------------------*/

char KBTable[]="147*2580369#ABCD"; //Таблица символов, соответствующих клавишам
//клавиатуры SDK-1.1

/*----------------------------------------------------------------------------
Функции
-----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------
ScanKBOnce()
-------------------------------------------------------------------------------
Единичное сканирование клавиатуры SDK-1.1. Если нажата клавиша, то 
соответствующий ей символ (см. таблицу KBTable[]) помещается по указанному 
адресу.

Вход: char *ch - адрес, куда поместить символ нажатой клавиши. Если 
не было зарегистрировано нажатия, этот аргумент игнорируется.
Выход: *ch - помещенный символ первой (в порядке сканирования) из нажатых
клавиш.
Результат: 0 - ни одна клавиша не была нажата при сканировании;
1 - зарегистрировано нажатие.
----------------------------------------------------------------------------- */
bit ScanKBOnce(char *ch)
{
    unsigned char row,col,rownum,colnum;
    unsigned int i;

    //Сканирование производится по "столбцам" клавиатуры, на которые подается
    //"бегущий 0".
    for(colnum = 0; colnum < 4; colnum++)
    {
        col = 0x1 << colnum; //0001,0010,0100,1000,0001,...

        WriteMax(KB, ~col); //11111110,11111101,11111011,11110111,11111110,...

        //При подаче нуля на очередной столбец на каждом из "рядов" проверяется
        //наличие нуля (факт замыкания контакта клавишей)
        for(rownum = 0; rownum < 4; rownum++)
        {
            row = ReadMax(KB) & (0x10 << rownum);
            if( !row ) //Обнаружено нажатие клавиши:
            {
                for(i = 0; i<10000; i++) continue; //проверка на дребезг контакта:
                //через примерно 40мс повтор сканирования той же клавиши

                row = ReadMax(KB) & (0x10 << rownum);
                if( !row )
                {
                    *ch = (KBTable[(colnum<<2) + rownum]);
                    return 1; //Стабильное нажатие клавиши
                }

            }
        }

    }

    return 0; //Ни одна клавиша не нажата
}
