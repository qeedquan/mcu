#include <LiquidCrystal.h>

#define nelem(x) (sizeof(x) / sizeof(x[0]))

enum {
  TILT = 6,
};

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int lastsw;

void setup() {
  pinMode(TILT, INPUT);
  Serial.begin(38400);
  lcd.begin(16, 2);
  lcd.print("Ask the");
  lcd.setCursor(0, 1);
  lcd.print("Crystal Ball!");
}

void loop() {
  const char *tab[] = {
    "Yes", "Most likely", "Certainly", "Outlook good",
    "Unsure", "Ask again", "Doubtful", "No",
  };
  int sw, reply;

  sw = digitalRead(TILT);
  Serial.println(sw);
  if (sw != lastsw && sw == LOW) {
    reply = random(nelem(tab));
    Serial.println(reply);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("The ball says:");
    lcd.setCursor(0, 1);
    lcd.print(tab[reply]);
  }
 
  lastsw = sw;
}
