/*----Kernel Includes----*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/timers.h"

/*---- std includes ----*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*---- esp includes ----*/
#include "esp_log.h"
#include "lvgl.h"
#include "esp_lvgl_port.h"
#include "driver/uart.h"

/*---- project includes ----*/
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "ssd1306.h"
#include "touch.h"
#include "interrupt.h"

static const char *TAG = "MAIN_DEBUGG_TAG";
static const char *ADC_TAG = "ADC_CHANNEL_8";
static const char *I2C_TAG = "I2C_SD1306_BUS_CONFIGURATION";
static const char *TAG_TOUCH = "TOUCH_PAD";
static const char *UART_TAG = "UART0_COMM";

// UART Configuration for USB communication to Raspberry Pi
#define UART_PORT_NUM      UART_NUM_0
#define UART_BAUD_RATE     115200
#define UART_DATA_BITS     UART_DATA_8_BITS
#define UART_PARITY        UART_PARITY_DISABLE
#define UART_STOP_BITS     UART_STOP_BITS_1
#define UART_FLOW_CTRL     UART_HW_FLOWCTRL_DISABLE
#define UART_SOURCE_CLK    UART_SCLK_DEFAULT

// ADC Queue Configuration
#define ADC_QUEUE_SIZE     1000
#define TEMP_SEND_INTERVAL 30000  // 30 seconds in ms

static esp_err_t drivers_setup();
static void uart_init_config(void);
static void send_temperature_data(void);
static void timer_callback(TimerHandle_t xTimer);
static void temperature_notify_task(void *pvParameters);
static void send_pwm_update(void);
static TaskHandle_t temp_task_handle = NULL;

// Global variables
int adcRawValue;
int voltage;
int temperature;
int servoAngle = 0;
uint32_t motorSpeed = 300;
bool motorRunning = false;

// Queues and handles
static QueueHandle_t adc_queue = NULL;
static QueueHandle_t que_touch = NULL;
static TimerHandle_t temp_timer = NULL;

// Display handle
lv_disp_t *disp = NULL;

// Motor speed control constants
#define MOTOR_SPEED_STEP    10
#define MOTOR_SPEED_MIN     200
#define MOTOR_SPEED_MAX     390

// Servo angle control constants
#define SERVO_ANGLE_STEP    15
#define SERVO_ANGLE_MIN     -90
#define SERVO_ANGLE_MAX     90

/*---- Touch Pad Configuration ----*/
static uint8_t guard_mode_flag = 0;

typedef struct touch_msg {
    touch_pad_intr_mask_t intr_mask;
    uint32_t pad_num;
    uint32_t pad_status;
    uint32_t pad_val;
} touch_event_t;

// Touch pad mapping
typedef enum {
    SERVO_ZERO_BTN = 0,        // TOUCH_PAD_NUM2 - Reset servo to 0°
    SERVO_NEG_BTN,             // TOUCH_PAD_NUM3 - Servo negative rotation
    SERVO_POS_BTN,             // TOUCH_PAD_NUM4 - Servo positive rotation
    MOTOR_SPEED_UP_BTN,        // TOUCH_PAD_NUM5 - Increase motor speed
    MOTOR_SPEED_DOWN_BTN,      // TOUCH_PAD_NUM6 - Decrease motor speed
    MOTOR_OFF_BTN              // TOUCH_PAD_NUM7 - Turn off motor
} touch_button_index_t;

static const touch_pad_t button[TOUCH_BUTTON_NUM] = {
    TOUCH_PAD_NUM2,     // Servo zero
    TOUCH_PAD_NUM3,     // Servo negative
    TOUCH_PAD_NUM4,     // Servo positive
    TOUCH_PAD_NUM5,     // Motor speed up
    TOUCH_PAD_NUM6,     // Motor speed down
    TOUCH_PAD_NUM7      // Motor off
};

static const float button_threshold[TOUCH_BUTTON_NUM] = {
    0.2, 0.2, 0.2, 0.2, 0.2, 0.2
};

// Function prototypes
static void touchsensor_interrupt_cb(void *arg);
static void touch_pad_set_thresholds(void);
static void touchsensor_filter_set(touch_filter_mode_t mode);
static void touch_pad_read_task(void *pvParameter);
static void touch_pad_init_config(void);
static void adc_read_task(void *pvParameter);
static void display_update_task(void *pvParameter);
static void handle_touch_event(touch_event_t *evt);

void app_main(){
    ESP_ERROR_CHECK(drivers_setup());
    touch_pad_init_config();
    uart_init_config();
    
    ESP_LOGI(TAG, "DRIVERS SETTED UP CORRECTLY");

    // Initialize ADC queue
    adc_queue = xQueueCreate(ADC_QUEUE_SIZE, sizeof(int));
    if (adc_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create ADC queue");
        return;
    }

    // Initialize timer for 30-second temperature data transmission
    temp_timer = xTimerCreate("TempTimer", 
                              pdMS_TO_TICKS(TEMP_SEND_INTERVAL),
                              pdTRUE,  // Auto-reload
                              (void*)0,
                              timer_callback);
    
    if (temp_timer != NULL) {
        xTimerStart(temp_timer, 0);
    }

    lv_disp_set_rotation(disp, LV_DISP_ROT_NONE);
    
    // Create UI
    if (lvgl_port_lock(0)) {
        example_lvgl_demo_ui(disp);
        lvgl_port_unlock();
    }

    // Create tasks
    xTaskCreate(&touch_pad_read_task, "touch_pad_read_task", 4096, NULL, 5, NULL);
    xTaskCreate(&adc_read_task, "adc_read_task", 4096, NULL, 4, NULL);
    xTaskCreate(&display_update_task, "display_update_task", 4096, NULL, 3, NULL);
    xTaskCreate(&temperature_notify_task, "temperature_notify_task", 4096, NULL, 4, &temp_task_handle);


    ESP_LOGI(TAG, "All tasks created successfully");

    // Main loop (can be used for other periodic tasks)
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static esp_err_t drivers_setup(){
    touch_pad_init();
    adc_config_init();
    i2c_master_init();
    ssd1306_init();
    lvgl_init(&disp);
    init_TIMER();
    init_isr();
    init_bdc_Motor();
    
    return ESP_OK;
}

static void uart_init_config(void) {
    const uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_BITS,
        .parity    = UART_PARITY,
        .stop_bits = UART_STOP_BITS,
        .flow_ctrl = UART_FLOW_CTRL,
        .source_clk = UART_SOURCE_CLK,
    };
    
    // Install UART driver
    ESP_ERROR_CHECK(uart_driver_install(UART_PORT_NUM, 1024, 1024, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_PORT_NUM, &uart_config));
    
    ESP_LOGI(UART_TAG, "UART0 configured for USB communication to Raspberry Pi");
}

static void adc_read_task(void *pvParameter) {
    int temp_data;
    
    while(1) {
        if(adc_lecture(&adcRawValue) == ESP_OK) {
            voltage = 3300 * adcRawValue / 4096;
            temperature = voltage / 10;
            
            // Store temperature in queue (FIFO - remove oldest if full)
            if (xQueueSend(adc_queue, &temperature, 0) != pdTRUE) {
                // Queue is full, remove oldest and add new
                xQueueReceive(adc_queue, &temp_data, 0);
                xQueueSend(adc_queue, &temperature, 0);
            }
            
            ESP_LOGI(TAG, "Raw ADC: %d, Voltage: %d mV, Temp: %d ºC", 
                     adcRawValue, voltage, temperature);
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Read every 100ms
    }
}

static void display_update_task(void *pvParameter) {
    while(1) {
        if (lvgl_port_lock(0)) {
            update_adc_label(voltage, temperature);
            lvgl_port_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Update display every 500ms
        if (lvgl_port_lock(0)) {
            update_servo_label(servoAngle);
            lvgl_port_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Update display every 500ms
        if (lvgl_port_lock(0)) {
            update_dcmotor_label(motorSpeed);
            lvgl_port_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Update display every 500ms
    }
}

static void send_pwm_update(void) {
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "(%d,%d,%lu)\n", temperature, servoAngle, motorSpeed);
    uart_write_bytes(UART_PORT_NUM, buffer, strlen(buffer));
}


static void handle_touch_event(touch_event_t *evt) {
    bool updated = false;

    if (evt->intr_mask & TOUCH_PAD_INTR_MASK_ACTIVE) {
        switch(evt->pad_num) {
            case TOUCH_PAD_NUM2: // Servo zero
                servoAngle = 0;
                set_servoAngle(&servoAngle);
                ESP_LOGI(TAG, "Servo reset to 0°");
                updated = true;
                break;

            case TOUCH_PAD_NUM3: // Servo negative rotation
                if (servoAngle > SERVO_ANGLE_MIN) {
                    servoAngle -= SERVO_ANGLE_STEP;
                    if (servoAngle < SERVO_ANGLE_MIN) servoAngle = SERVO_ANGLE_MIN;
                    set_servoAngle(&servoAngle);
                    ESP_LOGI(TAG, "Servo angle: %d°", servoAngle);
                    updated = true;
                }
                break;

            case TOUCH_PAD_NUM4: // Servo positive rotation
                if (servoAngle < SERVO_ANGLE_MAX) {
                    servoAngle += SERVO_ANGLE_STEP;
                    if (servoAngle > SERVO_ANGLE_MAX) servoAngle = SERVO_ANGLE_MAX;
                    set_servoAngle(&servoAngle);
                    ESP_LOGI(TAG, "Servo angle: %d°", servoAngle);
                    updated = true;
                }
                break;

            case TOUCH_PAD_NUM5: // Motor speed up
                if (motorSpeed < MOTOR_SPEED_MAX) {
                    motorSpeed += MOTOR_SPEED_STEP;
                    if (motorSpeed > MOTOR_SPEED_MAX) motorSpeed = MOTOR_SPEED_MAX;
                    motorRunning = true;
                    set_dcmotorSpeed(&motorSpeed);
                    ESP_LOGI(TAG, "Motor speed increased: %lu", motorSpeed);
                    updated = true;
                }
                break;

            case TOUCH_PAD_NUM6: // Motor speed down
                if (motorSpeed > MOTOR_SPEED_MIN) {
                    motorSpeed -= MOTOR_SPEED_STEP;
                    if (motorSpeed < MOTOR_SPEED_MIN) {
                        motorSpeed = MOTOR_SPEED_MIN;
                        motorRunning = false;
                    }
                    set_dcmotorSpeed(&motorSpeed);
                    ESP_LOGI(TAG, "Motor speed decreased: %lu", motorSpeed);
                    updated = true;
                }
                break;

            case TOUCH_PAD_NUM7: // Motor off
                motorSpeed = 270;
                motorRunning = false;
                set_dcmotorSpeed(&motorSpeed);
                ESP_LOGI(TAG, "Motor turned off");
                updated = true;
                break;
        }

        if (updated) {
            send_pwm_update();
        }
    }
}


static void send_temperature_data(void) {
    int temp_data;
    int queue_size = uxQueueMessagesWaiting(adc_queue);
    char uart_buffer[32];
    
    ESP_LOGI(UART_TAG, "Sending %d temperature readings to Raspberry Pi", queue_size);
    
    // Send header
    snprintf(uart_buffer, sizeof(uart_buffer), "TEMP_DATA_START:%d\n", queue_size);
    uart_write_bytes(UART_PORT_NUM, uart_buffer, strlen(uart_buffer));
    
    // Send all temperature data from queue
    while(xQueueReceive(adc_queue, &temp_data, 0) == pdTRUE) {
        snprintf(uart_buffer, sizeof(uart_buffer), "%d\n", temp_data);
        uart_write_bytes(UART_PORT_NUM, uart_buffer, strlen(uart_buffer));
    }
    
    // Send footer
    uart_write_bytes(UART_PORT_NUM, "TEMP_DATA_END\n", 14);
    
    ESP_LOGI(UART_TAG, "Temperature data sent successfully");
}

static void timer_callback(TimerHandle_t xTimer) {
    send_temperature_data();
}

// Interrupt handler for push button (call send_temperature_data)
void isr_handler(void* arg) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Notifica a la tarea que debe enviar los datos
    vTaskNotifyGiveFromISR(temp_task_handle, &xHigherPriorityTaskWoken);

    // Reinicia el temporizador (opcional si también lo quieres reiniciar)
    xTimerResetFromISR(temp_timer, &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}



/*---- Touch Pad Implementation ----*/
static void touchsensor_interrupt_cb(void *arg){
    int task_awoken = pdFALSE;
    touch_event_t evt;

    evt.intr_mask = touch_pad_read_intr_status_mask();
    evt.pad_status = touch_pad_get_status();
    evt.pad_num = touch_pad_get_current_meas_channel();

    xQueueSendFromISR(que_touch, &evt, &task_awoken);
    if (task_awoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

static void touch_pad_set_thresholds(void){
    uint32_t touch_value;
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++) {
        touch_pad_read_benchmark(button[i], &touch_value);
        touch_pad_set_thresh(button[i], touch_value * button_threshold[i]);
        ESP_LOGI(TAG_TOUCH, "touch pad [%d] base %"PRIu32", thresh %"PRIu32, 
                 button[i], touch_value, (uint32_t)(touch_value * button_threshold[i]));
    }
}

static void touchsensor_filter_set(touch_filter_mode_t mode){
    touch_filter_config_t filter_info = {
        .mode = mode,
        .debounce_cnt = 1,
        .noise_thr = 0,
        .jitter_step = 4,
        .smh_lvl = TOUCH_PAD_SMOOTH_IIR_2,
    };
    touch_pad_filter_set_config(&filter_info);
    touch_pad_filter_enable();
    ESP_LOGI(TAG_TOUCH, "touch pad filter init");
}

static void touch_pad_read_task(void *pvParameter){
    touch_event_t evt = {0};
    
    vTaskDelay(50 / portTICK_PERIOD_MS);
    touch_pad_set_thresholds();

    while (1) {
        int ret = xQueueReceive(que_touch, &evt, (TickType_t)portMAX_DELAY);
        if (ret != pdTRUE) {
            continue;
        }
        
        handle_touch_event(&evt);
        
        if (evt.intr_mask & TOUCH_PAD_INTR_MASK_SCAN_DONE) {
            ESP_LOGI(TAG, "Touch sensor group measurement done [%"PRIu32"].", evt.pad_num);
        }
        if (evt.intr_mask & TOUCH_PAD_INTR_MASK_TIMEOUT) {
            ESP_LOGI(TAG, "Touch sensor channel %"PRIu32" timeout. Skip!", evt.pad_num);
            touch_pad_timeout_resume();
        }
    }
}

static void temperature_notify_task(void *pvParameters) {
    while (1) {
        // Espera la notificación desde la ISR del botón
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        ESP_LOGI(TAG, "Push button pressed - sending temperature data");
        send_temperature_data();
    }
}



static void touch_pad_init_config(void ){
    if (que_touch == NULL) {
        que_touch = xQueueCreate(TOUCH_BUTTON_NUM, sizeof(touch_event_t));
    }
    
    ESP_LOGI(TAG, "Initializing touch pad");
    touch_pad_init();
    
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++) {
        touch_pad_config(button[i]);
    }

#if TOUCH_CHANGE_CONFIG
    touch_pad_set_measurement_interval(TOUCH_PAD_SLEEP_CYCLE_DEFAULT);
    touch_pad_set_charge_discharge_times(TOUCH_PAD_MEASURE_CYCLE_DEFAULT);
    touch_pad_set_voltage(TOUCH_PAD_HIGH_VOLTAGE_THRESHOLD, 
                          TOUCH_PAD_LOW_VOLTAGE_THRESHOLD, 
                          TOUCH_PAD_ATTEN_VOLTAGE_THRESHOLD);
    touch_pad_set_idle_channel_connect(TOUCH_PAD_IDLE_CH_CONNECT_DEFAULT);
    
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++) {
        touch_pad_set_cnt_mode(button[i], TOUCH_PAD_SLOPE_DEFAULT, TOUCH_PAD_TIE_OPT_DEFAULT);
    }
#endif

#if TOUCH_BUTTON_DENOISE_ENABLE
    touch_pad_denoise_t denoise = {
        .grade = TOUCH_PAD_DENOISE_BIT4,
        .cap_level = TOUCH_PAD_DENOISE_CAP_L4,
    };
    touch_pad_denoise_set_config(&denoise);
    touch_pad_denoise_enable();
    ESP_LOGI(TAG, "Denoise function init");
#endif

#if TOUCH_BUTTON_WATERPROOF_ENABLE
    touch_pad_waterproof_t waterproof = {
        .guard_ring_pad = button[3],
        .shield_driver = TOUCH_PAD_SHIELD_DRV_L2,
    };
    touch_pad_waterproof_set_config(&waterproof);
    touch_pad_waterproof_enable();
    ESP_LOGI(TAG, "touch pad waterproof init");
#endif

    touchsensor_filter_set(TOUCH_PAD_FILTER_IIR_16);
    touch_pad_timeout_set(true, TOUCH_PAD_THRESHOLD_MAX);
    touch_pad_isr_register(touchsensor_interrupt_cb, NULL, TOUCH_PAD_INTR_MASK_ALL);
    touch_pad_intr_enable(TOUCH_PAD_INTR_MASK_ACTIVE | 
                          TOUCH_PAD_INTR_MASK_INACTIVE | 
                          TOUCH_PAD_INTR_MASK_TIMEOUT);

    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_fsm_start();
}