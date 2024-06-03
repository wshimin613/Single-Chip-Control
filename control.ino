#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

int vm1_btn = 14;             // vm1 btn D5 腳位
int vm1_btn_val = 0;          // vm1 btn 電位

int vm2_btn = 12;             // vm2 btn D6 腳位
int vm2_btn_val = 0;          // vm2 btn 電位

int btn1_standby = 0;         // vm1 btn 狀態
int btn2_standby = 0;         // vm2 btn 狀態

int vm1_status = 0;         // vm1 電腦狀態
int vm2_status = 0;         // vm2 電腦狀態

int btn1_count = 0;           // vm1 btn 次數
int btn2_count = 0;           // vm2 btn 次數

int data = 1;                 // 接收數值變數

int connect_seconds = 0;              // 中斷讀秒
//int state = 0;                // Serial Monitor 防呆
int connect_state = 0;        // USB 連接狀態
int check_state = 0;          // 狀態連接確認


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(vm1_btn,INPUT_PULLUP);
  pinMode(vm2_btn,INPUT_PULLUP);
  
  lcd.init();
  lcd.backlight();      // Make sure backlight is on
  lcd.setCursor(1,0);   //Set cursor to character 2 on line 0
  lcd.print("Host is Opening"); 

}

void loop() {
  // put your main code here, to run repeatedly:
  vm1_btn_val = digitalRead(vm1_btn);
  vm2_btn_val = digitalRead(vm2_btn);
  clicked();  // 偵測按鈕
  
  if ( connect_seconds > 3000 && check_state == 0 ) {
      Serial.println("check");
      check_state = 1;
  }
  
  connect_seconds++;
  delay(1);
}

void clicked() {
    // VM1 按鈕
    if ( vm1_btn_val == LOW && btn1_standby == 1 ) {
      check_state = 1;
      btn1_standby = 0;
      connect_seconds = 0;
      btn1_count++;
    }
    else if ( vm1_btn_val == HIGH && btn1_standby == 0 ) {
      btn1_standby = 1;
    }

    // VM2 按鈕
    if ( vm2_btn_val == LOW && btn2_standby == 1 ) {
      check_state = 1;
      btn2_standby = 0;
      connect_seconds = 0;
      btn2_count++;
    }
    else if ( vm2_btn_val == HIGH && btn2_standby == 0 ) {
      btn2_standby = 1;
    }

    if ( connect_seconds >= 500 ){
      switch (btn1_count){
        case 1:
          lcd.clear();
          lcd.setCursor(0,0);
          if ( vm1_status == 0 ){
            lcd.print("L Starting");
          } else if ( vm1_status == 1 ){
            lcd.print("L Opened");  
          }
          Serial.println("vm1_open");
          btn1_standby = 0;
          btn1_count = 0;
          break;
        case 4:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("L Closing");
          Serial.println("vm1_destroy");
          btn1_standby = 0;
          btn1_count = 0;
          break;
        default:
          btn1_standby = 1;
          btn1_count = 0;
          break;
      }
      switch (btn2_count){
        case 1:
          lcd.clear();
          lcd.setCursor(0,0);
          if ( vm2_status == 0 ){
            lcd.print("R Starting");
          } else if ( vm2_status == 1 ){
            lcd.print("R Opened");
          }
          Serial.println("vm2_open");
          btn2_standby = 0;
          btn2_count = 0;
          break;
        case 4:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("R Closing");
          Serial.println("vm2_destroy");
          btn2_standby = 0;
          btn2_count = 0;
          break;
        default:
          btn2_standby = 1;
          btn2_count = 0;
          break;
      }
    }
    if ( Serial.available() > 0 ) {
      check_state = 0;
      connect_seconds = 0;
      value();
      connect_state = 0;
//      state = 1;
    }
    else {
//      state = 0;
      if ( connect_seconds > 7000 ) {
        if ( connect_state == 0 ) {
          lcd.clear();
        }
        lcd.setCursor(0,0);
        lcd.print("Not Connected");
        btn1_standby = 0;
        btn2_standby = 0;
        connect_state = 1;
      }
    }
//    Serial.println(connect_seconds);
}

void value() {
  data = Serial.parseInt();
  
  switch (data) {
    // 按鈕狀態初始化
    case 0:
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Host online");
      lcd.setCursor(0,1);
      lcd.print("L");
      lcd.setCursor(4,1);
      if ( vm1_status == 0 ){
        lcd.print("off");
      } else if ( vm1_status == 1 ){
        lcd.print("on");
      } else {
        lcd.print("err");
      }
      lcd.setCursor(9,1);
      lcd.print("R");
      lcd.setCursor(13,1);
      if ( vm2_status == 0 ){
        lcd.print("off");
      } else if ( vm2_status == 1 ){
        lcd.print("on");
      } else {
        lcd.print("err");
      }
      btn1_standby = 1;
      btn2_standby = 1;
      check_state = 0;
      connect_seconds = 0;
      break;

    // VM1 狀態
    case 10:  // VM1 關機
      btn1_standby = 0;
      check_state = 0;
      vm1_status = 0;
      break;
    case 11:  // VM1 開機
      btn1_standby = 0;
      check_state = 0;
      vm1_status = 1;
      break;
    case 12:  // VM1 已開機
      btn1_standby = 0;
      check_state = 0;
      vm1_status = 1;
      break;
    case 13:  // VM1 錯誤
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("L Error");
      btn1_standby = 0;
      check_state = 0;
      vm1_status = 3;
      break;

    // VM2 狀態
    case 20:  // VM2 關機
      btn2_standby = 0;
      check_state = 0;
      vm2_status = 0;
      break;
    case 21:  // VM2 開機
      btn2_standby = 0;
      check_state = 0;
      vm2_status = 1;
      break;
    case 22:  // VM2 已開機
      btn2_standby = 0;
      check_state = 0;
      vm2_status = 1;
      break;
    case 23:  // VM2 錯誤
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("R Error");
      btn2_standby = 0;
      check_state = 0;
      vm2_status = 3;
      break;

    case 300:
      vm1_status = 0;
      vm2_status = 0;
      break;
    case 310:
      vm1_status = 1;
      vm2_status = 0;
      break;
    case 301:
      vm1_status = 0;
      vm2_status = 1;
      break;
    case 311:
      vm1_status = 1;
      vm2_status = 1;
      break;
  }
}
