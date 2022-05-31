const emitStream = require( 'emit-stream');
const through    = require( 'through');

module.exports = function( cpp_entry_point, opts)
{
    const factory = require( cpp_entry_point);
    const { EventEmitter } = require( 'events');
    const emitter = new EventEmitter();

    const worker = new factory.StreamingWorker(
        function ( event, value) {
            emitter.emit( event, value);
        },
        function () {
            emitter.emit( "close");
        },
        function ( error) {
            emitter.emit( "error", error);
        },
        opts
    );

    const streamer = {
        from: emitter,
        to: {
            emit: function ( name, data) {
                worker.sendToAddon( name, data);
            }
        },
        stream: function () {
            return emitStream( emitter).pipe(
                through( function ( data) {
                    if( data[ 0] === "close")
                        this.end();
                    else
                        this.queue( data);
                })
            );
        },
        close: function () {
            worker.close();
        }
    };

    streamer.to.stream = function ( name, end) {
        return through( function ( data) {
            if ( Array.isArray(data)) {
                if ( data[0] === "close")
                    this.end();
                else
                    streamer.to.emit( data[ 0], data[ 1]);
            } else
                streamer.to.emit( name, data);
        }, end);
    };

    return streamer;
};