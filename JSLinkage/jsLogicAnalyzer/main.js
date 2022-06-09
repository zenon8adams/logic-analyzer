"use strict";

const EventEmitter = require( 'events');
const path = require( "path");

const make_analyzer = function ( opts)
{
    const addon_path = path.join( __dirname, "build/Release/logic_analyzer");
    const worker = require( addon_path);

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

const addon = make_analyzer( { input: "(((A & B) | D | C) => ((A ^ B) & E | C)) <=> (((C & ~F) & ~A | G) => ((A | ~B) | A & H))"});

addon.on( 'packet', ( packet) => {
    const result = JSON.parse( packet);
    if( result.headers != null)
        console.log( result);
    else
        console.log( packet)
});
addon.on( 'error', ( message) => console.log( message));