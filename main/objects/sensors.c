/*
 * Copyright 2021-2024 AVSystem <avsystem@avsystem.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <assert.h>
#include <stdbool.h>

#include <anjay/anjay.h>
#include <anjay/ipso_objects.h>
#include <anjay/ipso_objects_v2.h>
#include <anjay/security.h>
#include <anjay/server.h>

#include <avsystem/commons/avs_log.h>
#include <avsystem/commons/avs_sched.h>

#include "objects/objects.h"
#include "sdkconfig.h"

#include "ds18b20.h"
#include "main.h"

#define TEMPERATURE_OBJ_OID 3303
#define HUMIDITY_OBJ_OID 3304

// Manejadores para las colas de los sensores
extern QueueHandle_t temperature_sensor_1_queue;
extern QueueHandle_t temperature_sensor_2_queue;
extern QueueHandle_t temperature_sensor_3_queue;
extern QueueHandle_t humidity_sensor_DHT11_queue;
extern QueueHandle_t humidity_sensor_DHT22_queue;

typedef struct {
    const char *name;
    const char *unit;
    anjay_oid_t oid;
    double data;
    int (*read_data)(anjay_iid_t iid);
    void (*get_data)(double *sensor_data);
} basic_sensor_context_t;

static float temperature_sensor_data;
int temperature_read_data(anjay_iid_t iid) {
    if (1) {
        
        avs_log(temperature_sensor,
                    WARNING,
                    "IID: %d",
                    iid);

        switch (iid)
        {
        case 0:
            xQueuePeek(temperature_sensor_1_queue, &temperature_sensor_data, pdMS_TO_TICKS(50));
            avs_log(temperature_sensor,
                    INFO,
                    "Sensor 1: %0.2f °C",
                    temperature_sensor_data);
            break;
        case 1:
            xQueuePeek(temperature_sensor_2_queue, &temperature_sensor_data, pdMS_TO_TICKS(50));
            avs_log(temperature_sensor,
                    INFO,
                    "Sensor 2: %0.2f °C",
                    temperature_sensor_data);
            break;
        case 2:
            xQueuePeek(temperature_sensor_3_queue, &temperature_sensor_data, pdMS_TO_TICKS(50));
            avs_log(temperature_sensor,
                    INFO,
                    "Sensor 3: %0.2f °C",
                    temperature_sensor_data);
            break;
        default:
            break;
        }
        return 0;
    } else {
        return -1;
    }
}

static float humidity_sensor_data;
int humidity_read_data(anjay_iid_t iid) {
    if (1) {
        
        avs_log(humidity_sensor,
                WARNING,
                "IID: %d",
                iid);

        switch (iid)
        {
        case 0:
            if(xQueuePeek(humidity_sensor_DHT11_queue, &humidity_sensor_data, pdMS_TO_TICKS(50)) == pdFALSE){
                avs_log(humidity_sensor,
                    ERROR,
                    "ERROR AL LEER DATO DE HUMEDAD SENSOR DHT11"
                    );
                }else{
                    avs_log(humidity_sensor,
                            INFO,
                            "Sensor DHT11: %0.2f %%",
                            humidity_sensor_data);
                    }
            break;
        case 1:
            if(xQueuePeek(humidity_sensor_DHT22_queue, &humidity_sensor_data, pdMS_TO_TICKS(50)) == pdFALSE){
                avs_log(humidity_sensor,
                    ERROR,
                    "ERROR AL LEER DATO DE HUMEDAD SENSOR DHT22"
                    );
                }else{
                    avs_log(humidity_sensor,
                            INFO,
                            "Sensor DHT22: %0.2f %%",
                            humidity_sensor_data);
                    }
            break;
        default:
            break;
        }
        return 0;
    } else {
        return -1;
    }
}

void temperature_get_data(double *sensor_data) {
    *sensor_data = temperature_sensor_data;
}

void humidity_get_data(double *sensor_data) {
    *sensor_data = humidity_sensor_data;
}

static basic_sensor_context_t BASIC_SENSORS_DEF[] = {
    {
        .name = "Temperature sensor",
        .unit = "°C",
        .oid = TEMPERATURE_OBJ_OID,
        .read_data = temperature_read_data,
        .get_data = temperature_get_data,
    },
    {
        .name = "Humidity sensor",
        .unit = "%",
        .oid = HUMIDITY_OBJ_OID,
        .read_data = humidity_read_data,
        .get_data = humidity_get_data,
    },
};

int basic_sensor_get_value(anjay_iid_t iid, void *_ctx, double *value) {
    basic_sensor_context_t *ctx = (basic_sensor_context_t *) _ctx;

    assert(ctx->read_data);
    assert(ctx->get_data);
    assert(value);

    if (!ctx->read_data(iid)) {
        ctx->get_data(&ctx->data);
        *value = ctx->data;
        return 0;
    } else {
        return -1;
    }
}

void sensors_install(anjay_t *anjay) {
#if CONFIG_ANJAY_CLIENT_BOARD_M5STICKC_PLUS
    if (mpu6886_device_init()) {
        avs_log(ipso_object,
                WARNING,
                "Driver for MPU6886 could not be initialized!");
        return;
    }
#endif

    // for (int i = 0; i < (int) AVS_ARRAY_SIZE(BASIC_SENSORS_DEF); i++) {
        basic_sensor_context_t *ctx = &BASIC_SENSORS_DEF[0];

        if (anjay_ipso_basic_sensor_install(anjay, ctx->oid, 3)) {
            avs_log(ipso_object,
                    WARNING,
                    "Object: %s could not be installed",
                    ctx->name);
            // continue;
        }

        if (anjay_ipso_basic_sensor_instance_add(
                    anjay,
                    ctx->oid,
                    0,
                    (anjay_ipso_basic_sensor_impl_t) {
                        .unit = ctx->unit,
                        .user_context = ctx,
                        .min_range_value = NAN,
                        .max_range_value = NAN,
                        .get_value = basic_sensor_get_value
                    })) {
            avs_log(ipso_object,
                    WARNING,
                    "Instance of %s object could not be added",
                    ctx->name);
        }

        if (anjay_ipso_basic_sensor_instance_add(
                    anjay,
                    ctx->oid,
                    1,
                    (anjay_ipso_basic_sensor_impl_t) {
                        .unit = ctx->unit,
                        .user_context = ctx,
                        .min_range_value = NAN,
                        .max_range_value = NAN,
                        .get_value = basic_sensor_get_value
                    })) {
            avs_log(ipso_object,
                    WARNING,
                    "Instance of %s object could not be added",
                    ctx->name);
        }

        if (anjay_ipso_basic_sensor_instance_add(
                    anjay,
                    ctx->oid,
                    2,
                    (anjay_ipso_basic_sensor_impl_t) {
                        .unit = ctx->unit,
                        .user_context = ctx,
                        .min_range_value = NAN,
                        .max_range_value = NAN,
                        .get_value = basic_sensor_get_value
                    })) {
            avs_log(ipso_object,
                    WARNING,
                    "Instance of %s object could not be added",
                    ctx->name);
        }      

        ctx = &BASIC_SENSORS_DEF[1];

        if (anjay_ipso_basic_sensor_install(anjay, ctx->oid, 2)) {
            avs_log(ipso_object,
                    WARNING,
                    "Object: %s could not be installed",
                    ctx->name);
            // continue;
        }

        if (anjay_ipso_basic_sensor_instance_add(
                    anjay,
                    ctx->oid,
                    0,
                    (anjay_ipso_basic_sensor_impl_t) {
                        .unit = ctx->unit,
                        .user_context = ctx,
                        .min_range_value = NAN,
                        .max_range_value = NAN,
                        .get_value = basic_sensor_get_value
                    })) {
            avs_log(ipso_object,
                    WARNING,
                    "Instance of %s object could not be added",
                    ctx->name);
        }

        if (anjay_ipso_basic_sensor_instance_add(
                    anjay,
                    ctx->oid,
                    1,
                    (anjay_ipso_basic_sensor_impl_t) {
                        .unit = ctx->unit,
                        .user_context = ctx,
                        .min_range_value = NAN,
                        .max_range_value = NAN,
                        .get_value = basic_sensor_get_value
                    })) {
            avs_log(ipso_object,
                    WARNING,
                    "Instance of %s object could not be added",
                    ctx->name);
        }     
    // }
}

void sensors_update(anjay_t *anjay) {
    for (int i = 0; i < (int) AVS_ARRAY_SIZE(BASIC_SENSORS_DEF); i++) {
        if(i==0){
        anjay_ipso_basic_sensor_update(anjay, BASIC_SENSORS_DEF[i].oid, 0);
        anjay_ipso_basic_sensor_update(anjay, BASIC_SENSORS_DEF[i].oid, 1);
        anjay_ipso_basic_sensor_update(anjay, BASIC_SENSORS_DEF[i].oid, 2);        
        }else{
            anjay_ipso_basic_sensor_update(anjay, BASIC_SENSORS_DEF[i].oid, 0);
            anjay_ipso_basic_sensor_update(anjay, BASIC_SENSORS_DEF[i].oid, 1);
            }
    }
}

void sensors_release(void) {
#if CONFIG_ANJAY_CLIENT_BOARD_M5STICKC_PLUS
    mpu6886_driver_release();
#endif // CONFIG_ANJAY_CLIENT_BOARD_M5STICKC_PLUS
}
