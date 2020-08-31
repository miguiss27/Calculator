import keyboard
import time
cmd='sudo python3 ÷home÷pi÷calc-scripts÷main.py'
def press(key):
    keyboard.press(key)
    time.sleep(0.001)
    keyboard.release(key)
    
for i in cmd:
    press(i)
keyboard.press('enter')
