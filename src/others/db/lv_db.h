/**
 * @file lv_database.h
 *
 */

#ifndef LV_DATABASE_H
#define LV_DATABASE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../lv_conf_internal.h"
#if LV_USE_DB

#include <stdbool.h>
#include "../msg/lv_msg.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
struct _lv_db_entry_t;

typedef struct {
    struct _lv_db_entry_t * entries;
    uint32_t entry_cnt;
#if LV_USE_USER_DATA
    void * user_data;
#endif
} lv_db_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

lv_db_t * lv_db_create(void);

void lv_db_del(lv_db_t * db);

lv_res_t lv_db_add_value(lv_db_t * db, void * value);

void lv_db_del_value(lv_db_t * db, void * value);

lv_res_t lv_db_update_value(lv_db_t * db, const void * value);


/**
 * Subscribe to an `msg_id`
 * @param msg_id        the message ID to listen to
 * @param cb            callback to call if a message with `msg_id` was sent
 * @param user_data     arbitrary data which will be available in `cb` too
 * @return              pointer to a "subscribe object". It can be used the unsubscribe.
 */
void * lv_db_subscribe_to_value(lv_db_t * db, const void * value, lv_msg_subscribe_cb_t cb, void * user_data);

/**
 * Subscribe an `lv_obj` to a message.
 * `LV_EVENT_MSG_RECEIVED` will be triggered if a message with matching ID was sent
 * @param msg_id        the message ID to listen to
 * @param obj           pointer to an `lv_obj`
 * @param user_data     arbitrary data which will be available in `cb` too
 * @return              pointer to a "subscribe object". It can be used the unsubscribe.
 */
void * lv_db_subscribe_obj_to_value(lv_db_t * db, const void * value, lv_obj_t * obj, void * user_data);

/**
 * Cancel a previous subscription
 * @param s             pointer to a "subscibe object".
 *                      Return value of `lv_msg_subscribe` or `lv_msg_subscribe_obj`
 */
void lv_db_unsubscribe_from_value(void * s);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_DB*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_DATABASE_H*/
