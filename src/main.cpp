// based on https://www.instructables.com/Arduino-MM-Color-Sorter/

#include <Arduino.h>

// Uncomment to initialize color settings for your setup. Insert one of these m&m's in this order to set color values: red, yellow, orange, green, blue, brown
//#define doColorSetup true

// Uncomment to do servo position testing only
//#define doServoTest true

// servo positions for second stage sorting compartment
// change accrodingly to your setup
const int servoPositions[3] = {20, 55, 163};

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int whitePin = 13;
const int limitPin = 8;
const int servoPin = 3;

const int lightSensorPin = A0;
const int lightThreshold = 8;
const int sampleCount = 50;  // number of samples to take for each color reading
const int stabilizationDelay = 300;  // Time for LED to stabilize before reading

struct ColorReference {
    const String name;
    int red;
    int green;
    int blue;
    int count;
};

const int colorCount = 6;

ColorReference colors[colorCount] = {
    {"red", 12, 5, 5, 0},
    {"yellow", 13, 10, 5, 0},
    {"orange", 12, 7, 4, 0},
    {"green", 7, 9, 5, 0},
    {"blue", 6, 6, 6, 0},
    {"brown", 7, 5, 4, 0}};

ColorReference testColor = {"test", 0, 0, 0, 0};

int emptyChamberCount = 0;
const int maxEmptyChamberCount = 5;

#include <Stepper.h>
Stepper myStepper = Stepper(2048, 2, 6, 4, 7);

#include <Servo.h>
Servo myServo;

/**
 * @brief Sets the color of the LED by specifying the intensity of red, green, and blue components.
 *
 * @param redValue Intensity of the red component (0-255).
 * @param greenValue Intensity of the green component (0-255).
 * @param blueValue Intensity of the blue component (0-255).
 */
void setLED(int redValue, int greenValue, int blueValue) {
    analogWrite(redPin, redValue);
    analogWrite(greenPin, greenValue);
    analogWrite(bluePin, blueValue);
}

/**
 * @brief Updates the reference color array with newly associated values.
 *
 * @param colorId The index of the color to update in the colors array.
 */
void updateColorReference(int colorId) {
    // updates the color reference array with newly associated values
    for (int i = 0; i < 3; i++) {
        colors[colorId].red = colors[colorId].red + ((testColor.red - colors[colorId].red) / (colors[colorId].count + 1));
        colors[colorId].green = colors[colorId].green + ((testColor.green - colors[colorId].green) / (colors[colorId].count + 1));
        colors[colorId].blue = colors[colorId].blue + ((testColor.blue - colors[colorId].blue) / (colors[colorId].count + 1));
    }
    // incriments the color array counter (how many of this color have been sorted)
    colors[colorId].count++;
}

/**
 * @brief Moves the stepper motor clockwise until the limit switch is triggered.
 *
 */
void moveToLimit() {
    int i = 0;  // provides failsafe for jamming
    myStepper.step(200);
    while (digitalRead(limitPin) == HIGH) {
        myStepper.step(10);
        if (i > 200) {
            Serial.println("Jammed, Reversing");
            myStepper.step(-300);
            i = 0;
        }
        i++;
    }
    Serial.println("Limit Reached");
}

/**
 * @brief Tests the color by reading the light sensor values under different LED colors.
 * Stores the averaged RGB values in testColor.
 *
 */
void getColorValues() {
    // check if chamber is not empty
    digitalWrite(whitePin, HIGH);
    delay(100);  // Increased delay for white LED stabilization
    int check = analogRead(lightSensorPin);
    digitalWrite(whitePin, LOW);

    Serial.print("Light Check: ");
    Serial.println(check);

    // if the chamber is empty, return zeros
    if (check >= lightThreshold) {
        Serial.println("Nothing in chamber");
        emptyChamberCount++;
        testColor.red = 0;
        testColor.green = 0;
        testColor.blue = 0;
        return;
    }

    delay(100);
    emptyChamberCount = 0;

    Serial.println("Testing Color...");

    // Measure ambient light level (LED off)
    setLED(0, 0, 0);
    delay(50);
    int ambient = 0;
    for (int i = 0; i < 10; i++) {
        ambient += analogRead(lightSensorPin);
        delay(5);
    }
    ambient = ambient / 10;
    Serial.print("Ambient light: ");
    Serial.println(ambient);

    // Collect RED light reflection
    setLED(255, 0, 0);
    delay(stabilizationDelay);
    int readings[sampleCount];
    
    for (int i = 0; i < sampleCount; i++) {
        readings[i] = analogRead(lightSensorPin);
        delay(10);  // Small delay between readings
    }
    
    // Sort and take median to filter outliers
    for (int i = 0; i < sampleCount - 1; i++) {
        for (int j = 0; j < sampleCount - i - 1; j++) {
            if (readings[j] > readings[j + 1]) {
                int temp = readings[j];
                readings[j] = readings[j + 1];
                readings[j + 1] = temp;
            }
        }
    }
    
    // Average the middle 70% of readings (discard outliers)
    int startIdx = sampleCount * 0.15;
    int endIdx = sampleCount * 0.85;
    long sum = 0;
    for (int i = startIdx; i < endIdx; i++) {
        sum += readings[i];
    }
    testColor.red = sum / (endIdx - startIdx);
    
    Serial.print("Red - Min: ");
    Serial.print(readings[0]);
    Serial.print(" | Max: ");
    Serial.print(readings[sampleCount - 1]);
    Serial.print(" | Avg: ");
    Serial.print(testColor.red);
    Serial.print(" | Corrected: ");
    int correctedRed = testColor.red - ambient;
    Serial.println(correctedRed > 0 ? correctedRed : 0);

    // Collect GREEN light reflection
    setLED(0, 255, 0);
    delay(stabilizationDelay);
    
    for (int i = 0; i < sampleCount; i++) {
        readings[i] = analogRead(lightSensorPin);
        delay(10);
    }
    
    // Sort for median filtering
    for (int i = 0; i < sampleCount - 1; i++) {
        for (int j = 0; j < sampleCount - i - 1; j++) {
            if (readings[j] > readings[j + 1]) {
                int temp = readings[j];
                readings[j] = readings[j + 1];
                readings[j + 1] = temp;
            }
        }
    }
    
    sum = 0;
    for (int i = startIdx; i < endIdx; i++) {
        sum += readings[i];
    }
    testColor.green = sum / (endIdx - startIdx);
    
    Serial.print("Green - Min: ");
    Serial.print(readings[0]);
    Serial.print(" | Max: ");
    Serial.print(readings[sampleCount - 1]);
    Serial.print(" | Avg: ");
    Serial.print(testColor.green);
    Serial.print(" | Corrected: ");
    int correctedGreen = testColor.green - ambient;
    Serial.println(correctedGreen > 0 ? correctedGreen : 0);

    // Collect BLUE light reflection
    setLED(0, 0, 255);
    delay(stabilizationDelay);
    
    for (int i = 0; i < sampleCount; i++) {
        readings[i] = analogRead(lightSensorPin);
        delay(10);
    }
    
    // Sort for median filtering
    for (int i = 0; i < sampleCount - 1; i++) {
        for (int j = 0; j < sampleCount - i - 1; j++) {
            if (readings[j] > readings[j + 1]) {
                int temp = readings[j];
                readings[j] = readings[j + 1];
                readings[j + 1] = temp;
            }
        }
    }
    
    sum = 0;
    for (int i = startIdx; i < endIdx; i++) {
        sum += readings[i];
    }
    testColor.blue = sum / (endIdx - startIdx);
    
    Serial.print("Blue - Min: ");
    Serial.print(readings[0]);
    Serial.print(" | Max: ");
    Serial.print(readings[sampleCount - 1]);
    Serial.print(" | Avg: ");
    Serial.print(testColor.blue);
    Serial.print(" | Corrected: ");
    int correctedBlue = testColor.blue - ambient;
    Serial.println(correctedBlue > 0 ? correctedBlue : 0);

    // Print ratio information for color discrimination
    Serial.println("--- Color Ratios ---");
    if (testColor.green > 0) {
        Serial.print("R/G ratio: ");
        Serial.println((float)testColor.red / testColor.green);
    }
    if (testColor.blue > 0) {
        Serial.print("R/B ratio: ");
        Serial.println((float)testColor.red / testColor.blue);
        Serial.print("G/B ratio: ");
        Serial.println((float)testColor.green / testColor.blue);
    }

    // turn LED off and update overall m&m counter
    setLED(0, 0, 0);
    testColor.count++;
}

/**
 * @brief Finds the closest matching color ID based on the tested color values.
 *
 * @return int The ID of the matched color, or -1 if no match is found.
 */
int findMatch() {
    int matchedColorId = -1;

    // if chamber is empty return -1
    if (testColor.red == 0) {
        return matchedColorId;
    }

    long sums[colorCount];

    for (int i = 0; i < colorCount; i++) {
        sums[i] = pow(testColor.red - colors[i].red, 2) + pow(testColor.green - colors[i].green, 2) + pow(testColor.blue - colors[i].blue, 2);
    }

    long closest = min(sums[0], min(sums[1], min(sums[2], min(sums[3], min(sums[4], sums[5])))));

    if (closest == sums[0]) {
        matchedColorId = 0;
    } else if (closest == sums[1]) {
        matchedColorId = 1;
    } else if (closest == sums[2]) {
        matchedColorId = 2;
    } else if (closest == sums[3]) {
        matchedColorId = 3;
    } else if (closest == sums[4]) {
        matchedColorId = 4;
    } else {
        matchedColorId = 5;
    }

    colors[matchedColorId].count++;

    Serial.print("Matched Color: ");
    Serial.println(colors[matchedColorId].name);

    return matchedColorId;
}

/**
 * @brief Moves the stepper motor and servo to sort the item based on the color ID.
 *
 * @param colorId The index of the color to sort.
 */
void sort(int colorId) {
    Serial.print("Sorting to: ");
    Serial.println(colors[colorId].name);

    switch (colorId) {
        case 0:
            myServo.write(servoPositions[2]);
            myStepper.step(-1650);
            moveToLimit();
            break;
        case 1:
            myServo.write(servoPositions[1]);
            myStepper.step(-1650);
            moveToLimit();
            break;
        case 2:
            myServo.write(servoPositions[0]);
            myStepper.step(-1650);
            moveToLimit();
            break;
        case 3:
            myServo.write(servoPositions[2]);
            moveToLimit();
            break;
        case 4:
            myServo.write(servoPositions[1]);
            moveToLimit();
            break;
        case 5:
            myServo.write(servoPositions[0]);
            moveToLimit();
            break;
        default:
            Serial.println("No valid color selected for sorting");
            moveToLimit();
            break;
    }
}

/**
 * @brief Displays the sorting statistics on the serial monitor.
 *
 */
void showStats() {
    Serial.println("Sorting Complete!");
    Serial.print("Total: ");
    Serial.println(testColor.count);

    for (int i = 0; i < colorCount; i++) {
        Serial.print(colors[i].name);
        Serial.print(": ");
        Serial.println(colors[i].count);
    }

    // ends program
    delay(400);
    exit(0);
}

/**
 * @brief Set the Color References for each m&m color
 *
 */
void setColorReferences() {
    moveToLimit();

    for (int i = 0; i < colorCount; i++) {
        getColorValues();
        updateColorReference(i);
        sort(i);
    }
}

/**
 * @brief Tests the servo by moving it through predefined positions.
 *
 */
void servoTest() {
    Serial.println("Testing Servo...");

    for (int i = 0; i < 3; i++) {
        Serial.print("Position ");
        Serial.println(i);
        myServo.write(servoPositions[i]);
        delay(2000);
    }
}

void setup() {
    delay(100);
    Serial.begin(115200);
    Serial.println("M&M Sorter Initializing...");

    // set LEDs to output and limit switch to input w/ pullup resistor
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(whitePin, OUTPUT);
    pinMode(limitPin, INPUT_PULLUP);

    pinMode(lightSensorPin, INPUT);

    // Stepper motor, Servo motor setup
    myStepper.setSpeed(15);
    myServo.attach(servoPin);
    myServo.write(servoPositions[1]);

#ifdef doColorSetup
    setColorReferences();
    Serial.println("Color Settings Complete!");
    Serial.println("values are:");
    for (int i = 0; i < colorCount; i++) {
        Serial.print(colors[i].name);
        Serial.print(" [red: ");
        Serial.print(colors[i].red);
        Serial.print("] [green: ");
        Serial.print(colors[i].green);
        Serial.print("] [blue: ");
        Serial.print(colors[i].blue);
        Serial.println("]");
    }
#endif
}

void loop() {
#ifdef doServoTest
    servoTest();
#else

    getColorValues();
    sort(findMatch());

    if (emptyChamberCount >= maxEmptyChamberCount) {
        showStats();
    }
#endif
}