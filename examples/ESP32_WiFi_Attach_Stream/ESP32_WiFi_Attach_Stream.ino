/*
 * ESP32 WiFi Stream Attachment Example
 *
 * This example demonstrates how to send an email with a Stream attachment.
 * Stream attachments allow you to send data from any Stream source without
 * needing to save it to a filesystem first.
 *
 * Use cases:
 * - Sensor data in memory
 * - Generated CSV data
 * - Serial buffer contents
 * - HTTP response data
 * - Custom data streams
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

// Custom Stream class for demonstration
// This simulates sensor data or generated content
class DataStream : public Stream {
private:
  String data;
  size_t position;

public:
  DataStream(const String& content) : data(content), position(0) {}

  // Required Stream methods
  int available() override {
    return data.length() - position;
  }

  int read() override {
    if (position < data.length()) {
      return data.charAt(position++);
    }
    return -1;
  }

  int peek() override {
    if (position < data.length()) {
      return data.charAt(position);
    }
    return -1;
  }

  // Required Print methods (not used for reading but needed for Stream)
  size_t write(uint8_t c) override {
    return 0; // Not implemented for this read-only stream
  }
};

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n\nESP32 - Email with Stream Attachment Example");
  Serial.println("=============================================\n");

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

  // Example 1: Send CSV data as Stream attachment
  sendEmailWithCSVStream();

  delay(5000);

  // Example 2: Send JSON data as Stream attachment
  sendEmailWithJSONStream();

  delay(5000);

  // Example 3: Send text data with Base64 encoding
  sendEmailWithBase64Stream();
}

void loop() {
  // Nothing here
}

// Example 1: CSV data from "sensors"
void sendEmailWithCSVStream() {
  Serial.println("=== Example 1: CSV Stream Attachment ===");

  // Generate CSV data (simulating sensor readings)
  String csvData = "Timestamp,Temperature,Humidity,Pressure\n";
  csvData += "2025-10-09 10:00:00,22.5,65.2,1013.25\n";
  csvData += "2025-10-09 10:05:00,22.7,64.8,1013.30\n";
  csvData += "2025-10-09 10:10:00,23.1,64.5,1013.35\n";
  csvData += "2025-10-09 10:15:00,23.4,64.1,1013.40\n";

  Serial.println("Generated CSV data:");
  Serial.println(csvData);
  Serial.println();

  // Create a DataStream from the CSV string
  DataStream csvStream(csvData);

  // Prepare email message
  EMailSender::EMailMessage message;
  message.subject = "Sensor Data CSV Report";
  message.message = "<h2>Sensor Data Report</h2><p>Please find attached the sensor readings in CSV format.</p>";
  message.mime = MIME_TEXT_HTML;

  // Create attachment descriptor for Stream
  EMailSender::FileDescriptior fileDescriptor;
  fileDescriptor.storageType = EMailSender::EMAIL_STORAGE_TYPE_STREAM;
  fileDescriptor.filename = "sensor_data.csv";
  fileDescriptor.mime = "text/csv";
  fileDescriptor.encode64 = false;  // Plain text, no encoding needed
  fileDescriptor.stream = &csvStream;
  fileDescriptor.streamSize = csvData.length();

  // Create attachments array
  EMailSender::FileDescriptior fileDescriptors[1] = { fileDescriptor };
  EMailSender::Attachments attachments = { 1, fileDescriptors };

  // Send email
  Serial.println("Sending email with CSV stream attachment...");
  EMailSender::Response resp = emailSend.send("recipient@example.com", message, attachments);

  // Check result
  Serial.println("Status: " + resp.desc);
  Serial.println("Code: " + resp.code);
  if (resp.status) {
    Serial.println("✓ Email sent successfully!\n");
  } else {
    Serial.println("✗ Error sending email!\n");
  }
}

// Example 2: JSON data from "API"
void sendEmailWithJSONStream() {
  Serial.println("=== Example 2: JSON Stream Attachment ===");

  // Generate JSON data (simulating API response)
  String jsonData = "{\n";
  jsonData += "  \"device\": \"ESP32-01\",\n";
  jsonData += "  \"location\": \"Office\",\n";
  jsonData += "  \"readings\": [\n";
  jsonData += "    {\"sensor\": \"temperature\", \"value\": 22.5, \"unit\": \"°C\"},\n";
  jsonData += "    {\"sensor\": \"humidity\", \"value\": 65.2, \"unit\": \"%\"},\n";
  jsonData += "    {\"sensor\": \"pressure\", \"value\": 1013.25, \"unit\": \"hPa\"}\n";
  jsonData += "  ],\n";
  jsonData += "  \"timestamp\": \"2025-10-09T10:00:00Z\"\n";
  jsonData += "}\n";

  Serial.println("Generated JSON data:");
  Serial.println(jsonData);
  Serial.println();

  // Create a DataStream from the JSON string
  DataStream jsonStream(jsonData);

  // Prepare email message
  EMailSender::EMailMessage message;
  message.subject = "Device Status JSON Report";
  message.message = "<h2>Device Status</h2><p>Attached is the latest device status in JSON format.</p>";
  message.mime = MIME_TEXT_HTML;

  // Create attachment descriptor for Stream
  EMailSender::FileDescriptior fileDescriptor;
  fileDescriptor.storageType = EMailSender::EMAIL_STORAGE_TYPE_STREAM;
  fileDescriptor.filename = "device_status.json";
  fileDescriptor.mime = "application/json";
  fileDescriptor.encode64 = false;
  fileDescriptor.stream = &jsonStream;
  fileDescriptor.streamSize = jsonData.length();

  // Create attachments array
  EMailSender::FileDescriptior fileDescriptors[1] = { fileDescriptor };
  EMailSender::Attachments attachments = { 1, fileDescriptors };

  // Send email
  Serial.println("Sending email with JSON stream attachment...");
  EMailSender::Response resp = emailSend.send("recipient@example.com", message, attachments);

  // Check result
  Serial.println("Status: " + resp.desc);
  Serial.println("Code: " + resp.code);
  if (resp.status) {
    Serial.println("✓ Email sent successfully!\n");
  } else {
    Serial.println("✗ Error sending email!\n");
  }
}

// Example 3: Binary data with Base64 encoding
void sendEmailWithBase64Stream() {
  Serial.println("=== Example 3: Base64 Encoded Stream ===");

  // Generate some binary-like data
  String binaryData = "This could be binary data or special characters: ";
  binaryData += String((char)0x01) + String((char)0x02) + String((char)0x03);
  binaryData += "\nSpecial chars: €£¥©®™\n";

  Serial.println("Generated data (will be Base64 encoded)");
  Serial.println();

  // Create a DataStream
  DataStream dataStream(binaryData);

  // Prepare email message
  EMailSender::EMailMessage message;
  message.subject = "Binary Data with Base64 Encoding";
  message.message = "<h2>Encoded Data</h2><p>The attachment contains binary data encoded in Base64.</p>";
  message.mime = MIME_TEXT_HTML;

  // Create attachment descriptor with Base64 encoding
  EMailSender::FileDescriptior fileDescriptor;
  fileDescriptor.storageType = EMailSender::EMAIL_STORAGE_TYPE_STREAM;
  fileDescriptor.filename = "encoded_data.bin";
  fileDescriptor.mime = "application/octet-stream";
  fileDescriptor.encode64 = true;  // Enable Base64 encoding
  fileDescriptor.stream = &dataStream;
  fileDescriptor.streamSize = binaryData.length();

  // Create attachments array
  EMailSender::FileDescriptior fileDescriptors[1] = { fileDescriptor };
  EMailSender::Attachments attachments = { 1, fileDescriptors };

  // Send email
  Serial.println("Sending email with Base64 encoded stream...");
  EMailSender::Response resp = emailSend.send("recipient@example.com", message, attachments);

  // Check result
  Serial.println("Status: " + resp.desc);
  Serial.println("Code: " + resp.code);
  if (resp.status) {
    Serial.println("✓ Email sent successfully!\n");
  } else {
    Serial.println("✗ Error sending email!\n");
  }
}

