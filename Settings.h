/*  Define various values  */
#define TIME_REFRESH_PERIOD 1000
#define DHT_REFRESH_PERIOD 2000
#define MIN_LONG_PRESS_TIME 750
#define TIME_SETTINGS_REFRESH_PERIOD 400
#define CURSOR_BLINKING_PERIOD 700

// Cooldown between pushing the same button. We use it to avoid multiple actions in case of unwanted voltage interrupts
#define BUTTONS_COOLDOWN 140

/*  PINS DEFINITION */
// DHT PIN
#define DHT_PIN 2

// RTC PINS
#define RTC_RST_PIN 8
#define RTC_DAT_PIN 7
#define RTC_CLK_PIN 6

// Buttons Pins
#define LEFT_ARROW 19
#define MODE 18
#define RIGHT_ARROW 3
