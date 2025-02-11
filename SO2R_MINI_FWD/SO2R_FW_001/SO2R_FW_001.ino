const byte numChars = 10; //max number of OTSPR message
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;

byte OUT_PINS[] = {
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9
}; //OUTPUT PINS

bool RX_FOCUS_FLAG = false;
bool TX_FOCUS_FLAG = false;
bool IS_DISABLED = false;
bool IS_MUTE= false;
byte RX_STATE = 1;
byte TX1_CB = 0;
byte TX2_CB = 0;

void MuteRXAll(){                     //not used yet
        digitalWrite(OUT_PINS[4], LOW);
        digitalWrite(OUT_PINS[5], LOW);
        //digitalWrite(OUT_PINS[2], LOW);
        //digitalWrite(OUT_PINS[3], LOW);
}


void setup() {
    Serial.begin(9600);
    
    for (int i = 0; i < sizeof(OUT_PINS); i++) {
        pinMode(OUT_PINS[i], OUTPUT);
    }
// initial startup sequence - STEREO MODE // TX focus on TX1
    Set_RX_Stereo_TCVR1_TCVR2();
    digitalWrite(OUT_PINS[4], HIGH); //RX1 ON
    digitalWrite(OUT_PINS[5], HIGH); //RX2 ON

    digitalWrite(OUT_PINS[6], HIGH); //TX1 ON
    digitalWrite(OUT_PINS[7], LOW);  //TX2 OFF 

}

void loop() {
    recvWithEndMarker();
    showNewData();
}



void recvWithEndMarker() {
    static byte ndx = 0; //char array index
    char endMarker = '\r'; //command delimiter - for OTSPR is \r
    char rc; //rcv variable

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        } else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0; // reset index 
            newData = true; // reset flag
        }
    }
}

void showNewData() {
    if (newData == true) {
        if (strcmp(receivedChars, "?NAME") == 0) {
            Send_BoxDetails();
        }
        if (strcmp(receivedChars, "?FW") == 0) {
            Send_BoxFW();
        }
        
        if (strcmp(receivedChars, "RX1") == 0) {
            Set_RX_TCVR1();
        }
        if (strcmp(receivedChars, "RX2") == 0) {
            Set_RX_TCVR2();
        }
        if (strcmp(receivedChars, "TX1") == 0) {
            Set_TX_TCVR1();
        }
        if (strcmp(receivedChars, "TX2") == 0) {
            Set_TX_TCVR2();
        }
        if (strcmp(receivedChars, "MUTE") == 0) {
            MuteRXAll();
        }
        if (strcmp(receivedChars, "RX1S") == 0 or strcmp(receivedChars, "RX2S") == 0) {
            Set_RX_Stereo_TCVR1_TCVR2();
        }

        newData = false;
    }
}


void Set_RX_TCVR1() {
    MuteRXAll(); //Mute B4 Switch

  //STEREO RX1
    digitalWrite(OUT_PINS[0], HIGH);
    digitalWrite(OUT_PINS[3], HIGH);

    digitalWrite(OUT_PINS[4], HIGH);
    digitalWrite(OUT_PINS[5], LOW);
}

void Set_RX_TCVR2() {
    MuteRXAll(); //Mute B4 Switch
  //STEREO RX2
    digitalWrite(OUT_PINS[1], HIGH);
    digitalWrite(OUT_PINS[2], HIGH);

    digitalWrite(OUT_PINS[4], LOW);
    digitalWrite(OUT_PINS[5], HIGH);
}

void Set_TX_TCVR1() {
    digitalWrite(OUT_PINS[6], HIGH);
    digitalWrite(OUT_PINS[7], LOW);
}

void Set_TX_TCVR2() {
    digitalWrite(OUT_PINS[6], LOW);
    digitalWrite(OUT_PINS[7], HIGH);
}

void Set_RX_Stereo_TCVR1_TCVR2() {
    MuteRXAll(); //Mute B4 Switch
  //RX1 LEFT - RX2 RIGHT
    digitalWrite(OUT_PINS[0], HIGH);
    digitalWrite(OUT_PINS[2], HIGH);

    digitalWrite(OUT_PINS[4], HIGH);
    digitalWrite(OUT_PINS[5], HIGH);
}

// void Reset_All_Outputs() {
//     for (int i = 0; i < sizeof(OUT_PINS); i++) {
//         digitalWrite(OUT_PINS[i], LOW);
//     }
// }

void Send_BoxDetails(){
  Serial.print("NAME SO2R MINI - YO3HEX (C) 2025");
  Serial.write('\r');
}

void Send_BoxFW(){
  Serial.print("FW2.0.1");
  Serial.write('\r');
}
