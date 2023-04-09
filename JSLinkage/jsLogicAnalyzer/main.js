"use strict";

const EventEmitter = require( 'events');
const path         = require( 'path');
const readline     = require( 'readline');

const make_analyzer = function ( opts)
{
    const worker = require('bindings')('logic_analyzer');

    const emitter = new EventEmitter();
    worker.StreamingWorker( function ( key, value) {
        emitter.emit( key, value);
    }, function () {
        emitter.emit( "close");
    }, function ( error) {
        emitter.emit( "error", error);
    }, opts);

    return emitter;
};

const tabulate = function( text)
{
    let headers = null;
    let rows    = [];
    let table   = [];

    const addon = make_analyzer({ input: text});
    addon.on( 'packet', ( packet) => {
        const entry = JSON.parse( packet);
        if( entry.headers != null)
        {
            headers = entry.headers;
        }
        else
        {
            rows.push( entry.rows);
            const row = {};
            entry.rows.forEach(( each, index) => row[ headers[ index]] = each); 
            table.push( row);
        }
    });

    addon.on( 'error', ( message) => console.log( message));

    addon.on( 'close', () => {
        console.table( table, headers);
        headers = null;
        rows = [];
        table = [];
        controller
            .question( 
                "Solve another(yes/no/Y/N/)? ", reply => {
                if([ "yes", "y"].includes( reply.toLowerCase()))
                    main();
                else
                    process.exit( 0);
            });
    });
};

const controller = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

const main = function()
{
    controller.question( 
        "KEY: XOR( ^, @ ), OR( |, \\/ ), AND( &, /\\ )," +
        " NOT( ~, ! ), IF( =>, -> ), IFF( <=> )" +
        "\nEnter a logical expression: ", query => {
        if( query.length !== 0)
            tabulate( query);
    });
}

main();
