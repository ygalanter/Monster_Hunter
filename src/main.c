#include <pebble.h>

Window *window;
GBitmap *background_img, *minute_img, *hour_img;
BitmapLayer *background_layer;
RotBitmapLayer *rot_minute, *rot_hour;

static void handle_tick(struct tm *t, TimeUnits units_changed) {
 //test
  
  t->tm_hour = 6;
  t->tm_min = 45;
  
 rot_bitmap_layer_set_angle(rot_minute, TRIG_MAX_ANGLE * t->tm_min / 60);
 rot_bitmap_layer_set_angle(rot_hour,  (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6));
}


static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
 
  //background
  background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  background_img = gbitmap_create_with_resource(RESOURCE_ID_BACK);
  bitmap_layer_set_bitmap(background_layer, background_img);
  layer_add_child(window_layer, bitmap_layer_get_layer(background_layer));
  
  //hour
  hour_img = gbitmap_create_with_resource(RESOURCE_ID_HOUR);
  rot_hour = rot_bitmap_layer_create(hour_img);
  rot_bitmap_set_compositing_mode(rot_hour, GCompOpSet);
  rot_bitmap_set_src_ic(rot_hour,GPoint(10, 47));
  layer_set_frame((Layer*)rot_hour, GRect(22, 36, 108, 108));
  layer_add_child(window_layer, (Layer*)rot_hour);
  
  //minute
  minute_img = gbitmap_create_with_resource(RESOURCE_ID_MINUTE);
  rot_minute = rot_bitmap_layer_create(minute_img);
  rot_bitmap_set_compositing_mode(rot_minute, GCompOpSet);
  rot_bitmap_set_src_ic(rot_minute,GPoint(13, 67));
  layer_set_frame((Layer*)rot_minute, GRect(0, 16, 144, 144));
  layer_add_child(window_layer, (Layer*)rot_minute);
  
}

static void window_unload(Window *window) {
    gbitmap_destroy(background_img);
    bitmap_layer_destroy(background_layer);
  
    gbitmap_destroy(minute_img);
    rot_bitmap_layer_destroy(rot_minute);
  
    gbitmap_destroy(hour_img);
    rot_bitmap_layer_destroy(rot_hour);
}

static void init() {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_set_background_color(window, GColorWindsorTan);
  window_stack_push(window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
}

static void deinit() {

  tick_timer_service_unsubscribe();
  window_destroy(window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}