import serial
import threading
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.axes import Axes
from mpl_toolkits.axes_grid1.inset_locator import inset_axes
from gpiozero import Servo, PWMOutputDevice, LED

# Pines GPIO
SERVO_PIN = 13
MOTOR_PIN = 18
LED_PIN = 17  # Asegúrate que esté conectado con resistencia pull-up

# PWM Rango ESP32 y RPi
ESP_PWM_MIN = 270
ESP_PWM_MAX = 390

# Inicializar actuadores
servo = Servo(SERVO_PIN, min_pulse_width=0.0005, max_pulse_width=0.0025)
motor = PWMOutputDevice(MOTOR_PIN)
led = LED(LED_PIN)

# Datos para gráfica
temperature_data = []
time_data = []
start_time = time.time()
data_lock = threading.Lock()

# Estado actual
current_angle = 0
current_pwm = 0

# Serial
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

def map_pwm_to_duty(pwm_val):
    return max(0.0, min(1.0, (pwm_val - ESP_PWM_MIN) / (ESP_PWM_MAX - ESP_PWM_MIN)))

def blink_led():
    led.on()
    threading.Timer(0.2, led.off).start()

def update_plot(frame):
    with data_lock:
        ax.clear()
        ax.plot(time_data, temperature_data, label='Temperatura (°C)', color='red')
        ax.set_title("Temperatura en Tiempo Real")
        ax.set_xlabel("Tiempo (s)")
        ax.set_ylabel("Temperatura (°C)")
        ax.grid(True)
        ax.legend(loc='upper left')

        # Subgráfico tipo label visual grande
        ax_inset.clear()
        ax_inset.axis("off")
        label_text = f"Ángulo Servo: {current_angle}°\nPWM Motor: {current_pwm}"
        ax_inset.text(0.5, 0.5, label_text,
                      ha='center', va='center', fontsize=16, weight='bold',
                      bbox=dict(facecolor='white', alpha=0.8))

def serial_reader():
    global temperature_data, time_data, current_angle, current_pwm
    collecting_bulk = False
    bulk_buffer = []

    while True:
        line = ser.readline().decode('utf-8', errors='ignore').strip()

        if line.startswith("I (") or "MAIN_DEBUGG_TAG" in line:
            continue

        if line.startswith("TEMP_DATA_START"):
            collecting_bulk = True
            bulk_buffer.clear()
            continue
        elif line.startswith("TEMP_DATA_END"):
            with data_lock:
                timestamp = time.time() - start_time
                for temp in bulk_buffer:
                    temperature_data.append(temp)
                    time_data.append(timestamp)
                    timestamp += 0.1
                    blink_led()
                if len(temperature_data) > 300:
                    temperature_data = temperature_data[-300:]
                    time_data = time_data[-300:]
            collecting_bulk = False
            continue

        if collecting_bulk:
            try:
                bulk_buffer.append(int(line))
            except ValueError:
                continue
            continue

        if line.startswith("(") and line.endswith(")"):
            try:
                t, ang, pwm = map(int, line.strip("()").split(","))
                servo.value = max(-1.0, min(1.0, ang / 90.0))
                motor.value = map_pwm_to_duty(pwm)
                current_angle = ang
                current_pwm = pwm
                with data_lock:
                    timestamp = time.time() - start_time
                    temperature_data.append(t)
                    time_data.append(timestamp)
                    if len(temperature_data) > 300:
                        temperature_data = temperature_data[-300:]
                        time_data = time_data[-300:]
                blink_led()
                print(f"[Realtime] T={t}°C | Servo={ang}° | PWM={pwm}")
            except Exception as e:
                print("Formato inválido:", line)
        else:
            try:
                t = int(line)
                with data_lock:
                    timestamp = time.time() - start_time
                    temperature_data.append(t)
                    time_data.append(timestamp)
                    if len(temperature_data) > 300:
                        temperature_data = temperature_data[-300:]
                        time_data = time_data[-300:]
                blink_led()
            except ValueError:
                pass

# Hilo de lectura
threading.Thread(target=serial_reader, daemon=True).start()

# Gráfica principal
fig, ax = plt.subplots()
ax_inset = inset_axes(ax, width="30%", height="18%", loc='upper center', borderpad=2)

ani = animation.FuncAnimation(fig, update_plot, interval=1000)
plt.tight_layout()
plt.show()
