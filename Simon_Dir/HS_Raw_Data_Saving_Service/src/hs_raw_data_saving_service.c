#include "hs_raw_data_saving_service.h"

typedef struct appdata {
	Evas_Object *win;	// 윈도우
	Evas_Object *conform;	// Conformant, 중간 층 인듯
	Evas_Object *label;	// 라벨
	/* Box */
	Evas_Object *box;
	/* List */
	Evas_Object *list;
	/* Naviframe */
	Elm_Object_Item *navi_item;
} appdata_s;

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}

/*
 * creates the UI components: window, conformant, and label
 * receives a pointer to fill in the appdata_s structure
 */
static void
create_base_gui(appdata_s *ad)
{
	/* Window */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);	// window component is created with elm_win_util_standard_add
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
	eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);
	/*
	 * One of callbacks handles the delete and request event when the window is to be closed
	 * The ohter handles the EEXT_CALLBACK_EVENT event when the h/w Back key is pressed.
	 */

	/* Conformant
	 * it is needed to show the indicator bar at the top of screen:
	 * */
	ad->conform = elm_conformant_add(ad->win);	// first object added inside the window with the elm_conformant_add()
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);	// decides whether the indicator is visible
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);	// sets the indicator opacity mode
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);	// the conformant size and position are controlled by the window component directly
	evas_object_show(ad->conform);	// makes the conformant component visible

	/* Box
	 * using the conformant as the parent, and set the box as the conformant content.
	 */
	ad->box = elm_box_add(ad->conform);
	evas_object_size_hint_weight_set(ad->box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(ad->box);
	elm_object_content_set(ad->conform, ad->box);

	/* Modify the label code */
	ad->label = elm_label_add(ad->box);
	elm_object_text_set(ad->label, "<align=center>Hello Simon</align>");
	evas_object_size_hint_weight_set(ad->label, 0.0, 0.0);
	/* Comment out the elm_object_content_set() function */
	/* elm_object_content_set(ad->conform, ad->label); */
	evas_object_size_hint_align_set(ad->label, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_min_set(ad->label, 50, 50);
	/* Show and add to box */
	evas_object_show(ad->label);
	elm_box_pack_end(ad->box, ad->label);


	/* Label */
	/*
	ad->label = elm_label_add(ad->conform);	// the label component for the text is added.
	elm_object_text_set(ad->label, "<align=center><font_size=50>Hello Simon</font></align>");
	elm_object_style_set(ad->label, "marker");
	evas_object_size_hint_weight_set(ad->label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_content_set(ad->conform, ad->label);
	*/

	int i;
	/* Create the list */
	ad->list = elm_list_add(ad->box);
	/* Set the list size */
	evas_object_size_hint_weight_set(ad->list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->list, EVAS_HINT_FILL, EVAS_HINT_FILL);

	for (i=0;i<4;i++) {
		char tmp[8];
		snprintf(tmp, sizeof(tmp), "Item %d", i+1);
		/* Add an item to the list */
		elm_list_item_append(ad->list, tmp, NULL, NULL, NULL, NULL);
	}

	/* Show and add to box */
	evas_object_show(ad->list);
	elm_box_pack_end(ad->box, ad->list);

	// Show window after base gui is set up
	evas_object_show(ad->win);



}

static bool
app_create(void *data)	// main loop, called when the application process starts, used to create UI comps
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */
	appdata_s *ad = data;

	create_base_gui(ad);

	return true;
}

/*
 * Called after the app_create callback when the process starts
 * Called when a launch request is received while the process is running.
 * Can receive app_control data
 * Used to implement parameter-specific actions of app.
 */
static void
app_control(app_control_h app_control, void *data)	// called after the app_create callback when the process starts
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)	// called when the app window is totally hidden.
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)	// called when the app window is shown.
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data) // called when the app process is terminating, called after the main loop quits
{
	/* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
	// appdata_s 화면 요소를 나타내는 구조체이다.
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	// 각각은 function pointer이다.
	event_callback.create = app_create; 
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);
	ui_app_remove_event_handler(handlers[APP_EVENT_LOW_MEMORY]);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
	}

	return ret;
}
