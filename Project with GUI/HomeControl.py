import os
import sys
import serial
import threading
from datetime import datetime
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QLabel, QVBoxLayout, QTableWidget, QTableWidgetItem,
    QWidget, QPushButton, QHBoxLayout, QGridLayout, QMessageBox
)
from PyQt5.QtGui import QPixmap, QIcon
from PyQt5.QtCore import Qt

class HomeControlApp(QMainWindow):
    def __init__(self):
        super().__init__()

        # Initialize lamp and plug states
        self.lamp_on = False
        self.plug_on = False

        # GUI Setup
        self.setWindowTitle("Smart Home Control")
        self.setWindowIcon(QIcon("utils/app_icon.png"))
        self.setGeometry(100, 100, 800, 600)

        # Image file paths
        self.lamp_on_image = "utils/lamp_on.jpg"
        self.lamp_off_image = "utils/lamp_off.jpg"
        self.plug_on_image = "utils/plug_on.jpg"
        self.plug_off_image = "utils/plug_off.jpg"
        # self.switch_icon_image = "utils/switch_icon.png"
        self.alarm_on_image = "utils/alarm_on.png"
        self.alarm_off_image = "utils/alarm_off.png"
        # Lamp Controls
        self.lamp_label = QLabel(self)
        self.lamp_label.setPixmap(QPixmap(self.lamp_off_image).scaled(100, 100, Qt.KeepAspectRatio))
        self.lamp_label.setAlignment(Qt.AlignCenter)

        # Lamp Switch Button
        self.lamp_switch_button = QPushButton("Turn Lamp ON", self)
        # self.lamp_switch_button.setIcon(QIcon(self.switch_icon_image))
        self.lamp_switch_button.clicked.connect(self.toggle_lamp)  # Connect to toggle_lamp

        # Plug Controls
        self.plug_label = QLabel(self)
        self.plug_label.setPixmap(QPixmap(self.plug_off_image).scaled(100, 100, Qt.KeepAspectRatio))
        self.plug_label.setAlignment(Qt.AlignCenter)

        # Plug "On" Button
        self.plug_on_button = QPushButton("On", self)
        # self.plug_on_button.setIcon(QIcon(self.switch_icon_image))
        self.plug_on_button.clicked.connect(self.turn_plug_on)  # Connect to turn_plug_on

        # Plug "Off" Button
        self.plug_off_button = QPushButton("Off", self)
        # self.plug_off_button.setIcon(QIcon(self.switch_icon_image))
        self.plug_off_button.clicked.connect(self.turn_plug_off)  # Connect to turn_plug_off

        # Buzzer Controls
        self.buzzer_label = QLabel("Buzzer Status: OFF", self)
        self.buzzer_label.setStyleSheet("font-size: 16px; color: red;")
        self.buzzer_button = QPushButton("Toggle Buzzer", self)
        self.buzzer_button.clicked.connect(self.toggle_buzzer)

        # Door Status
        self.status_label = QLabel("Waiting for door status...", self)
        self.status_label.setStyleSheet("font-size: 20px;")

        # Alarm Controls
        self.alarm_label = QLabel(self)
        self.alarm_label.setPixmap(QPixmap(self.alarm_off_image).scaled(100, 100, Qt.KeepAspectRatio))
        self.alarm_label.setAlignment(Qt.AlignCenter)

        # Temperature Visualization
        self.temp_label = QLabel("Temperature: -- °C", self)
        self.temp_label.setStyleSheet("font-size: 20px; color: red;")

        # Log Table
        self.log_table = QTableWidget(self)
        self.log_table.setColumnCount(2)
        self.log_table.setHorizontalHeaderLabels(["Real Time of status", "Door Status"])
        self.log_table.setStyleSheet("font-size: 14px;")

        # Layouts
        control_layout = QGridLayout()

        # Lamp Controls
        control_layout.addWidget(self.lamp_label, 0, 0, alignment=Qt.AlignCenter)
        control_layout.addWidget(self.lamp_switch_button, 1, 0, alignment=Qt.AlignCenter)

        # Add to the control_layout
        control_layout.addWidget(self.buzzer_label, 3, 0, alignment=Qt.AlignCenter)  # Buzzer status label
        control_layout.addWidget(self.buzzer_button, 4, 0, alignment=Qt.AlignCenter)  # Buzzer button below the label

        # Plug Controls
        control_layout.addWidget(self.plug_label, 0, 1, alignment=Qt.AlignCenter)
        control_layout.addWidget(self.plug_on_button, 1, 1, alignment=Qt.AlignCenter)
        control_layout.addWidget(self.plug_off_button, 2, 1, alignment=Qt.AlignCenter)

        # Temperature and Alarm Layout
        temp_layout = QVBoxLayout()
        temp_layout.addWidget(self.alarm_label)
        temp_layout.addWidget(self.temp_label)

        # Main Layout
        main_layout = QVBoxLayout()
        main_layout.addLayout(control_layout)
        main_layout.addLayout(temp_layout)
        main_layout.addWidget(self.status_label)
        main_layout.addWidget(self.log_table)

        container = QWidget()
        container.setLayout(main_layout)
        self.setCentralWidget(container)

    # UART Setup
        self.uart_port = serial.Serial("COM3", baudrate=9600, timeout=1)  # Replace with your port
        self.start_uart_listener()

    def start_uart_listener(self):
        """Start a thread to listen for UART data."""
        self.uart_thread = threading.Thread(target=self.uart_listener, daemon=True)
        self.uart_thread.start()

    def uart_listener(self):
        """Read data from UART and update the GUI."""
        while True:
            if self.uart_port.in_waiting > 0:
                data = self.uart_port.readline().decode('utf-8').strip()
                self.handle_uart_data(data)

    def handle_uart_data(self, data):
        """Process UART data received from the TivaC."""
        print(f"Received UART Data: {data}")  # Debug print

        if data.startswith("LAMP"):
            # Update lamp state based on TivaC response
            if "ON" in data:
                self.lamp_on = True
                self.lamp_label.setPixmap(QPixmap(self.lamp_on_image).scaled(100, 100, Qt.KeepAspectRatio))
            elif "OFF" in data:
                self.lamp_on = False
                self.lamp_label.setPixmap(QPixmap(self.lamp_off_image).scaled(100, 100, Qt.KeepAspectRatio))

        elif data.startswith("PLUG"):
            # Update plug state based on TivaC response
            if "ON" in data:
                self.plug_on = True
                self.plug_label.setPixmap(QPixmap(self.plug_on_image).scaled(100, 100, Qt.KeepAspectRatio))
            elif "OFF" in data:
                self.plug_on = False
                self.plug_label.setPixmap(QPixmap(self.plug_off_image).scaled(100, 100, Qt.KeepAspectRatio))

        elif data.startswith("DOOR:"):
            status = data.split(":")[1]
            self.update_door_status(status)
        elif data.startswith("TEMP:"):
            temperature = float(data.split(":")[1])
            self.update_temperature(temperature)
            if temperature > 30:
                self.turn_alarm_on()  # Check alarm state after temperature update
            else:
                self.turn_alarm_off()  # Check alarm state after temperature update

    def turn_alarm_on(self):
        """Turn the alarm ON and update the UI."""
        self.alarm_label.setPixmap(QPixmap(self.alarm_on_image).scaled(100, 100, Qt.KeepAspectRatio))
        self.alarm_on = True

    def turn_alarm_off(self):
        """Turn the alarm OFF and update the UI."""
        self.alarm_label.setPixmap(QPixmap(self.alarm_off_image).scaled(100, 100, Qt.KeepAspectRatio))
        self.alarm_on = False

    def update_door_status(self, status):
        """Update the door status and log the event."""
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        self.status_label.setText(f"Door Status: {status}")

        # Add to log table
        row_position = self.log_table.rowCount()
        self.log_table.insertRow(row_position)
        self.log_table.setItem(row_position, 0, QTableWidgetItem(timestamp))
        self.log_table.setItem(row_position, 1, QTableWidgetItem(status))

    def update_temperature(self, temperature):
        # Update the temperature display
        self.temp_label.setText(f"Temperature: {temperature:.1f} °C")

    def toggle_buzzer(self):
        """Toggle the buzzer state and update the UI."""
        if hasattr(self, 'buzzer_on') and self.buzzer_on:  # Check if the attribute exists and is True
            self.uart_port.write("BUZZER_OFF\n".encode('utf-8'))  # Send command to turn off the buzzer
            self.uart_port.flush()
            self.buzzer_label.setText("Buzzer Status: ON")
            self.buzzer_label.setStyleSheet("font-size: 16px; color: green;")
            self.buzzer_on = False
        else:
            self.uart_port.write("BUZZER_ON\n".encode('utf-8'))  # Send command to turn on the buzzer
            self.uart_port.flush()
            self.buzzer_label.setText("Buzzer Status: OFF")
            self.buzzer_label.setStyleSheet("font-size: 16px; color: red;")
            self.buzzer_on = True

    def toggle_lamp(self):
        """Toggle lamp state and send command over UART."""
        if self.lamp_on:
            self.uart_port.write("LAMP_OFF\n".encode('utf-8'))  # Send command to turn off the lamp
            self.uart_port.flush()  # Ensure the command is completely sent
            self.lamp_label.setPixmap(QPixmap(self.lamp_off_image).scaled(100, 100, Qt.KeepAspectRatio))
            self.lamp_switch_button.setText("Turn Lamp ON")  # Update button text
            self.lamp_on = False
        else:
            self.uart_port.write("LAMP_ON\n".encode('utf-8'))  # Send command to turn on the lamp
            self.uart_port.flush()  # Ensure the command is completely sent
            self.lamp_label.setPixmap(QPixmap(self.lamp_on_image).scaled(100, 100, Qt.KeepAspectRatio))
            self.lamp_switch_button.setText("Turn Lamp OFF")  # Update button text
            self.lamp_on = True

    def turn_plug_on(self):
        """Turn the plug ON and update the UI."""
        self.uart_port.write("PLUG_ON\n".encode('utf-8'))  # Send command to turn on the plug
        self.uart_port.flush()  # Ensure the command is completely sent
        self.plug_label.setPixmap(QPixmap(self.plug_on_image).scaled(100, 100, Qt.KeepAspectRatio))
        self.plug_on = True

    def turn_plug_off(self):
        """Turn the plug OFF and update the UI."""
        self.uart_port.write("PLUG_OFF\n".encode('utf-8'))  # Send command to turn off the plug
        self.uart_port.flush()  # Ensure the command is completely sent
        self.plug_label.setPixmap(QPixmap(self.plug_off_image).scaled(100, 100, Qt.KeepAspectRatio))
        self.plug_on = False

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = HomeControlApp()
    window.show()
    sys.exit(app.exec_())
