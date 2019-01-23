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

static float v[ 2 ] = { 0.0, 0.0 };


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

void main( void )
{
    unsigned char c;

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
    }
}