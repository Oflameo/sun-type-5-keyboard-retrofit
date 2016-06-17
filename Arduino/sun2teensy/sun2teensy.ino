static byte keyStack[6];
static byte LEDstate = 0;
static byte mod = 0;
/*
  The status byte has a bit for each of the LEDs on the keyboard
  If the bit is set (1), then the LED is on
  Bit 0 corresponds to the Num Lock indicator
  bit 1 is Compose
  bit 2 is Scroll Lock
  bit 3 is Caps Lock
*/
//TODO handle stack overflow
byte addToKeyStack( char key ) {
  for ( int i = 0; i < 6; i++ ) {
    if ( keyStack[i] == 0 ) {
      keyStack[i] = key;
      return 1;
    }
  }
  return 0;
}
//TODO handle mismatched character
byte dropFromKeyStack( char key ) {
  for ( int i = 0; i < 6; i++ ) {
    if ( keyStack[i] == key ) {
      keyStack[i] = 0;
      return 1;
    }
  }
  return 0;
}

void beep( int len ) {
  Serial1.write( 0x02 );
  delay( len );
  Serial1.write( 0x03 );
}

void keyClick( byte enabled ) {
  if ( enabled ) Serial1.write( 0x0A );
  else Serial1.write( 0x0B );
}

void ledEnable() {
  Serial1.write( 0x0E );
  Serial1.write( LEDstate );
}

//TODO add keyboard beeper code to respond to exceptions

//TODO handle Idle state and use to clear buffer
/*
  0x07F - Idle
  Code sent after a key up if no other keys are pressed. Also sent after
  power-up and reset if no keys are pressed.
*/
void setup() {
  Serial.begin( 9600 );
  Serial1.begin( 1200 );
  Serial1.write( 0x01 ); //Reset
  //TODO Add Reset response checking code
  /*
    After receiving this command, the keyboard executes the self test
    routine. The keyboard responds with: 0FFH, 004H, 007FH if the self
    test passes and no keys are down. The code 07FH is replaced by
    the make code if a key is down.
    The keyboard sends 07EH, 001H if the self test fails.
  */
  Serial1.write( 0x03 ); //Bell Off
  Serial1.write( 0x0B ); //Click Off
  Serial1.write( 0x0E ); //LED Command
  Serial1.write( LEDstate );
  //tada
  beep( 100 );
  delay( 50 );
  beep( 100 );
}

void handleKeypress( byte in ) {
  switch ( in ) {
    case 0x4C: mod |= MODIFIERKEY_CTRL;   break;
    case 0xCC: mod &= ~MODIFIERKEY_CTRL;  break;

    case 0x4D: addToKeyStack( KEY_A );    break;
    case 0xCD: dropFromKeyStack( KEY_A ); break;

    case 0x68: addToKeyStack( KEY_B );    break;
    case 0xE8: dropFromKeyStack( KEY_B ); break;
    
    case 0x66: addToKeyStack( KEY_C );    break;
    case 0xE6: dropFromKeyStack( KEY_C ); break;

    case 0x4F: addToKeyStack( KEY_D );    break;
    case 0xCF: dropFromKeyStack( KEY_D ); break;
  }
}

void outputKeys() {
  Keyboard.set_modifier( mod );
  Keyboard.set_key1( keyStack[0] );
  Keyboard.set_key2( keyStack[1] );
  Keyboard.set_key3( keyStack[2] );
  Keyboard.set_key4( keyStack[3] );
  Keyboard.set_key5( keyStack[4] );
  Keyboard.set_key6( keyStack[5] );
  Keyboard.send_now();
}

void loop() {
  while ( Serial1.available() > 0 ) {
    byte in = Serial1.read();
    Serial.println( in );
    handleKeypress( in );
    outputKeys();
  }
}
