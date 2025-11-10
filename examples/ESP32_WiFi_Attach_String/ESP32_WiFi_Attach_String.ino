/*
 * ESP32 WiFi String Attachment Example
 *
 * This example demonstrates how to send an email with attachments
 * created from String objects with custom MIME types.
 *
 * Perfect for:
 * - Sending JSON data without ArduinoJson library
 * - Creating CSV reports from sensor data
 * - Generating XML configuration files
 * - Attaching plain text logs
 * - Creating custom file formats
 *
 * NO NEED for Stream wrappers - just pass the String directly!
 *
 * Required Libraries:
 * - EMailSender (this library)
 *
 * Author: Renzo Mischianti
 * Website: www.mischianti.org
 */

#include <WiFi.h>
#include <EMailSender.h>

// WiFi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Gmail SMTP configuration
// IMPORTANT: Use App Password for Gmail (not your regular password)
// Generate at: https://myaccount.google.com/apppasswords
EMailSender emailSend("your.email@gmail.com",
                      "your_app_password",
                      "your.email@gmail.com",
                      "ESP32 Sender",
                      "smtp.gmail.com",
                      587);  // Port 587 for STARTTLS

// Simulate sensor readings
struct SensorData {
  float temperature;
  float humidity;
  int lightLevel;
  unsigned long timestamp;
};

SensorData readSensors() {
  SensorData data;
  data.temperature = 22.5 + (random(-100, 100) / 100.0);
  data.humidity = 65.0 + (random(-100, 100) / 100.0);
  data.lightLevel = random(0, 1024);
  data.timestamp = millis();
  return data;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n\nESP32 - Email with String Attachments");
  Serial.println("======================================\n");

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✓ WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Example 1: JSON attachment from String
  sendJsonAttachment();

  delay(5000);

  // Example 2: CSV attachment from String
  sendCsvAttachment();

  delay(5000);

  // Example 3: XML attachment from String
  sendXmlAttachment();

  delay(5000);

  // Example 4: Plain text log attachment
  sendTextLogAttachment();

  delay(5000);

  // Example 5: Multiple String attachments with different MIME types
  sendMultipleStringAttachments();
}

void loop() {
  // Nothing here
}

// Example 1: JSON attachment from String
void sendJsonAttachment() {
  Serial.println("=== Example 1: JSON String Attachment ===");

  // Read sensors
  SensorData data = readSensors();

  // Create JSON manually (no library needed)
  String jsonContent = "{\n";
  jsonContent += "  \"device_id\": \"ESP32-SENSOR-01\",\n";
  jsonContent += "  \"timestamp\": " + String(data.timestamp) + ",\n";
  jsonContent += "  \"temperature\": " + String(data.temperature, 2) + ",\n";
  jsonContent += "  \"humidity\": " + String(data.humidity, 2) + ",\n";
  jsonContent += "  \"light_level\": " + String(data.lightLevel) + ",\n";
  jsonContent += "  \"wifi_rssi\": " + String(WiFi.RSSI()) + "\n";
  jsonContent += "}";

  Serial.println("Generated JSON:");
  Serial.println(jsonContent);
  Serial.println();

  // Prepare email message
  EMailSender::EMailMessage message;
  message.subject = "Sensor Data JSON Report";
  message.message = "<h2>Sensor Data Available</h2>"
                    "<p>Current sensor readings have been collected and attached as JSON file.</p>"
                    "<p><strong>Quick Summary:</strong><br>"
                    "Temperature: " + String(data.temperature, 1) + "°C<br>"
                    "Humidity: " + String(data.humidity, 1) + "%</p>";
  message.mime = MIME_TEXT_HTML;

  // Create attachment - Just pass the String directly!
  EMailSender::FileDescriptior fileDescriptor;
  fileDescriptor.storageType = EMailSender::EMAIL_STORAGE_TYPE_STRING;
  fileDescriptor.filename = "sensor_data.json";
  fileDescriptor.mime = "application/json";  // JSON MIME type
  fileDescriptor.encode64 = false;
  fileDescriptor.content = jsonContent;  // ← Simply assign the String!

  EMailSender::FileDescriptior fileDescriptors[1] = { fileDescriptor };
  EMailSender::Attachments attachments = { 1, fileDescriptors };

  // Send email
  Serial.println("Sending email...");
  EMailSender::Response resp = emailSend.send("recipient@example.com", message, attachments);

  Serial.println("Status: " + resp.desc);
  if (resp.status) {
    Serial.println("✓ Email sent successfully!\n");
  } else {
    Serial.println("✗ Error: " + resp.desc + "\n");
  }
}

// Example 2: CSV attachment from String
void sendCsvAttachment() {
  Serial.println("=== Example 2: CSV String Attachment ===");

  // Create CSV content
  String csvContent = "Timestamp,Temperature (°C),Humidity (%),Light Level\n";

  // Generate 10 rows of data
  for (int i = 0; i < 10; i++) {
    SensorData data = readSensors();
    csvContent += String(data.timestamp) + ",";
    csvContent += String(data.temperature, 2) + ",";
    csvContent += String(data.humidity, 2) + ",";
    csvContent += String(data.lightLevel) + "\n";
    delay(100); // Small delay between readings
  }

  Serial.println("Generated CSV:");
  Serial.println(csvContent.substring(0, 200)); // Print first 200 chars
  Serial.println("... (truncated)\n");

  // Prepare email
  EMailSender::EMailMessage message;
  message.subject = "Sensor Data CSV Export";
  message.message = "<h2>Sensor Data Export</h2>"
                    "<p>10 sensor readings have been exported to CSV format.</p>"
                    "<p>You can open the attached file with Excel or any spreadsheet application.</p>";
  message.mime = MIME_TEXT_HTML;

  // Create attachment - Just pass the String directly!
  EMailSender::FileDescriptior fileDescriptor;
  fileDescriptor.storageType = EMailSender::EMAIL_STORAGE_TYPE_STRING;
  fileDescriptor.filename = "sensor_data.csv";
  fileDescriptor.mime = "text/csv";  // CSV MIME type
  fileDescriptor.encode64 = false;
  fileDescriptor.content = csvContent;  // ← Simply assign the String!

  EMailSender::FileDescriptior fileDescriptors[1] = { fileDescriptor };
  EMailSender::Attachments attachments = { 1, fileDescriptors };

  // Send email
  Serial.println("Sending email...");
  EMailSender::Response resp = emailSend.send("recipient@example.com", message, attachments);

  Serial.println("Status: " + resp.desc);
  if (resp.status) {
    Serial.println("✓ Email sent successfully!\n");
  } else {
    Serial.println("✗ Error: " + resp.desc + "\n");
  }
}

// Example 3: XML attachment from String
void sendXmlAttachment() {
  Serial.println("=== Example 3: XML String Attachment ===");

  // Read sensors
  SensorData data = readSensors();

  // Create XML content
  String xmlContent = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  xmlContent += "<device>\n";
  xmlContent += "  <info>\n";
  xmlContent += "    <id>ESP32-SENSOR-01</id>\n";
  xmlContent += "    <name>Living Room Sensor</name>\n";
  xmlContent += "    <ip>" + WiFi.localIP().toString() + "</ip>\n";
  xmlContent += "  </info>\n";
  xmlContent += "  <readings timestamp=\"" + String(data.timestamp) + "\">\n";
  xmlContent += "    <temperature unit=\"celsius\">" + String(data.temperature, 2) + "</temperature>\n";
  xmlContent += "    <humidity unit=\"percent\">" + String(data.humidity, 2) + "</humidity>\n";
  xmlContent += "    <light unit=\"raw\">" + String(data.lightLevel) + "</light>\n";
  xmlContent += "  </readings>\n";
  xmlContent += "  <network>\n";
  xmlContent += "    <ssid>" + String(WiFi.SSID()) + "</ssid>\n";
  xmlContent += "    <rssi>" + String(WiFi.RSSI()) + "</rssi>\n";
  xmlContent += "  </network>\n";
  xmlContent += "</device>";

  Serial.println("Generated XML:");
  Serial.println(xmlContent);
  Serial.println();

  // Prepare email
  EMailSender::EMailMessage message;
  message.subject = "Device Configuration XML";
  message.message = "<h2>Device Configuration</h2>"
                    "<p>Current device configuration and sensor readings in XML format.</p>"
                    "<p><strong>Device:</strong> ESP32-SENSOR-01<br>"
                    "<strong>IP:</strong> " + WiFi.localIP().toString() + "</p>";
  message.mime = MIME_TEXT_HTML;

  // Create attachment - Just pass the String directly!
  EMailSender::FileDescriptior fileDescriptor;
  fileDescriptor.storageType = EMailSender::EMAIL_STORAGE_TYPE_STRING;
  fileDescriptor.filename = "device_config.xml";
  fileDescriptor.mime = "application/xml";  // XML MIME type
  fileDescriptor.encode64 = false;
  fileDescriptor.content = xmlContent;  // ← Simply assign the String!

  EMailSender::FileDescriptior fileDescriptors[1] = { fileDescriptor };
  EMailSender::Attachments attachments = { 1, fileDescriptors };

  // Send email
  Serial.println("Sending email...");
  EMailSender::Response resp = emailSend.send("recipient@example.com", message, attachments);

  Serial.println("Status: " + resp.desc);
  if (resp.status) {
    Serial.println("✓ Email sent successfully!\n");
  } else {
    Serial.println("✗ Error: " + resp.desc + "\n");
  }
}

// Example 4: Plain text log attachment
void sendTextLogAttachment() {
  Serial.println("=== Example 4: Text Log Attachment ===");

  // Create log content
  String logContent = "=== ESP32 Device Log ===\n\n";
  logContent += "Device ID: ESP32-SENSOR-01\n";
  logContent += "Firmware Version: 1.0.0\n";
  logContent += "Uptime: " + String(millis() / 1000) + " seconds\n";
  logContent += "IP Address: " + WiFi.localIP().toString() + "\n";
  logContent += "WiFi SSID: " + String(WiFi.SSID()) + "\n";
  logContent += "WiFi RSSI: " + String(WiFi.RSSI()) + " dBm\n\n";

  logContent += "=== System Information ===\n";
  logContent += "Free Heap: " + String(ESP.getFreeHeap()) + " bytes\n";
  logContent += "Heap Size: " + String(ESP.getHeapSize()) + " bytes\n";
  logContent += "CPU Frequency: " + String(ESP.getCpuFreqMHz()) + " MHz\n";
  logContent += "Chip Model: " + String(ESP.getChipModel()) + "\n\n";

  logContent += "=== Recent Sensor Readings ===\n";
  for (int i = 0; i < 5; i++) {
    SensorData data = readSensors();
    logContent += "[" + String(data.timestamp) + "] ";
    logContent += "Temp: " + String(data.temperature, 1) + "°C, ";
    logContent += "Hum: " + String(data.humidity, 1) + "%, ";
    logContent += "Light: " + String(data.lightLevel) + "\n";
    delay(100);
  }

  logContent += "\n=== End of Log ===\n";

  Serial.println("Generated log:");
  Serial.println(logContent.substring(0, 300)); // Print first 300 chars
  Serial.println("... (truncated)\n");

  // Prepare email
  EMailSender::EMailMessage message;
  message.subject = "Device Log Report - " + String(WiFi.localIP().toString());
  message.message = "<h2>Device Log Report</h2>"
                    "<p>Complete device log with system information and recent sensor readings.</p>"
                    "<p><strong>Uptime:</strong> " + String(millis() / 1000) + " seconds</p>";
  message.mime = MIME_TEXT_HTML;

  // Create attachment - Just pass the String directly!
  EMailSender::FileDescriptior fileDescriptor;
  fileDescriptor.storageType = EMailSender::EMAIL_STORAGE_TYPE_STRING;
  fileDescriptor.filename = "device_log.txt";
  fileDescriptor.mime = "text/plain";  // Plain text MIME type
  fileDescriptor.encode64 = false;
  fileDescriptor.content = logContent;  // ← Simply assign the String!

  EMailSender::FileDescriptior fileDescriptors[1] = { fileDescriptor };
  EMailSender::Attachments attachments = { 1, fileDescriptors };

  // Send email
  Serial.println("Sending email...");
  EMailSender::Response resp = emailSend.send("recipient@example.com", message, attachments);

  Serial.println("Status: " + resp.desc);
  if (resp.status) {
    Serial.println("✓ Email sent successfully!\n");
  } else {
    Serial.println("✗ Error: " + resp.desc + "\n");
  }
}

// Example 5: Multiple String attachments with different MIME types
void sendMultipleStringAttachments() {
  Serial.println("=== Example 5: Multiple String Attachments ===");

  // Read sensors once
  SensorData data = readSensors();

  // 1. JSON content
  String jsonContent = "{\"device\":\"ESP32\",\"temp\":" + String(data.temperature) + "}";

  // 2. CSV content
  String csvContent = "Sensor,Value\n";
  csvContent += "Temperature," + String(data.temperature) + "\n";
  csvContent += "Humidity," + String(data.humidity) + "\n";

  // 3. Text content
  String txtContent = "Device: ESP32-SENSOR-01\n";
  txtContent += "Temperature: " + String(data.temperature) + "°C\n";
  txtContent += "Humidity: " + String(data.humidity) + "%\n";

  Serial.println("Created 3 attachments: JSON, CSV, TXT\n");

  // Prepare email
  EMailSender::EMailMessage message;
  message.subject = "Multi-Format Sensor Report";
  message.message = "<h2>Sensor Data in Multiple Formats</h2>"
                    "<p>The same sensor data is attached in three different formats:</p>"
                    "<ul>"
                    "<li><strong>data.json</strong> - JSON format</li>"
                    "<li><strong>data.csv</strong> - CSV format</li>"
                    "<li><strong>data.txt</strong> - Plain text format</li>"
                    "</ul>";
  message.mime = MIME_TEXT_HTML;

  // Create 3 attachments - All using String directly!
  EMailSender::FileDescriptior fileDescriptor1;
  fileDescriptor1.storageType = EMailSender::EMAIL_STORAGE_TYPE_STRING;
  fileDescriptor1.filename = "data.json";
  fileDescriptor1.mime = "application/json";
  fileDescriptor1.encode64 = false;
  fileDescriptor1.content = jsonContent;  // ← Simply assign the String!

  EMailSender::FileDescriptior fileDescriptor2;
  fileDescriptor2.storageType = EMailSender::EMAIL_STORAGE_TYPE_STRING;
  fileDescriptor2.filename = "data.csv";
  fileDescriptor2.mime = "text/csv";
  fileDescriptor2.encode64 = false;
  fileDescriptor2.content = csvContent;  // ← Simply assign the String!

  EMailSender::FileDescriptior fileDescriptor3;
  fileDescriptor3.storageType = EMailSender::EMAIL_STORAGE_TYPE_STRING;
  fileDescriptor3.filename = "data.txt";
  fileDescriptor3.mime = "text/plain";
  fileDescriptor3.encode64 = false;
  fileDescriptor3.content = txtContent;  // ← Simply assign the String!

  EMailSender::FileDescriptior fileDescriptors[3] = {
    fileDescriptor1,
    fileDescriptor2,
    fileDescriptor3
  };
  EMailSender::Attachments attachments = { 3, fileDescriptors };

  // Send email
  Serial.println("Sending email with 3 attachments...");
  EMailSender::Response resp = emailSend.send("recipient@example.com", message, attachments);

  Serial.println("Status: " + resp.desc);
  if (resp.status) {
    Serial.println("✓ Email sent successfully!\n");
  } else {
    Serial.println("✗ Error: " + resp.desc + "\n");
  }
}
