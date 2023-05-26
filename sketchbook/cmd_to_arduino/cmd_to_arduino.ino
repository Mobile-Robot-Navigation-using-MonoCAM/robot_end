// Motor left connections
int enA = 9;
int in1 = 7;
int in2 = 6;

// Motor right connections
int enB = 10;
int in3 = 5;
int in4 = 4;

void leftForward(int pwm)
{
    // Motor left forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, pwm); // Set motor A speed
}

void leftBackward(int pwm)
{
    // Motor left forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, pwm); // Set motor A speed
}

void leftStop()
{
    // Motor left stop
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0); // Set motor A speed to 0
}

void rightForward(int pwm)
{
    // Motor right forward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, pwm); // Set motor B speed
}

void rightBackward(int pwm)
{
    // Motor right forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enB, pwm); // Set motor B speed
}

void rightStop()
{
    // Motor right stop
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enB, 0); // Set motor B speed to 0
}

void setup() {
  // Set the motor control pins as outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(9600);
}

void loop() {
    // check availability of serial data:
    if (Serial.available() > 0)
    {
        // Read the incoming byte
        String inputString = Serial.readStringUntil('\n');
    
        // Process the received data
        int value1, value2;
        char *ptr = strtok(const_cast<char *>(inputString.c_str()), ":");
        if (ptr != NULL) {
            value1 = atoi(ptr);
            ptr = strtok(NULL, ":");
        }
        if (ptr != NULL) {
            value2 = atoi(ptr);
        }

        // Example: Print the received byte to the serial monitor    
        if (value1 == 0)
            leftStop();
        else
        {
            if (value1 < 0)
            {
                leftBackward(-value1);
            }
            else
            {
                leftForward(value1);
            }
        }

        if (value2 == 0)
            rightStop();
        else
        {
            if (value2 < 0)
            {
                rightBackward(-value2);
            }
            else
            {
                rightForward(value2);
            }
        }
    }
}
