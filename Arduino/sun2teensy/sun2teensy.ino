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

    case 0x6E: mod |= MODIFIERKEY_SHIFT;    break;
    case 0xEE: mod &= MODIFIERKEY_SHIFT; break;

    case 0x13: mod |= MODIFIERKEY_ALT;    break;
    case 0x93: mod &= MODIFIERKEY_ALT; break;

    case 0x43: mod |= MODIFIERKEY_GUI;    break;
    case 0xC3: mod &= MODIFIERKEY_GUI; break;

    case 0x4D: addToKeyStack( KEY_A );    break;
    case 0xCD: dropFromKeyStack( KEY_A ); break;

    case 0x68: addToKeyStack( KEY_B );    break;
    case 0xE8: dropFromKeyStack( KEY_B ); break;
    
    case 0x66: addToKeyStack( KEY_C );    break;
    case 0xE6: dropFromKeyStack( KEY_C ); break;

    case 0x4F: addToKeyStack( KEY_D );    break;
    case 0xCF: dropFromKeyStack( KEY_D ); break;

    case 0x38: addToKeyStack( KEY_E );    break;
    case 0xB8: dropFromKeyStack( KEY_E ); break;

    case 0x50: addToKeyStack( KEY_F );    break;
    case 0xD0: dropFromKeyStack( KEY_F ); break;

    case 0x51: addToKeyStack( KEY_G );    break;
    case 0xD1: dropFromKeyStack( KEY_G ); break;

    case 0x52: addToKeyStack( KEY_H );    break;
    case 0xD2: dropFromKeyStack( KEY_H ); break;

    case 0x3D: addToKeyStack( KEY_I );    break;
    case 0xBD: dropFromKeyStack( KEY_I ); break;

    case 0x53: addToKeyStack( KEY_J );    break;
    case 0xD3: dropFromKeyStack( KEY_J ); break;

    case 0x54: addToKeyStack( KEY_K );    break;
    case 0xD4: dropFromKeyStack( KEY_K ); break;

    case 0x55: addToKeyStack( KEY_L );    break;
    case 0xD5: dropFromKeyStack( KEY_L ); break;

    case 0x6A: addToKeyStack( KEY_M );    break;
    case 0xEA: dropFromKeyStack( KEY_M ); break;

    case 0x69: addToKeyStack( KEY_N );    break;
    case 0xE9: dropFromKeyStack( KEY_N ); break;

    case 0x3E: addToKeyStack( KEY_O );    break;
    case 0xBE: dropFromKeyStack( KEY_O ); break;

    case 0x3F: addToKeyStack( KEY_P );    break;
    case 0xEF: dropFromKeyStack( KEY_P ); break;

    case 0x36: addToKeyStack( KEY_Q );    break;
    case 0xB6: dropFromKeyStack( KEY_Q ); break;

    case 0x39: addToKeyStack( KEY_R );    break;
    case 0xB9: dropFromKeyStack( KEY_R ); break;

    case 0x4E: addToKeyStack( KEY_S );    break;
    case 0xCE: dropFromKeyStack( KEY_S ); break;

    case 0x3A: addToKeyStack( KEY_T );    break;
    case 0xBA: dropFromKeyStack( KEY_T ); break;

    case 0x3C: addToKeyStack( KEY_U );    break;
    case 0xBC: dropFromKeyStack( KEY_U ); break;

    case 0x67: addToKeyStack( KEY_V );    break;
    case 0xE7: dropFromKeyStack( KEY_V ); break;

    case 0x37: addToKeyStack( KEY_W );    break;
    case 0xB7: dropFromKeyStack( KEY_W ); break;

    case 0x65: addToKeyStack( KEY_X );    break;
    case 0xE5: dropFromKeyStack( KEY_X ); break;

    case 0x3B: addToKeyStack( KEY_Y );    break;
    case 0xBB: dropFromKeyStack( KEY_Y ); break;

    case 0x64: addToKeyStack( KEY_Z );    break;
    case 0xE4: dropFromKeyStack( KEY_Z ); break;

    case 0x1E: addToKeyStack( KEY_1 );    break;
    case 0x9E: dropFromKeyStack( KEY_1 ); break;

    case 0x1F: addToKeyStack( KEY_2 );    break;
    case 0x9F: dropFromKeyStack( KEY_2 ); break;

    case 0x20: addToKeyStack( KEY_3 );    break;
    case 0xA0: dropFromKeyStack( KEY_3 ); break;

    case 0x21: addToKeyStack( KEY_4 );    break;
    case 0xA1: dropFromKeyStack( KEY_4 ); break;

    case 0x22: addToKeyStack( KEY_5 );    break;
    case 0xA2: dropFromKeyStack( KEY_5 ); break;

    case 0x23: addToKeyStack( KEY_6 );    break;
    case 0xA3: dropFromKeyStack( KEY_6 ); break;

    case 0x24: addToKeyStack( KEY_7 );    break;
    case 0xA4: dropFromKeyStack( KEY_7 ); break;

    case 0x25: addToKeyStack( KEY_8 );    break;
    case 0xA5: dropFromKeyStack( KEY_8 ); break;

    case 0x26: addToKeyStack( KEY_9 );    break;
    case 0xA6: dropFromKeyStack( KEY_9 ); break;

    case 0x27: addToKeyStack( KEY_0 );    break;
    case 0xA7: dropFromKeyStack( KEY_0 ); break;

    case 0x5A: addToKeyStack( KEY_ENTER );    break;
    case 0xDA: dropFromKeyStack( KEY_ENTER ); break;

    case 0x1D: addToKeyStack( KEY_ESC );    break;
    case 0x9D: dropFromKeyStack( KEY_ESC ); break;

    case 0x2B: addToKeyStack( KEY_BACKSPACE );    break;
    case 0xAB: dropFromKeyStack( KEY_BACKSPACE ); break;

    case 0x35: addToKeyStack( KEY_TAB );    break;
    case 0xB5: dropFromKeyStack( KEY_TAB ); break;

    case 0x79: addToKeyStack( KEY_SPACE );    break;
    case 0xF9: dropFromKeyStack( KEY_SPACE ); break;

    case 0x28: addToKeyStack( KEY_MINUS );    break;
    case 0xA8: dropFromKeyStack( KEY_MINUS ); break;

    case 0x29: addToKeyStack( KEY_EQUAL );    break;
    case 0xA9: dropFromKeyStack( KEY_EQUAL ); break;

    case 0x40: addToKeyStack( KEY_LEFT_BRACE );    break;
    case 0xC0: dropFromKeyStack( KEY_LEFT_BRACE ); break;

    case 0x41: addToKeyStack( KEY_RIGHT_BRACE );    break;
    case 0xC1: dropFromKeyStack( KEY_RIGHT_BRACE ); break;

    case 0x58: addToKeyStack( KEY_BACKSLASH );    break;
    case 0xD8: dropFromKeyStack( KEY_BACKSLASH ); break;

    //case 0x4F: addToKeyStack( KEY_NUMBER );    break;
    //case 0xCF: dropFromKeyStack( KEY_NUMBER ); break;

    case 0x56: addToKeyStack( KEY_SEMICOLON );    break;
    case 0xD6: dropFromKeyStack( KEY_SEMICOLON ); break;

    case 0x57: addToKeyStack( KEY_QUOTE );    break;
    case 0xD7: dropFromKeyStack( KEY_QUOTE ); break;

    case 0x2A: addToKeyStack( KEY_TILDE );    break;
    case 0xAA: dropFromKeyStack( KEY_TILDE ); break;

    case 0x6B: addToKeyStack( KEY_COMMA );    break;
    case 0xEB: dropFromKeyStack( KEY_COMMA ); break;

    case 0x6C: addToKeyStack( KEY_PERIOD );    break;
    case 0xEC: dropFromKeyStack( KEY_PERIOD ); break;

    case 0x6D: addToKeyStack( KEY_SLASH );    break;
    case 0xED: dropFromKeyStack( KEY_SLASH ); break;

    case 0x77: addToKeyStack( KEY_CAPS_LOCK );    break;
    case 0xF7: dropFromKeyStack( KEY_CAPS_LOCK ); break;

    case 0x05: addToKeyStack( KEY_F1 );    break;
    case 0x85: dropFromKeyStack( KEY_F1 ); break;

    case 0x06: addToKeyStack( KEY_F2 );    break;
    case 0x86: dropFromKeyStack( KEY_F2 ); break;

    case 0x08: addToKeyStack( KEY_F3 );    break;
    case 0x88: dropFromKeyStack( KEY_F3 ); break;

    case 0x0A: addToKeyStack( KEY_F4 );    break;
    case 0x8A: dropFromKeyStack( KEY_F4 ); break;

    case 0x0C: addToKeyStack( KEY_F5 );    break;
    case 0x8C: dropFromKeyStack( KEY_F5 ); break;

    case 0x0E: addToKeyStack( KEY_F6 );    break;
    case 0x8E: dropFromKeyStack( KEY_F6 ); break;

    case 0x10: addToKeyStack( KEY_F7 );    break;
    case 0x90: dropFromKeyStack( KEY_F7 ); break;

    case 0x11: addToKeyStack( KEY_F8 );    break;
    case 0x91: dropFromKeyStack( KEY_F8 ); break;

    case 0x12: addToKeyStack( KEY_F9 );    break;
    case 0x92: dropFromKeyStack( KEY_F9 ); break;

    case 0x07: addToKeyStack( KEY_F10 );    break;
    case 0x87: dropFromKeyStack( KEY_F10 ); break;

    case 0x09: addToKeyStack( KEY_F11 );    break;
    case 0x89: dropFromKeyStack( KEY_F11 ); break;

    case 0x0B: addToKeyStack( KEY_F12 );    break;
    case 0x8B: dropFromKeyStack( KEY_F12 ); break;

    case 0x16: addToKeyStack( KEY_PRINTSCREEN );    break;
    case 0x96: dropFromKeyStack( KEY_PRINTSCREEN ); break;

    case 0x17: addToKeyStack( KEY_SCROLL_LOCK );    break;
    case 0x97: dropFromKeyStack( KEY_SCROLL_LOCK ); break;

    case 0x15: addToKeyStack( KEY_PAUSE );    break;
    case 0x95: dropFromKeyStack( KEY_PAUSE ); break;

    //case 0x5E: addToKeyStack( KEY_INSERT );    break;
    //case 0xDE: dropFromKeyStack( KEY_INSERT ); break;

    //case 0x44: addToKeyStack( KEY_HOME );    break;
    //case 0xC4: dropFromKeyStack( KEY_HOME ); break;

    //case 0x46: addToKeyStack( KEY_PAGE_UP );    break;
    //case 0xC6: dropFromKeyStack( KEY_PAGE_UP ); break;

    //case 0x32: addToKeyStack( KEY_DELETE );    break;
    //case 0xB2: dropFromKeyStack( KEY_DELETE ); break;

    //case 0x70: addToKeyStack( KEY_END );    break;
    //case 0xF0: dropFromKeyStack( KEY_END ); break;

    //case 0x72: addToKeyStack( KEY_PAGE_DOWN );    break;
    //case 0xF2: dropFromKeyStack( KEY_PAGE_DOWN ); break;

    //case 0x45: addToKeyStack( KEY_UP );    break;
    //case 0xC5: dropFromKeyStack( KEY_UP ); break;

    case 0x62: addToKeyStack( KEY_NUM_LOCK );    break;
    case 0xE2: dropFromKeyStack( KEY_NUM_LOCK ); break;

    case 0x2E: addToKeyStack( KEYPAD_SLASH );    break;
    case 0xAE: dropFromKeyStack( KEYPAD_ASTERIX ); break;

    case 0x47: addToKeyStack( KEYPAD_MINUS );    break;
    case 0xC7: dropFromKeyStack( KEYPAD_MINUS ); break;

    case 0x7D: addToKeyStack( KEYPAD_PLUS );    break;
    case 0xFD: dropFromKeyStack( KEYPAD_ENTER ); break;

    case 0x70: addToKeyStack( KEYPAD_1 );    break;
    case 0xF0: dropFromKeyStack( KEYPAD_1 ); break;

    case 0x71: addToKeyStack( KEYPAD_2 );    break;
    case 0xF1: dropFromKeyStack( KEYPAD_2 ); break;

    case 0x72: addToKeyStack( KEYPAD_3 );    break;
    case 0xF2: dropFromKeyStack( KEYPAD_3 ); break;

    case 0x5B: addToKeyStack( KEYPAD_4 );    break;
    case 0xDB: dropFromKeyStack( KEYPAD_4 ); break;

    case 0x5C: addToKeyStack( KEYPAD_5 );    break;
    case 0xDC: dropFromKeyStack( KEYPAD_5 ); break;

    case 0x5D: addToKeyStack( KEYPAD_6 );    break;
    case 0xDD: dropFromKeyStack( KEYPAD_6 ); break;

    case 0x44: addToKeyStack( KEYPAD_7 );    break;
    case 0xC4: dropFromKeyStack( KEYPAD_7 ); break;

    case 0x45: addToKeyStack( KEYPAD_8 );    break;
    case 0xC5: dropFromKeyStack( KEYPAD_8 ); break;

    case 0x46: addToKeyStack( KEYPAD_9 );    break;
    case 0xC6: dropFromKeyStack( KEYPAD_9 ); break;

    case 0x5E: addToKeyStack( KEYPAD_0 );    break;
    case 0xDE: dropFromKeyStack( KEYPAD_0 ); break;

    case 0x32: addToKeyStack( KEYPAD_PERIOD );    break;
    case 0xB2: dropFromKeyStack( KEYPAD_PERIOD ); break;
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
