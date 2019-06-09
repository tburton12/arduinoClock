// Updating time desired fields on LCD with current data
void update_time(LiquidCrystal_I2C lcd, Time t){
  // Clear first row
  lcd.setCursor(0,0);
  lcd.print("                ");
  
  // Print time
  lcd.setCursor(0,0);
  if (t.hour < 10)
    lcd.print("0");
  lcd.print(t.hour, DEC);
  lcd.print(":");
  if (t.min < 10)
    lcd.print("0");
  lcd.print(t.min, DEC);
  lcd.print(":");
  if (t.sec < 10)
    lcd.print("0");
  lcd.print(t.sec, DEC);
  lcd.setCursor(11,0);

  // Print date
  lcd.setCursor(11,0);
  if (t.date < 10)
    lcd.print("0");
  lcd.print(t.date, DEC);
  lcd.print(".");
  if (t.mon < 10)
    lcd.print("0");
  lcd.print(t.mon, DEC);
  
  
}

// Updating only humidity desired fields on LCD with current data
void update_humidity(LiquidCrystal_I2C lcd, float humidity){
  // Clear humidity space
  lcd.setCursor(0,1);
  lcd.print("        ");

  // Print humidity
  lcd.setCursor(0,1);
  lcd.print(humidity);
  lcd.print("%");
}

// Updating only temperature desired fields on LCD with current data
void update_temperature(LiquidCrystal_I2C lcd, float temperature){
  // Clear humidity space
  lcd.setCursor(7,1);
  lcd.print("         ");

  // Print temperature
  lcd.setCursor(10,1);
  lcd.print(temperature);
  lcd.print(char(223));
}

// Display date and time in settings mode
void display_date_time_settings(LiquidCrystal_I2C lcd, Time t){
  lcd.clear();
  lcd.setCursor(4,0);
  if (t.hour < 10)
    lcd.print("0");
  lcd.print(t.hour, DEC);
  lcd.print(":");
  if (t.min < 10)
    lcd.print("0");
  lcd.print(t.min, DEC);
  lcd.print(":");
  if (t.sec < 10)
    lcd.print("0");
  lcd.print(t.sec, DEC);
  lcd.setCursor(11,0);

  // Print date
  lcd.setCursor(3,1);
  if (t.date < 10)
    lcd.print("0");
  lcd.print(t.date, DEC);
  lcd.print(".");
  if (t.mon < 10)
    lcd.print("0");
  lcd.print(t.mon, DEC);
  lcd.print(".");
  if (t.year < 10)
    lcd.print("0");
  lcd.print(t.year, DEC);
}
