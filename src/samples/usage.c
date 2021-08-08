#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include "interface.h"

int main()
{
    puts( "KEY: XOR( ^, @ ), OR( |, \\/ ), AND( &, /\\ ), NOT( ~, ! ), IF( =>, -> ), IFF( <=> )\n" );
    printf( "Enter a logical expression: " );

    char *buffer = NULL;
    size_t len = 0;
    if( (len = getline( &buffer, &len, stdin )) == -1 )
        return -1;
    buffer[ len-1] = L'\0';

    wchar_t *w_str = toWString( buffer );
    PostfixGenerator *gen = makePostfixGenerator( w_str );
    wchar_t **pfx_array = postfixForm( gen, &len );
    LogicModel *logicModel = makeLogicModel( pfx_array, len );
    TableModel *dispModel = makeTableModel( logicModel );

    FILE *fp = fopen( "sample.txt", "w");

    addOutputChannel( dispModel, tty0() );
    addOutputChannel( dispModel, fp);
    setDisplayModeAscending( dispModel, 1 );
    display( dispModel );

    destroyPostfixObject( gen );
    destroyLogicModelObject( logicModel );
    destroyTableModelObject( dispModel );
    freeArray( pfx_array, len );
    freeString( w_str );
    free( buffer );

    return 0;
}