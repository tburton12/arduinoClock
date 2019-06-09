// Print current time and date on Serial
void printTime(DS1302 rtc){
  // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());
  
}

// Print current temperature and humidity on Serial
void printDHTInfo(DHT dht, int dht_pin){
   float humidity = dht.getHumidity();
   float temperature = dht.getTemperature();
   Serial.print("Current humidity = ");
   Serial.print(humidity);     
   Serial.print("% ");     
   Serial.print("temperature = ");     
   Serial.print(temperature);     
   Serial.println("C ");     
     
}
