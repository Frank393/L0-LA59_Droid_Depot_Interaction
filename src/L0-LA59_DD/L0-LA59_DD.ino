//L0-LA59_Droid_Depot_Interaction V1
//Author: Frank393
//https://github.com/Frank393
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEServer.h>

// Define variables
int scanTime = 5; // scan time in seconds
BLEScan* pBLEScan;// pointer to BLE scan object
BLEAdvertising* pAdvertising; // pointer to BLE advertising object

int Sound = D1;

// Define the list of BLE MAC addresses for landspeeder and droid units
uint8_t landList[][9] = {
  {0x0A, 0x04, 0x01, 0x02, 0xA6, 0x01},
  {0x0A, 0x04, 0x02, 0x02, 0xA6, 0x01},
  {0x0A, 0x04, 0x03, 0x02, 0xA6, 0x01},
  {0x0A, 0x04, 0x04, 0x02, 0xA6, 0x01},
  {0x0A, 0x04, 0x05, 0x02, 0xA6, 0x01},
  {0x0A, 0x04, 0x06, 0x02, 0xA6, 0x01},
  {0x0A, 0x04, 0x07, 0x02, 0xA6, 0x01},
  {0x0A, 0x04, 0x05, 0xFF, 0xA6, 0x01},
  {0x0A, 0x04, 0x07, 0xFF, 0xA6, 0x01}
};
//  R Unit: 03 04 44 81 82 01
// BB Unit: 03 04 44 81 82 02
//    Blue: 03 04 44 81 8A 03
//    Gray: 03 04 44 81 82 04
//     Red: 03 04 44 81 92 05
//  Orange: 03 04 44 81 8A 06
//  Purple: 03 04 44 81 82 07
//   Black: 03 04 44 81 92 08
//   CB-23: 03 04 44 81 82 09
//  Yellow: 03 04 44 81 8A 0A
//  C1-10P: 03 04 44 81 8A 0B
//     D-O: 03 04 44 81 8A 0C
//  Blue 2: 03 04 44 81 82 0D
// BD unit: 03 04 44 81 8A 0E
uint8_t droidList[][14] = {
  {0x03, 0x04, 0x44 , 0x81, 0x82, 0x01},
  {0x03, 0x04, 0x44 , 0x81, 0x82, 0x02},
  {0x03, 0x04, 0x44 , 0x81, 0x8A, 0x03},
  {0x03, 0x04, 0x44 , 0x81, 0x82, 0x04},
  {0x03, 0x04, 0x44 , 0x81, 0x92, 0x05},
  {0x03, 0x04, 0x44 , 0x81, 0x8A, 0x06},
  {0x03, 0x04, 0x44 , 0x81, 0x82, 0x07},
  {0x03, 0x04, 0x44 , 0x81, 0x92, 0x08},
  {0x03, 0x04, 0x44 , 0x81, 0x82, 0x09},
  {0x03, 0x04, 0x44 , 0x81, 0x8A, 0x0A},
  {0x03, 0x04, 0x44 , 0x81, 0x8A, 0x0B},
  {0x03, 0x04, 0x44 , 0x81, 0x8A, 0x0C},
  {0x03, 0x04, 0x44 , 0x81, 0x82, 0x0D},
  {0x03, 0x04, 0x44 , 0x81, 0x8A, 0x0E}
};


class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    // Check if the device has the desired manufacturer data
    std::string payload = advertisedDevice.getManufacturerData();
    if (payload.length() == 8) {
      uint16_t company_id = payload[0] | (payload[1] << 8);// Extract the company ID from the payload
      Serial.println(company_id);
      if (company_id == 0x0183) {// Check if the company ID matches the desired value
        Serial.println("Found 0183");
        for (int i = 0; i < 9; i++) {// Check if the device is in the landList
        if (memcmp(&payload[2], landList[i], 6) == 0) {
          Serial.print("Found device: ");
          digitalWrite(Sound, HIGH);
          delay(300);
          digitalWrite(Sound, LOW);
          Serial.println(advertisedDevice.toString().c_str());// Print the device information
          delay(60000);// Wait for 1 minute before continuing
        }
        }
        for (int i = 0; i < 14; i++) { // Check if the device is in the droidList
        if (memcmp(&payload[2], droidList[i], 6) == 0) {
          Serial.print("Found device: ");
          digitalWrite(Sound, HIGH);
          delay(300);
          digitalWrite(Sound, LOW);
          Serial.println(advertisedDevice.toString().c_str()); // Print the device information
          delay(60000);
        }
      }
    }
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");
  pinMode(Sound,OUTPUT);

  // Initialize BLE  
  BLEDevice::init("");

  // Create a new BLE scan object and set its callbacks
  pBLEScan = BLEDevice::getScan(); // Create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());

  // Set the scan parameters
  pBLEScan->setActiveScan(true); // Active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // Less or equal setInterval value

  // Initialize BLE advertising
  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();
  pAdvertising = BLEDevice::getAdvertising();
  
  // Set the advertising data
  BLEAdvertisementData advertisementData = BLEAdvertisementData();
  advertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED
  std::string manufacturerData = std::string("\x83\x01\x03\x04\x44\x81\x8A\x03", 8);
  advertisementData.setManufacturerData(manufacturerData);

// Set the advertising data and start advertising  
  pAdvertising->setAdvertisementData(advertisementData);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x06);  // sets minimum values for power and interval
  pAdvertising->setMaxPreferred(0x12);
  pAdvertising->start();

}

void loop() {
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults(); // Delete results from BLEScan buffer to release memory
  delay(2000);
}