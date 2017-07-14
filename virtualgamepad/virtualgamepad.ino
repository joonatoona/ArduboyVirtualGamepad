#include <Arduboy2.h>

// Use library https://github.com/MHeironimus/ArduinoJoystickLibrary/tree/version-1.0
#include <Joystick.h>

// Create the Joystick with 4 buttons
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_GAMEPAD, 4,  0,  true, true, false, false, false, false,
                   false, false, false, false, false);

Arduboy2 arduboy;

unsigned long holdSpecialButton = 0, startHeldDown = 0, selectHeldDown = 0;
bool startButton = false, selectButton = false;
const int m = 0xFFFF;
const int buttonHeldTime = 300, buttonReleaseTime = 1000;

void customBegin() {
  arduboy.boot();
  arduboy.blank();
  arduboy.flashlight();
  arduboy.systemButtons();
  do {
    delay(50);
  } while (arduboy.buttonsState());
}


void setup() {
  customBegin();
  arduboy.setFrameRate(120);

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setXAxisRange(-m, m);
  Joystick.setYAxisRange(-m, m);
}

bool dirty = true;
int prevx = 0, prevy = 0;
bool button0, button1;

void loop() {
  //Prevent the Arduboy from running too fast
  if (!arduboy.nextFrame())
    return;

  int x = 0, y = 0;

  if (holdSpecialButton < millis())
  {
    if (startButton || selectButton)
    {
      Joystick.setButton(2, false);
      Joystick.setButton(3, false);
      digitalWrite(LED_BUILTIN_TX, HIGH);
      selectButton = false;
      startButton = false;
    }

    if (arduboy.pressed(UP_BUTTON))
      y -= m;

    if (arduboy.pressed(DOWN_BUTTON))
      y += m;

    if (arduboy.pressed(LEFT_BUTTON))
      x -= m;

    if (arduboy.pressed(RIGHT_BUTTON))
      x += m;

    if (x != prevx)
    {
      Joystick.setXAxis(x);
      digitalWrite(LED_BUILTIN_TX, HIGH);
      prevx = x;
    }
    if (y != prevy)
    {
      Joystick.setYAxis(y);
      digitalWrite(LED_BUILTIN_TX, HIGH);
      prevy = y;
    }

    if (x == 0 && arduboy.pressed(LEFT_BUTTON) && arduboy.notPressed(A_BUTTON + B_BUTTON)) // Start button
    {
      if (startHeldDown == 0)
        startHeldDown = millis();
      else if (millis() - startHeldDown > buttonHeldTime)
      {
        holdSpecialButton = millis() + buttonReleaseTime;
        Joystick.setButton(3, true);
        digitalWrite(LED_BUILTIN_TX, HIGH);
        startButton = true;
      }
    }
    else
      startHeldDown = 0;

    if (y == 0 && arduboy.pressed(UP_BUTTON) && arduboy.notPressed(A_BUTTON + B_BUTTON)) // Select button
    {
      if (selectHeldDown == 0)
        selectHeldDown = millis();
      else if (millis() - selectHeldDown > buttonHeldTime)
      {
        holdSpecialButton = millis() + buttonReleaseTime;
        Joystick.setButton(2, true);
        digitalWrite(LED_BUILTIN_TX, HIGH);
        selectButton = true;
      }
    }
    else
      selectHeldDown = 0;

    if (button0 != arduboy.pressed(A_BUTTON))
    {
      button0 = !button0;
      Joystick.setButton(0, button0);
      digitalWrite(LED_BUILTIN_TX, HIGH);
    }

    if (button1 != arduboy.pressed(B_BUTTON))
    {
      button1 = !button1;
      Joystick.setButton(1, button1);
      digitalWrite(LED_BUILTIN_TX, HIGH);
    }
  }

  dirty = !arduboy.nextFrame();

  if (dirty)
  {
    arduboy.clear();

    // Draw joystick
    switch (x)
    {
      case 0: arduboy.drawTriangle(18, 13, 30, 25, 18, 37, WHITE); arduboy.drawRect(7, 13, 12, 25);
        arduboy.drawTriangle(46, 13, 46, 37, 34, 25, WHITE); arduboy.drawRect(46, 13, 12, 25);
        arduboy.drawLine(18, 14, 18, 36, BLACK); arduboy.drawLine(46, 14, 46, 36, BLACK);
        break;
      case -m: arduboy.fillTriangle(18, 13, 30, 25, 18, 37, WHITE); arduboy.fillRect(7, 13, 12, 25);
        arduboy.drawTriangle(46, 13, 46, 37, 34, 25, WHITE); arduboy.drawRect(46, 13, 12, 25);
        /*arduboy.drawLine(18, 14, 18, 36, BLACK);*/ arduboy.drawLine(46, 14, 46, 36, BLACK);
        break;
      case m: arduboy.drawTriangle(18, 13, 30, 25, 18, 37, WHITE); arduboy.drawRect(7, 13, 12, 25);
        arduboy.fillTriangle(46, 13, 46, 37, 34, 25, WHITE); arduboy.fillRect(46, 13, 12, 25);
        arduboy.drawLine(18, 14, 18, 36, BLACK); /*arduboy.drawLine(46, 14, 46, 36, BLACK);*/
        break;
    }

    switch (y)
    {
      case 0: arduboy.drawTriangle(32, 23, 44, 11, 20, 11, WHITE); arduboy.drawRect(20, 0, 25, 12);
        arduboy.drawTriangle(32, 27, 44, 39, 20, 39, WHITE); arduboy.drawRect(20, 39, 25, 12);
        arduboy.drawLine(43, 11, 21, 11, BLACK); arduboy.drawLine(21, 39, 43, 39, BLACK);
        break;
      case -m: arduboy.fillTriangle(32, 23, 44, 11, 20, 11, WHITE); arduboy.fillRect(20, 0, 25, 12);
        arduboy.drawTriangle(32, 27, 44, 39, 20, 39, WHITE); arduboy.drawRect(20, 39, 25, 12);
        /*arduboy.drawLine(43, 11, 21, 11, BLACK);*/ arduboy.drawLine(21, 39, 43, 39, BLACK);
        break;
      case m: arduboy.drawTriangle(32, 23, 44, 11, 20, 11, WHITE); arduboy.drawRect(20, 0, 25, 12);
        arduboy.fillTriangle(32, 27, 44, 39, 20, 39, WHITE); arduboy.fillRect(20, 39, 25, 12);
        arduboy.drawLine(43, 11, 21, 11, BLACK); /*arduboy.drawLine(21, 39, 43, 39, BLACK);*/
        break;
    }

    if (arduboy.pressed(A_BUTTON))
      arduboy.fillCircle(88, 30, 12);
    else
      arduboy.drawCircle(88, 30, 12);

    if (arduboy.pressed(B_BUTTON))
      arduboy.fillCircle(115, 24, 12);
    else
      arduboy.drawCircle(115, 24, 12);

    if (selectButton)
      arduboy.fillRoundRect(40, 56, 20, 8, 1);
    else
      arduboy.drawRoundRect(40, 56, 20, 8, 1);

    if (startButton)
      arduboy.fillRoundRect(68, 56, 20, 8, 1);
    else
      arduboy.drawRoundRect(68, 56, 20, 8, 1);

    // Rounding
    arduboy.drawPixel(20, 0, BLACK);
    arduboy.drawPixel(44, 0, BLACK);
    arduboy.drawPixel(20, 50, BLACK);
    arduboy.drawPixel(44, 50, BLACK);
    arduboy.drawPixel(7, 13, BLACK);
    arduboy.drawPixel(7, 37, BLACK);
    arduboy.drawPixel(57, 13, BLACK);
    arduboy.drawPixel(57, 37, BLACK);

    arduboy.display();
    dirty = false;
  }
}
