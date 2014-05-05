msg_t msg_fromSerialParse(char* buffer, int maxLen)
{
    char lastChar = 0;
    char len = 0;

    while (lastChar != 13 && len < maxLen)
    {
        while (Serial.available() <= 0)
        {

        }

        lastChar = Serial.read();
        buffer[len] = lastChar;
        len++;
    }

    if ( lastChar != 13 )
    {
        Serial.println("\nno newline after cmd");
        while (Serial.available() <= 0)
        {
            Serial.read();
        }

        msg_t msg = {};
        msg.len = -1;
        return msg;
    }
    else
    {
        buffer[len-1] = 0;
    }

    msg_t msg = {};
    msg.buffer = buffer;
    msg.len = len;
    msg.index = 0;

    return msg;
}

int msg_read (msg_t* msg)
{
    int val = msg_peek(msg);
    if ( val != -1 )
    {
        msg->index++;
    }

    return val;
}

int msg_peek (msg_t* msg)
{
    if ( msg->index >= msg->len )
    {
        return -1;
    }

    return msg->buffer[msg->index];
}

int msg_parseInt(msg_t* msg)
{
    int sizeLeft = (msg->len) - (msg->index);
    char* numberBuffer = (char*) malloc( sizeLeft * sizeof(char)  );
    int numberIndex = 0;

    if ( msg_peek(msg) == '-' )
    {
        numberBuffer[numberIndex++] = msg_read(msg);
    }

    while ( isdigit( msg_peek(msg) )  )
    {
        numberBuffer[numberIndex++] = msg_read(msg);
    }

    numberBuffer[numberIndex] = 0;

    int numer = atoi(numberBuffer);
    free(numberBuffer);

    return numer;
}

void msg_test()
{
    Serial.println( "--- test1 --- " );
    msg_t test1 = {};
    test1.buffer = "hej";
    test1.len = 3;
    test1.index = 0;

    Serial.println( 'h' == msg_read(&test1) );
    Serial.println( 'e' == msg_read(&test1) );
    Serial.println( 'j' == msg_read(&test1) );
    Serial.println(  -1 == msg_read(&test1) );


    Serial.println( "--- test2 --- " );

    msg_t test2 = {};
    test2.buffer = "123,-42";
    test2.len = 7;
    test2.index = 0;

    Serial.println( 123 == msg_parseInt(&test2) );
    Serial.println( ',' == msg_read(&test2) );
    Serial.println( -42 == msg_parseInt(&test2) );
    Serial.println(  -1 == msg_read(&test2) );
}
