#if LV_BUILD_TEST
#include "../lvgl.h"

#include "unity/unity.h"

static lv_db_t * db;

void setUp(void)
{
    /* Function run before every test */
    db = lv_db_create();
}

void tearDown(void)
{
    /* Function run after every test */
    lv_db_del(db);
}


static uint32_t value_received;

static void msg_cb(lv_msg_t * msg)
{
    uint32_t * v = lv_msg_get_payload(msg);
    value_received = *v;
}
static void event_cb(lv_event_t * e)
{
    lv_msg_t * msg = lv_event_get_msg(e);
    uint32_t * v = lv_msg_get_payload(msg);
    lv_label_set_text_fmt(lv_event_get_target(e), "%d", *v);
}

void test_add_entry_and_send_msg(void)
{
    static uint32_t value = 100;

    lv_res_t res = lv_db_add_value(db, &value);
    TEST_ASSERT_EQUAL_INT(LV_RES_OK, res);
    lv_db_subscribe_to_value(db, &value, msg_cb, NULL);

    value_received = 0;
    value = 100;
    lv_db_update_value(db, &value);
    TEST_ASSERT_EQUAL_UINT32(100, value_received);

    value = 200;
    lv_db_update_value(db, &value);
    TEST_ASSERT_EQUAL_UINT32(200, value_received);

    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_db_subscribe_obj_to_value(db, &value, label, NULL);
    lv_obj_add_event_cb(label, event_cb, LV_EVENT_MSG_RECEIVED, NULL);

    value = 300;
    lv_db_update_value(db, &value);
    TEST_ASSERT_EQUAL_STRING("300", lv_label_get_text(label));
}


#endif
