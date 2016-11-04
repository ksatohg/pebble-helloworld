#include <pebble.h>

static Window *s_window;
static TextLayer *s_text_layer;

#define TEXT_LEN (32)
char text[TEXT_LEN + 1];

BitmapLayer *bitmap_layer;
GBitmap *bitmap;


// TickTimerServiceハンドラ （１秒ごとに呼ばれる）
void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  // 表示する文字を作成
  strftime(text, TEXT_LEN, "%Y-%m-%d%n%H:%M:%S", tick_time);
  
  // 表示する文字を text_layer に設定
  text_layer_set_text(s_text_layer, text);
  
  // ログを表示する
  APP_LOG(APP_LOG_LEVEL_DEBUG, text);
}



static void init(void) {
	// windowを作成
	s_window = window_create();
  Layer *root_layer = window_get_root_layer(s_window);

  // Bitmapレイヤー
  {
    // リソース IMAGE_BACKGROUNDからビットマップを作成
    bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
    // ビットマップ レイヤーを生成
    bitmap_layer = bitmap_layer_create(layer_get_bounds(root_layer));
    // ビットマップ レイヤーに画像を設定
    bitmap_layer_set_bitmap(bitmap_layer, bitmap);
    
    // ビットマップ レイヤーをrootレイヤーに重ねる
    layer_add_child(window_get_root_layer(s_window), bitmap_layer_get_layer(bitmap_layer));
  }
  
  // テキスト レイヤー
  {
    
    // テキストレイヤーを作成
    GRect bounds = layer_get_bounds(root_layer);
  	s_text_layer = text_layer_create(bounds);
    
    // テキストレイヤーのフォントをGOTHICボールド28pxに設定
  	text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    // テキストレイヤーをセンタリング
  	text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
    // テキストレイヤーの文字を白に設定
    text_layer_set_text_color(s_text_layer, GColorWhite);
    // テキストレイヤーの背景色を透明に設定
    text_layer_set_background_color(s_text_layer, GColorClear);
    
    // Enable text flow and paging on the text layer, with a slight inset of 10, for round screens
    text_layer_enable_screen_text_flow_and_paging(s_text_layer, 10);
  
    // テキストレイヤーをrootレイヤーに重ねる
  	layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_text_layer));
  	
  }  
  // windowをウインドウ・スタックに重ねる
	window_stack_push(s_window, true);
	
  // TickTimeServiceを使用して1秒ごとにハンドラが呼ばれるように設定する
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

static void deinit(void) {
	// Destroy the text layer
	text_layer_destroy(s_text_layer);
	
	// Destroy the window
	window_destroy(s_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
