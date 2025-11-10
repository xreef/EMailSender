/*
 * ESP32 WiFi String Attachment - Simple Example
 *
 * This example demonstrates how to send an email with a String
 * attached directly as a file, without needing Stream or file system.
 *
 * Perfect for:
 * - Sending JSON data generated in memory
 * - Creating CSV reports from sensor data
 * - Attaching plain text logs
 * - Any text content created dynamically
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

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n\nESP32 - Simple String Attachment Example");
  Serial.println("=========================================\n");

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

  // Example 1: Simple JSON string attachment
  sendJsonExample();

  delay(3000);

  // Example 2: Simple CSV string attachment
  sendCsvExample();

  delay(3000);

  // Example 3: Simple text log attachment
  sendTextExample();

  Serial.println("\n✓ All examples completed!");
}

void loop() {
  // Nothing here
}

// Example 1: Attach a JSON string directly
void sendJsonExample() {
  Serial.println("=== Example 1: JSON String ===");

  // Create JSON content as a simple String
  String jsonData = "{";
  jsonData += "\"device\":\"ESP32\",";
  jsonData += "\"temperature\":22.5,";
  jsonData += "\"humidity\":65.3,";
  jsonData += "\"timestamp\":" + String(millis());
  jsonData += "}";

  Serial.println("JSON content:");
  Serial.println(jsonData);
  Serial.println();

  // Prepare email message
  EMailSender::EMailMessage message;
  message.subject = "Sensor Data (JSON)";
  message.message = "Current sensor readings attached as JSON file.";

  // Create attachment with String content
  EMailSender::FileDescriptior fileDescriptor;
  fileDescriptor.storageType = EMailSender::EMAIL_STORAGE_TYPE_STRING;
  fileDescriptor.filename = "data.json";
  fileDescriptor.mime = "application/json";
  fileDescriptor.encode64 = false;
  fileDescriptor.content = jsonData;  // ← Just assign the String!

  EMailSender::FileDescriptior fileDescriptors[1] = { fileDescriptor };
  EMailSender::Attachments attachments = { 1, fileDescriptors };

  // Send email
  Serial.println("Sending email...");
  EMailSender::Response resp = emailSend.send("recipient@example.com", message, attachments);

  Serial.println("Status: " + resp.desc);
  Serial.println(resp.status ? "✓ Success\n" : "✗ Failed\n");
}

// Example 2: Attach a CSV string directly
void sendCsvExample() {
  Serial.println("=== Example 2: CSV String ===");

  // Create CSV content as a simple String
  String csvData = "Time,Temperature,Humidity\n";
  for (int i = 0; i < 5; i++) {
    csvData += String(millis()) + ",";
    csvData += String(22.0 + random(-50, 50) / 10.0) + ",";
    csvData += String(60.0 + random(-50, 50) / 10.0) + "\n";
    delay(100);
  }

  Serial.println("CSV content:");
  Serial.println(csvData);
  Serial.println();

  // Prepare email message
  EMailSender::EMailMessage message;
  message.subject = "Sensor Data (CSV)";
  message.message = "5 sensor readings attached as CSV file.";

  // Create attachment with String content
  EMailSender::FileDescriptior fileDescriptor;
  fileDescriptor.storageType = EMailSender::EMAIL_STORAGE_TYPE_STRING;
  fileDescriptor.filename = "data.csv";
  fileDescriptor.mime = "text/csv";
  fileDescriptor.encode64 = false;
  fileDescriptor.content = csvData;  // ← Just assign the String!

  EMailSender::FileDescriptior fileDescriptors[1] = { fileDescriptor };
  EMailSender::Attachments attachments = { 1, fileDescriptors };

  // Send email
  Serial.println("Sending email...");
  EMailSender::Response resp = emailSend.send("recipient@example.com", message, attachments);

  Serial.println("Status: " + resp.desc);
  Serial.println(resp.status ? "✓ Success\n" : "✗ Failed\n");
}

// Example 3: Attach a text log string directly
void sendTextExample() {
  Serial.println("=== Example 3: Text Log String ===");

  // Create text log as a simple String
  String logData = "=== Device Log ===\n\n";
  logData += "Device: ESP32\n";
  logData += "Uptime: " + String(millis() / 1000) + " seconds\n";
  logData += "Free Heap: " + String(ESP.getFreeHeap()) + " bytes\n";
  logData += "WiFi RSSI: " + String(WiFi.RSSI()) + " dBm\n";
  logData += "\n=== End of Log ===\n";

  Serial.println("Log content:");
  Serial.println(logData);
  Serial.println();

  // Prepare email message
  EMailSender::EMailMessage message;
  message.subject = "Device Log";
  message.message = "System log attached as text file.";

  // Create attachment with String content
  EMailSender::FileDescriptior fileDescriptor;
  fileDescriptor.storageType = EMailSender::EMAIL_STORAGE_TYPE_STRING;
  fileDescriptor.filename = "log.txt";
  fileDescriptor.mime = "text/plain";
  fileDescriptor.encode64 = false;
  fileDescriptor.content = logData;  // ← Just assign the String!

  EMailSender::FileDescriptior fileDescriptors[1] = { fileDescriptor };
  EMailSender::Attachments attachments = { 1, fileDescriptors };

  // Send email
  Serial.println("Sending email...");
  EMailSender::Response resp = emailSend.send("recipient@example.com", message, attachments);

  Serial.println("Status: " + resp.desc);
  Serial.println(resp.status ? "✓ Success\n" : "✗ Failed\n");
}

