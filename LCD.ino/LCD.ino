#include <Bounce2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// Button pins
#define BUTTON_FORWARD 2
#define BUTTON_BACK 3

// Navigation states
#define UNSET 0
#define FORWARD 1
#define BACKWARD 2

// Debounce
Bounce buttonForwardDebounce = Bounce();
Bounce buttonBackDebounce = Bounce();

// Previous button states
bool previousButtonForwardState = false;
bool previousButtonBackState = false;

// Custom LCD chars (progressBar)
byte beginProgressBar[8] = { 0b11111, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b11111 };
byte middleProgressBar[8] = { 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111 };
byte endProgressBar[8] = { 0b11111, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b11111 };
byte progressBarFilled1[8] = { 0b11111, 0b00000, 0b10000, 0b10000, 0b10000, 0b10000, 0b00000, 0b11111 };
byte progressBarFilled2[8] = { 0b11111, 0b00000, 0b11000, 0b11000, 0b11000, 0b11000, 0b00000, 0b11111 };
byte progressBarFilled3[8] = { 0b11111, 0b00000, 0b11100, 0b11100, 0b11100, 0b11100, 0b00000, 0b11111 };
byte progressBarFilled4[8] = { 0b11111, 0b00000, 0b11110, 0b11110, 0b11110, 0b11110, 0b00000, 0b11111 };
byte progressBarFilled5[8] = { 0b11111, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000, 0b11111 };

// Text to navigate through
const String text[] = {"while (!(succeed",
                       " = try()));",
                       "Hi!,",
                       "I am Ferry and",
                       "I'm enrolled in",
                       "an ICT",
                       "excellence",
                       "program at",
                       "university. I'm",
                       "a real team",
                       "player and I",
                       "very eager to",
                       "develop myself. ",
                       "I believe that",
                       "being a good",
                       "programmer is",
                       "not the most",
                       "important",
                       "characteristic",
                       "of a software",
                       "engineer.  What",
                       "really matters",
                       "is being ",
                       "innovative,",
                       "resilient and",
                       "taking",
                       "responsibility. ",
                       "Because I think",
                       "these will make",
                       "a difference.",
                       "Being eager and",
                       "thinking forward",
                       "are key. I am",
                       "ready to face",
                       "Google, are you",
                       "ready to face",
                       "me?",
                       ""
                      };

// Text pointer, where are we reading
byte currentRow = 0;
// Navigation state (prevents 2x forward / backward click by changing direction)
byte previousNavigation = UNSET;

void setup()
{
  pinMode(BUTTON_BACK, INPUT_PULLUP);
  pinMode(BUTTON_FORWARD, INPUT_PULLUP);

  buttonForwardDebounce.attach(BUTTON_FORWARD);
  buttonForwardDebounce.interval(5);

  buttonBackDebounce.attach(BUTTON_BACK);
  buttonBackDebounce.interval(5);

  lcd.begin(16, 2);

  lcd.createChar(5, beginProgressBar);
  lcd.createChar(6, middleProgressBar);
  lcd.createChar(7, endProgressBar);
  lcd.createChar(8, progressBarFilled1);
  lcd.createChar(9, progressBarFilled2);
  lcd.createChar(10, progressBarFilled3);
  lcd.createChar(11, progressBarFilled4);
  lcd.createChar(12, progressBarFilled5);

  initializeDisplay();
}

void loop()
{
  buttonForwardDebounce.update();
  buttonBackDebounce.update();

  bool buttonForwardDown = buttonForwardDebounce.read();
  bool buttonBackDown = buttonBackDebounce.read();

  if (!buttonForwardDown) {
    previousButtonForwardState = true;
  }
  else if (buttonForwardDown && previousButtonForwardState) {
    previousButtonForwardState = false;

    navigateForward();
  }

  if (!buttonBackDown) {
    previousButtonBackState = true;
  }
  else if (buttonBackDown && previousButtonBackState) {
    previousButtonBackState = false;

    navigateBackward();
  }
}

// Helper methods display
void navigateForward() {
  if (currentRow + 2 <= sizeof(text) / sizeof(String)) {
    if (previousNavigation == UNSET || previousNavigation == FORWARD) {
      printText(text[currentRow], text[currentRow + 1]);
      currentRow += 2;
    }
    else {
      printText(text[currentRow + 2], text[currentRow + 3]);
      currentRow += 4;
    }
    previousNavigation = FORWARD;
  }
}

void navigateBackward() {
  if (previousNavigation == FORWARD && currentRow - 4 >= 0) {
    printText(text[currentRow - 4], text[currentRow - 3]);
    currentRow -= 4;
    previousNavigation = BACKWARD;
  }
  else if (previousNavigation == BACKWARD && currentRow - 2 >= 0) {
    printText(text[currentRow - 2], text[currentRow - 1]);
    currentRow -= 2;
    previousNavigation = BACKWARD;
  }
}

void printText(String row1, String row2) {
  lcd.clear();
  lcd.print(row1);
  lcd.setCursor(0, 1);
  lcd.print(row2);
}

void initializeDisplay() {
  printText("Looking for an", "motivated intern");
  delay(4000);

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Googling ...");
  delay(1000);
  drawProgressBar();
  fillProgress();

  printText("Results found: 1", "'Ferry Jongmans'");
  delay(4000);

  printText("Push '>' to view", "search result");
}

void drawProgressBar() {
  lcd.setCursor(0, 1);
  lcd.write(5);
  for (int i = 0; i < 14; i++) {
    lcd.write(6);
  }
  lcd.write(7);
}

void fillProgress() {
  byte count = 0;
  byte cursor = 0;
  for (int i = 0; i < 80; i++) {
    lcd.setCursor(cursor, 1);
    switch (count) {
      case 0:
        lcd.write(8);
        break;
      case 1:
        lcd.write(9);
        break;
      case 2:
        lcd.write(10);
        break;
      case 3:
        lcd.write(11);
        break;
      case 4:
        lcd.write(12);
        break;
    }
    if (count < 4) {
      count++;
    } else {
      count = 0;
      cursor++;
    }
    delay(50);
  }
}
