# Database

The goal of a database (`lv_db`) is to have the application state (the variables containing the current settings, values, etc) in one central place. The database acts as an abstraction layer for the UI. This way the UI elements and the application logic doesn't have to know about each other directly but they see only the these values.

The database is built on top of [lv_msg](/others/msg) to allow subscribing to values and get notification on value change.

For example, the current state of the battery (percentage) can be added to the database. When the MCU measures the current voltage, it can just the set a `battery_pct` value and call `lv_db_update_value(db, &battery_pct)`. After that all the UI elements who are subscribed to `battery_pct` (e.g. icon on the header, popup to show low battery, etc) will be notified.

This way the UI elements can be created and deleted dynamically, and they can subscribe to and unsubscribe from variables. The task which sets the actual value doesn't need to care which widgets are interested in that value, or even e.g. a label is now exists or not, and doesn't need to know the exact structure of the UI either, just set the value and call the update function.


`lv_db` is not superior to `lv_msg` but only completes it. Database solves the issue of keeping track of global data (a.k.a. application state). `lv_msg` still can be used to pass messages to the system. E.g. "Low voltage", "Limit reached", "Login failed", etc.

## Creating a database
A new database can be created an runtime using:
```c
lv_db_t * db = lv_db_create();
```
Any number of databases can be created. For example you can create a smaller database for only one screen or even a group of widgets to easily keep track of value and state changes.

A database and all its entries can be destroyed with
```
lv_db_del(db);
```

## Values

A new value can be added to the database like this:
```c
static uint32_t value = 0;
lv_res_t res = lv_db_add_value(db, &value);
```

The first parameter is a database to which the value should be added. The second parameter is a pointer to any kind of data. The database stores only the value's pointer so `value` should be global, static or dynamically allocated.

## Subscribe to a value

To subscription to a value [lv_msg](others/msg) is used internally, so it's recommended to take look at it first.

To add function to be called on value change use:
```
lv_db_subscribe_to_value(db, &value, msg_cb, user_data);
```

To subscribe an `lv_obj` to a value use:
```c
lv_db_subscribe_obj_to_value(db, &value, obj, user_data);
```
It will send an `LV_EVENT_MSG_RECEIVED` on value update where the message's payload is the value.


## Update value
As `lv_db` doesn't know the type of the value so the user needs to update it and tell the database that the entry has been changed.

In the most simple case:
```c

static uint32_t value = 0;
lv_db_add_value(db, &value);
lv_db_subscribe_to_value(db, &value, msg_cb, NULL);
...
value = 10;
lv_db_update_value(db, &value); //Will call msg_cb
```

## Example

```eval_rst

.. include:: ../../examples/others/db/index.rst

```
## API


```eval_rst

.. doxygenfile:: lv_db.h
  :project: lvgl

```
