#include <U8g2lib.h>
#include <UiUiUi.h>

//XBM DATA
#define smiley_width 20
#define smiley_height 20
static const unsigned char smiley_bits[] U8X8_PROGMEM = {
   0x80, 0x1f, 0x00, 0xe0, 0x7f, 0x00, 0x78, 0xe0, 0x01, 0x1c, 0x80, 0x03,
   0x0c, 0x00, 0x03, 0x06, 0x00, 0x06, 0x06, 0x00, 0x06, 0xc3, 0x30, 0x0c,
   0xc3, 0x30, 0x0c, 0x03, 0x00, 0x0c, 0x03, 0x00, 0x0c, 0x03, 0x00, 0x0c,
   0x43, 0x20, 0x0c, 0xc6, 0x79, 0x06, 0x86, 0x1f, 0x06, 0x0c, 0x06, 0x03,
   0x1c, 0x80, 0x03, 0x78, 0xe0, 0x01, 0xe0, 0x7f, 0x00, 0x80, 0x1f, 0x00 };

UIBitmap smiley1 = UIBitmap(smiley_width, smiley_height, smiley_bits);
UIBitmap smiley2 = UIBitmap(smiley_width, smiley_height, smiley_bits);
UIBitmap smiley3 = UIBitmap(smiley_width, smiley_height, smiley_bits);
UIBitmap smiley4 = UIBitmap(smiley_width, smiley_height, smiley_bits);

UIEnvelope pad1 = UIEnvelope(UIExpansion::None, UIAlignment::Center, UISize(1,1), &smiley1);
UIEnvelope pad2 = UIEnvelope(UIExpansion::Horizontal, UIAlignment::Center, UISize(1,1), &smiley2);
UIEnvelope pad3 = UIEnvelope(UIExpansion::Vertical, UIAlignment::Center, UISize(1,1), &smiley3);
UIEnvelope pad4 = UIEnvelope(UIExpansion::Both, UIAlignment::Center, UISize(1,1), &smiley4);

UIBorder border1 = UIBorder(2,&pad1);
UIBorder border2 = UIBorder(2,&pad2);
UIBorder border3 = UIBorder(2,&pad3);
UIBorder border4 = UIBorder(2,&pad4);

UIEnvelope marg1 = UIEnvelope(UIExpansion::Both, UIAlignment::Center, UISize(1,1), &border1);
UIEnvelope marg2 = UIEnvelope(UIExpansion::Both, UIAlignment::Center, UISize(1,1), &border2, &marg1);
UIEnvelope marg3 = UIEnvelope(UIExpansion::Both, UIAlignment::Center, UISize(1,1), &border3);
UIEnvelope marg4 = UIEnvelope(UIExpansion::Both, UIAlignment::Center, UISize(1,1), &border4, &marg3);

UIRows rows1 = UIRows(&marg2);
UIRows rows2 = UIRows(&marg4,&rows1);

UIColumns cols1 = UIColumns(&rows2);

UIDisplay display = UIDisplay(&cols1);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C screen(U8G2_R2);

void setup() {
  Serial.begin(115200);
  screen.begin();
  display.init(&screen);
}

void loop() {
  display.render(&screen);
}