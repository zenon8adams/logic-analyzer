"use strict";

const EventEmitter = require( 'events');
const path         = require( "path");

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

const addon = make_analyzer({ input: "A & B | C & D"});//" <=> (((~C & ~F) & ~A | ~G) => ((A | ~B) | A & ~H))"});

let headers = null;
let rows    = [];
let table   = [];

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
});
