#include <application.h>
#include <tools.h>

// LED instance
bc_led_t led;

// Button instance
bc_button_t button;
uint16_t event_count = 0;

// LoRa instance
bc_cmwx1zzabz_t lora;

void lora_callback(bc_cmwx1zzabz_t *self, bc_cmwx1zzabz_event_t event, void *event_param)
{
    if (event == BC_CMWX1ZZABZ_EVENT_JOIN_SUCCESS)
    {
        bc_led_set_mode(&led, BC_LED_MODE_OFF);
    }

    if (event == BC_CMWX1ZZABZ_EVENT_ERROR)
    {
        bc_led_set_mode(&led, BC_LED_MODE_BLINK_FAST);
    }

    if (event == BC_CMWX1ZZABZ_EVENT_JOIN_ERROR)
    {
        bc_led_set_mode(&led, BC_LED_MODE_BLINK_FAST);
    }

    if (event == BC_CMWX1ZZABZ_EVENT_SEND_MESSAGE_START)
    {
        bc_led_set_mode(&led, BC_LED_MODE_ON);
    }

    if (event == BC_CMWX1ZZABZ_EVENT_SEND_MESSAGE_DONE)
    {
        bc_led_set_mode(&led, BC_LED_MODE_OFF);
    }

    if (event == BC_CMWX1ZZABZ_EVENT_MESSAGE_RECEIVED)
    {
        volatile uint8_t port = bc_cmwx1zzabz_get_received_message_port(self);
        volatile uint32_t length = bc_cmwx1zzabz_get_received_message_length(self);

        uint8_t msg_buffer[51];
        volatile uint32_t len = bc_cmwx1zzabz_get_received_message_data(self, msg_buffer, sizeof(msg_buffer));

        bc_led_pulse(&led, 800);

        if (port == 1)
        {
            if (bc_module_lcd_is_ready())
            {
                bc_system_pll_enable();

                if (msg_buffer[0] != 0)
                {
                    bc_module_lcd_clear();
                }

                bc_module_lcd_set_font(&bc_font_ubuntu_24);

                bc_module_lcd_draw_string(msg_buffer[1], msg_buffer[2], (char *) msg_buffer + 3, 1);

                bc_module_lcd_update();

                bc_system_pll_disable();
            }
        }
    }
}

void transmit_button_task(void *param)
{
    (void) param;

    if (!bc_cmwx1zzabz_is_ready(&lora))
    {
        bc_scheduler_plan_current_now();
        return;
    }

    bc_scheduler_unregister(bc_scheduler_get_current_task_id());

    bc_cmwx1zzabz_send_message(&lora, &event_count, sizeof(event_count));
}

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    if (event == BC_BUTTON_EVENT_PRESS)
    {
        event_count++;

        bc_scheduler_register(transmit_button_task, NULL, 0);
    }
}

void application_init(void)
{

    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);

    // Initialize button
    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    // Initialize LCD
    bc_module_lcd_init();
    bc_module_lcd_clear();
    bc_module_lcd_update();

    bc_cmwx1zzabz_init(&lora, BC_UART_UART1);
    bc_cmwx1zzabz_set_event_handler(&lora, lora_callback, NULL);
    //bc_cmwx1zzabz_set_appkey(&lora, "4257FAD1D73C3B7D4C2EDBAE6EBDA740");
    //bc_cmwx1zzabz_set_appskey(&lora, "4357FAD1D73C3B7D4C2EDBAE6EBDA740");
    //bc_cmwx1zzabz_set_nwkskey(&lora, "4457FAD1D73C3B7D4C2EDBAE6EBDA740");

    bc_cmwx1zzabz_set_band(&lora, BC_CMWX1ZZABZ_CONFIG_BAND_EU868);
    bc_cmwx1zzabz_set_mode(&lora, BC_CMWX1ZZABZ_CONFIG_MODE_OTAA);
    bc_cmwx1zzabz_set_class(&lora, BC_CMWX1ZZABZ_CONFIG_CLASS_C);

    bc_cmwx1zzabz_join(&lora);

    bc_led_set_mode(&led, BC_LED_MODE_ON);


    // uint8_t buffer[51];
    // uint8_t in[64];
    // for (size_t i = 0; i < sizeof(in); i++)
    // {
    //     in[i] = i;
    // }

    // size_t a = compress(in, sizeof(in), buffer, sizeof(buffer));

    // uint8_t out[64];

    // size_t b = decompress(out, sizeof(out), buffer, sizeof(buffer));

    // if (a == b)
    // {
    //     a++;
    // }
}
