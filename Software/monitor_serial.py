#!/usr/bin/env python3
import serial
import time
import sys

port = '/dev/cu.usbmodem2101'
baud = 115200

print(f"等待串口 {port} 打开... (按 Ctrl+C 退出)")
time.sleep(1)

try:
    ser = serial.Serial(port, baud, timeout=1)
    print(f"=== 串口已打开，等待设备启动日志 ===")
    print("=" * 50)
    
    while True:
        if ser.in_waiting:
            line = ser.readline().decode('utf-8', errors='ignore').rstrip()
            if line:
                print(line)
        time.sleep(0.1)
except KeyboardInterrupt:
    print("\n监控结束")
except Exception as e:
    print(f"错误：{e}")
finally:
    if 'ser' in locals():
        ser.close()
