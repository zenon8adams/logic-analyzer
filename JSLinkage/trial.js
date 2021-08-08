
const trial = require( 'bindings')( 'trial');

readline = require( 'readline').createInterface({
  input: process.stdin,
  output: process.stdout
});

console.log( "KEY: XOR( ^, @ ), OR( |, \\/ ), AND( &, /\\ ), NOT( ~, ! ), IF( =>, -> ), IFF( <=> )");
readline.question(`Enter a logic expression: `, name => {
  trial.run( name);
  readline.close();
});
