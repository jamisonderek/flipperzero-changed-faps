#include "game.h"
#include "game_settings.h"
#include "levels/level_menu.h"
#include "levels/level_game.h"
#include "levels/level_settings.h"
#include "levels/level_message.h"

const NotificationSequence sequence_sound_blip = {
    &message_note_c7,
    &message_delay_50,
    &message_sound_off,
    NULL,
};

const NotificationSequence sequence_sound_menu = {
    &message_note_c6,
    &message_delay_10,
    &message_sound_off,
    NULL,
};

const GpioPinRecord* find_gpio_pin(const GpioPin* pin) {
    for(size_t i = 0; i < gpio_pins_count; i++) {
        if(gpio_pins[i].pin == pin) {
            return &gpio_pins[i];
        }
    }

    return NULL;
}

void game_start(GameManager* game_manager, void* ctx) {
    GameContext* context = ctx;
    context->imu = imu_alloc();
    context->imu_present = imu_present(context->imu);

    context->adc_handle = furi_hal_adc_acquire();
    furi_hal_adc_configure(context->adc_handle); // 0-2048 mV range (for ~10KOhm sensors)

    context->paddle_left = find_gpio_pin(&gpio_ext_pc0);
    furi_assert(context->paddle_left);
    furi_hal_gpio_init(context->paddle_left->pin, GpioModeAnalog, GpioPullNo, GpioSpeedLow);

    context->paddle_button = find_gpio_pin(&gpio_ext_pc1);
    furi_assert(context->paddle_button);
    furi_hal_gpio_init(context->paddle_button->pin, GpioModeAnalog, GpioPullNo, GpioSpeedLow);

    context->paddle_present = false;
    float voltage = furi_hal_adc_convert_to_voltage(
        context->adc_handle,
        furi_hal_adc_read(context->adc_handle, context->paddle_left->channel));
    // Detect paddle if it is in the extreme position (full left/right)
    if(voltage < 50 || voltage > 1500) {
        context->paddle_present = true;
    }

    context->levels.menu = game_manager_add_level(game_manager, &level_menu);
    context->levels.settings = game_manager_add_level(game_manager, &level_settings);
    context->levels.game = game_manager_add_level(game_manager, &level_game);
    context->levels.message = game_manager_add_level(game_manager, &level_message);

    if(!game_settings_load(&context->settings)) {
        context->settings.sound = true;
        context->settings.show_fps = false;
    }

    context->app = furi_record_open(RECORD_NOTIFICATION);
    context->game_manager = game_manager;

    game_manager_show_fps_set(context->game_manager, context->settings.show_fps);
}

void game_stop(void* ctx) {
    GameContext* context = ctx;
    imu_free(context->imu);
    furi_hal_adc_release(context->adc_handle);

    furi_record_close(RECORD_NOTIFICATION);
}

const Game game = {
    .target_fps = 30,
    .show_fps = false,
    .always_backlight = true,
    .start = game_start,
    .stop = game_stop,
    .context_size = sizeof(GameContext),
};

void game_switch_sound(GameContext* context) {
    context->settings.sound = !context->settings.sound;
    game_settings_save(&context->settings);
}

void game_switch_show_fps(GameContext* context) {
    context->settings.show_fps = !context->settings.show_fps;
    game_manager_show_fps_set(context->game_manager, context->settings.show_fps);
    game_settings_save(&context->settings);
}

void game_sound_play(GameContext* context, const NotificationSequence* sequence) {
    if(context->settings.sound) {
        notification_message(context->app, sequence);
    }
}