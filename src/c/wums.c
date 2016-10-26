#include <pebble.h>
#include "enamel.h"
#include <pebble-events/pebble-events.h>

#define time_font fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS)
#define date_font fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS)

static EventHandle s_updatesettings_event_handler;

static Window *s_main_window;
// static Layer *s_window_layer;
static TextLayer *s_time_layer, *s_date_layer; 
static Layer *s_battery_layer;
static int s_battery_level;
const char *FormatDate;

#define TXT_COL (GColorWhite)
#define BG_COL (GColorBlack)

static void update_time() {
  FormatDate = enamel_get_DateFormat();
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char time_buffer[8];
  strftime(time_buffer, sizeof(time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  // Copy date into buffer from tm structure
  static char date_buffer[16];
  if (strcmp(FormatDate, "1") == 0){
    strftime(date_buffer, sizeof(date_buffer), "%y.%m.%d", tick_time);
  } else if (strcmp(FormatDate, "2") == 0) {
    strftime(date_buffer, sizeof(date_buffer), "%m.%d.%y", tick_time);
  } else if (strcmp(FormatDate, "3") == 0) {
    strftime(date_buffer, sizeof(date_buffer), "%d.%m.%y", tick_time);
  }

    // Display the time on the time TextLayer
  text_layer_set_text(s_time_layer, time_buffer);
    // Display the date on the date TextLayer
  text_layer_set_text(s_date_layer, date_buffer);
}
  
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void battery_callback(BatteryChargeState state) {
  // Record the new battery level
  s_battery_level = state.charge_percent;
}

static void battery_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

  // define the width of the bar and the necessary space left if the bar to place the bar in the middle
  int width = (int)(float)(((float)s_battery_level / 100.0F) * bounds.size.w);
  int spacer = ((bounds.size.w - width) / 2);

  // Draw the background
  graphics_context_set_fill_color(ctx, BG_COL);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  // Draw the bar
  graphics_context_set_fill_color(ctx, enamel_get_LineColor());
  graphics_fill_rect(ctx, GRect(spacer, 0, width, bounds.size.h), 0, GCornerNone);
}

static void main_window_load(Window *window) {

  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  // s_window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(window, BG_COL);
  
  // Create the TextLayer
  s_time_layer = text_layer_create(GRect(0, (bounds.size.h/2)-48, bounds.size.w, 48));
  text_layer_set_background_color(s_time_layer, BG_COL);
  text_layer_set_text_color(s_time_layer, TXT_COL);
  text_layer_set_font(s_time_layer, time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Create battery meter Layer
  s_battery_layer = layer_create(GRect(0, 0, bounds.size.w, 10));
  layer_set_update_proc(s_battery_layer, battery_update_proc);
    
  // Create date TextLayer
  s_date_layer = text_layer_create(GRect(0, (bounds.size.h/2)+2, bounds.size.w, 20));
  text_layer_set_text_color(s_date_layer, TXT_COL);
  text_layer_set_background_color(s_date_layer, BG_COL);
  text_layer_set_font(s_date_layer, date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
  // Add Layers
  //layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), s_battery_layer);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  
  // Update battery meter
  layer_mark_dirty(s_battery_layer);
  
}

static void main_window_unload(Window *window) {
  
  // Destroy Layers
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  layer_destroy(s_battery_layer);
}

static void init() {
  
  // Initialize Enamel to register App Message handlers and restores settings
  enamel_init();
  
  s_updatesettings_event_handler = enamel_settings_received_subscribe(update_time, s_date_layer);

  // call pebble-events app_message_open function
  events_app_message_open();
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

    // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
    // Make sure the time is displayed from the start
  update_time();
  
  // Register for battery level updates
battery_state_service_subscribe(battery_callback);
  
// Ensure battery level is displayed from the start
battery_callback(battery_state_service_peek());  

}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
  
  enamel_settings_received_unsubscribe(s_updatesettings_event_handler);
  // Deinit Enamel to unregister App Message handlers and save settings
  enamel_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}