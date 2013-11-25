#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>

int main(void)
{
    ButtonBoard buttons( FEHIO::Bank3 );

    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::White );

    while( true )
    {
        if( buttons.MiddlePressed() )
        {
            LCD.WriteLine( "foo!" );
            Sleep( 100 );
        }
    }
    return 0;
}
