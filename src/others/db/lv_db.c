/**
 * @file lv_db.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_db.h"
#if LV_USE_DB

#include "../../misc/lv_assert.h"

/*********************
 *      DEFINES
 *********************/
#define LV_DB_ID_FIRST  0x00010001

/**********************
 *      TYPEDEFS
 **********************/
typedef struct _lv_db_entry_t {
    void * value;
    uint32_t msg_idx;
#if LV_USE_USER_DATA
    void * user_data;
#endif
} lv_db_entry_t;


/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_db_entry_t * get_entry_from_value(lv_db_t * db, const void * value);

/**********************
 *  STATIC VARIABLES
 **********************/
static uint32_t msg_id_last = LV_DB_ID_FIRST;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
lv_db_t * lv_db_create(void)
{
    lv_db_t * db = lv_malloc(sizeof(lv_db_t));
    if(db == NULL) {
        LV_ASSERT_MALLOC(db);
        return NULL;
    }

    lv_memzero(db, sizeof(lv_db_t));
    return db;
}

void lv_db_del(lv_db_t * db)
{
    LV_ASSERT_NULL(db);
    lv_free(db->entries);
    lv_free(db);
}

lv_res_t lv_db_add_value(lv_db_t * db, void * value)
{
    lv_db_entry_t * entries = lv_realloc(db->entries, (db->entry_cnt + 1) * sizeof(lv_db_entry_t));
    if(entries == NULL) {
        LV_ASSERT_MALLOC(entries);
        LV_LOG_ERROR("Couldn't add entry to the database");
        return LV_RES_INV;
    }

    db->entries = entries;
    lv_db_entry_t * e = &db->entries[db->entry_cnt];
    db->entry_cnt++;

    lv_memzero(e, sizeof(lv_db_entry_t));
    e->msg_idx = msg_id_last;
    e->value = value;

    msg_id_last++;
    return LV_RES_OK;
}

void lv_db_del_value(lv_db_t * db, void * value)
{
    LV_UNUSED(db);
    LV_UNUSED(value);
    /*TODO*/
}

lv_res_t lv_db_update_value(lv_db_t * db, const void * value)
{
    LV_ASSERT_NULL(db);
    lv_db_entry_t * e = get_entry_from_value(db, value);
    if(e == NULL) {
        LV_LOG_WARN("`value` was not found in the database");
        return LV_RES_INV;
    }

    lv_msg_send(e->msg_idx, e->value);
    return LV_RES_OK;
}

void * lv_db_subscribe_to_value(lv_db_t * db, const void * value, lv_msg_subscribe_cb_t cb, void * user_data)
{
    LV_ASSERT_NULL(db);
    lv_db_entry_t * e = get_entry_from_value(db, value);
    if(e == NULL) {
        LV_LOG_WARN("`value` was not found in the database");
        return LV_RES_INV;
    }

    return lv_msg_subscribe(e->msg_idx, cb, user_data);
}

void * lv_db_subscribe_obj_to_value(lv_db_t * db, const void * value, lv_obj_t * obj, void * user_data)
{

    LV_ASSERT_NULL(db);
    lv_db_entry_t * e = get_entry_from_value(db, value);
    if(e == NULL) {
        LV_LOG_WARN("`value` was not found in the database");
        return LV_RES_INV;
    }

    return lv_msg_subscribe_obj(e->msg_idx, obj, user_data);
}

void lv_db_unsubscribe_from_value(void * s)
{
    lv_msg_unsubscribe(s);
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

static lv_db_entry_t * get_entry_from_value(lv_db_t * db, const void * value)
{
    LV_ASSERT_NULL(db);
    lv_db_entry_t * e;
    uint32_t i;
    for(i = 0; i < db->entry_cnt; i++) {
        e = &db->entries[i];
        if(e->value == value) {
            return e;
        }
    }
    return NULL;
}

#endif /*LV_USE_DB*/

