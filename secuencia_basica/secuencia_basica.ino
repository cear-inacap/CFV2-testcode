/*
 * Autor: cefuve.com
 * v2
 *
 * Copyright (c) 2023 CEAR INACAP.  All right reserved.
 * License: GPL 3.0.
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions.
 */
 

// Definición de pines
#define   M1_sensor   36
#define   M2_sensor   39
#define   M3_sensor   34
#define   M4_sensor   35

#define   PWM_MOTOR   23

#define   TEST_LED    2

#define   SCL_GYRO    22
#define   SDA_GYRO    21

#define   TX_RPI  17
#define   RX_RPI  16

#define   SHIFT   32
#define   LATCH   33
#define   DATA    25

#define   BUZZER  12

#define   EN1A    26
#define   EN1B    27
#define   EN2A    14
#define   EN2B    13
#define   EN3A    19
#define   EN3B    4
#define   EN4A    5
#define   EN4B    18

// Declaración de palabras claves
#define   STP     0
#define   FWD     1
#define   REV     2

#define   M1      1
#define   M2      2
#define   M3      3
#define   M4      4

// Memoria enviada al registro de desplazamiento
// Contiene activacion/desactivacion de cada motor
byte motor_register = 0;

// Valor del ancho de pulso (0-255)
#define   PWMval  60

unsigned long actual_time = 0;

byte secuencia = 0;

void motor_control(byte id, byte dir){
    //M1: motor 1 (1)
    //M2: motor 2 (2)
    //M3: motor 3 (3)
    //M4: motor 4 (4)
    //STP: detener (0)
    //FWD: adelante (1)
    //REV: reversa (2)
    
    byte bit1;
    byte bit2;

    switch(id){
        case M1:  bit1 = 0;
                  bit2 = 1;
                  break;
        case M2:  bit1 = 2;
                  bit2 = 3;
                  break;
        case M3:  bit1 = 4;
                  bit2 = 5;
                  break;
        case M4:  bit1 = 6;
                  bit2 = 7;
                  break;
        default:  bit1 = 8;
                  bit2 = 9;
    }

    
    if(dir == STP){
        digital_out(bit1, LOW);
        digital_out(bit2, LOW);
    }
    else if(dir == FWD){
        digital_out(bit1, HIGH);
        digital_out(bit2, LOW);
    }
    else if(dir == REV){
        digital_out(bit1, LOW);
        digital_out(bit2, HIGH);
    }
}


void digital_out(byte pin, bool state) {
    bitWrite(motor_register, pin, state);
  
    shiftOut(DATA, SHIFT, MSBFIRST, motor_register);
    
    digitalWrite(LATCH, HIGH);
    digitalWrite(LATCH, LOW);
}


void setup() {
    Serial.begin(115200);
    pinMode(PWM_MOTOR, OUTPUT);
    pinMode(TEST_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    pinMode(DATA, OUTPUT);
    pinMode(SHIFT, OUTPUT);
    pinMode(LATCH, OUTPUT);

    ledcSetup(/*ledChannel*/0, /*freq*/100, /*resolution*/8);
    ledcAttachPin(PWM_MOTOR, /*ledChannel*/0);
    
    digitalWrite(LATCH, LOW);
    for(int i=0; i<8; i++) digital_out(i, LOW);

    ledcWrite(/*ledChannel*/0, /*value*/PWMval);

    digitalWrite(TEST_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(50);
    digitalWrite(BUZZER, LOW);
    delay(1000);
    digitalWrite(TEST_LED, LOW);
}


void loop() {
 
    if(millis() > actual_time + 1000){
        actual_time = millis();
        secuencia++;

        if(secuencia == 1) avanzar();
        if(secuencia == 2) parar();
        if(secuencia == 3) izquierda();
        if(secuencia == 4) parar();
        if(secuencia == 5) derecha();
        if(secuencia == 6) parar();
        if(secuencia == 7) retroceder();
        if(secuencia == 8) parar();

        if(secuencia > 12) secuencia = 0;
    }
    
}

void avanzar(){
    motor_control(M1, FWD);
    motor_control(M2, FWD);
    motor_control(M3, FWD);
    motor_control(M4, FWD);
}

void retroceder(){
    motor_control(M1, REV);
    motor_control(M2, REV);
    motor_control(M3, REV);
    motor_control(M4, REV);
}

void parar(){
    motor_control(M1, STP);
    motor_control(M2, STP);
    motor_control(M3, STP);
    motor_control(M4, STP);
}

void izquierda(){
    motor_control(M1, FWD);
    motor_control(M2, REV);
    motor_control(M3, FWD);
    motor_control(M4, REV);
}

void derecha(){
    motor_control(M1, REV);
    motor_control(M2, FWD);
    motor_control(M3, REV);
    motor_control(M4, FWD);
}

void giro_izquierda(){
    motor_control(M1, REV);
    motor_control(M2, FWD);
    motor_control(M3, FWD);
    motor_control(M4, REV);
}

void giro_derecha(){
    motor_control(M1, FWD);
    motor_control(M2, REV);
    motor_control(M3, REV);
    motor_control(M4, FWD);
}

void diagonal_derecha(){
    motor_control(M1, FWD);
    motor_control(M2, STP);
    motor_control(M3, FWD);
    motor_control(M4, STP);
}

void diagonal_izquierda(){
    motor_control(M1, STP);
    motor_control(M2, FWD);
    motor_control(M3, STP);
    motor_control(M4, FWD);
}
