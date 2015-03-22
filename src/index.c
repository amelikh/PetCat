/*
 * main.c
 * Constructs a Window housing an output TextLayer to show data from 
 * either modes of operation of the accelerometer.
 */

#include "pebble.h"

#define TAP_NOT_DATA false

static Window *s_main_window;
static TextLayer *s_output_layer;
static BitmapLayer *s_image_layer;
static GBitmap *s_image_bitmap;



//test

//end test


static void data_handler(AccelData *data, uint32_t num_samples) {
  
  // Long lived buffer
  static char s_buffer[128];
  

  
  // Compose string of all data
  snprintf(s_buffer, sizeof(s_buffer), 
    "N X,Y,Z\n0 %d,%d,%d", 
    data[0].x, data[0].y, data[0].z
  ); 
        
  //Show the data

 
  
  text_layer_set_text(s_output_layer, "BADASS CAT");
   
  
  if(data[0].x>0){
   // text_layer_set_text(s_output_layer,"LEFT");
    s_image_bitmap = gbitmap_create_with_resource(RESOURCE_ID_WHITE_CAT);
    bitmap_layer_set_bitmap(s_image_layer, s_image_bitmap);
  }
  if(data[0].x<0){
   // text_layer_set_text(s_output_layer,"RIGHT");
    s_image_bitmap = gbitmap_create_with_resource(RESOURCE_ID_WHITE_CAT_RIGHT);
    bitmap_layer_set_bitmap(s_image_layer, s_image_bitmap);
   
  }
   
}


static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  //Create output TextLayer
  s_output_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 10, window_bounds.size.h));
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_output_layer, "No data yet.");
  text_layer_set_overflow_mode(s_output_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
  
  //image????? 
   s_image_bitmap = gbitmap_create_with_resource(RESOURCE_ID_WHITE_CAT);

  s_image_layer = bitmap_layer_create(GRect(2, 50, window_bounds.size.w - 50, window_bounds.size.h - 60));
  bitmap_layer_set_bitmap(s_image_layer, s_image_bitmap);
  bitmap_layer_set_background_color(s_image_layer, GColorWhite);
  bitmap_layer_set_alignment(s_image_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_image_layer));
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  bitmap_layer_destroy(s_image_layer);
}

static void init() {
  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);

  // Use tap service? If not, use data service
  if (TAP_NOT_DATA) {
    // Subscribe to the accelerometer tap service
    
  } else {
    // Subscribe to the accelerometer data service
    int num_samples = 3;
    accel_data_service_subscribe(num_samples, data_handler);

    // Choose update rate
    accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
  }
}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);

  if (TAP_NOT_DATA) {
    accel_tap_service_unsubscribe();
  } else {
    accel_data_service_unsubscribe();
  }
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
