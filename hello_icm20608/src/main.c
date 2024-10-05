/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <math.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define M_PI   3.14159265358979323846

// Raw data from the IMU
double accelX = 0, accelY = 0, accelZ = 0;
double gyrX = 0, gyrY = 0, gyrZ = 0;

// Predicted Orientation (Gyro)
double gx = 0, gy = 0, gz = 0;

// Predicted Orientation (Acc)
double ax = 0, ay = 0;

// Sample Frequency 100 Hz
const int TIME_STEP_MS = 10;

static int process_icm20608(const struct device *dev)
{
    struct sensor_value temperature;
    struct sensor_value accel[3];
    struct sensor_value gyro[3];
    int rc = sensor_sample_fetch(dev);

    if (rc == 0) {
        rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ,
                    accel);
    }
    if (rc == 0) {
        rc = sensor_channel_get(dev, SENSOR_CHAN_GYRO_XYZ,
                    gyro);
    }
    if (rc == 0) {
        rc = sensor_channel_get(dev, SENSOR_CHAN_DIE_TEMP,
                    &temperature);
    }
    if (rc == 0) {

        // printf("[%s]:%g Cel\n"
        //        "  accel %f %f %f m/s/s\n"
        //        "  gyro  %f %f %f rad/s\n",
        //        sensor_value_to_double(&temperature));

        accelX = sensor_value_to_double(&accel[0]);
        accelY = sensor_value_to_double(&accel[1]);
        accelZ = sensor_value_to_double(&accel[2]);

        gyrX = sensor_value_to_double(&gyro[0]);
        gyrY = sensor_value_to_double(&gyro[1]);
        gyrZ = sensor_value_to_double(&gyro[2]);

        // angles based on accelerometer
        ax = atan2(accelY, accelZ) * 180 / M_PI;                                      // roll
        ay = atan2(-accelX, sqrt( pow(accelY, 2) + pow(accelZ, 2))) * 180 / M_PI;    // pitch

        // This is incorrect, many tutorials make this mistake
        // ax = atan2(accelY, sqrt( pow(accelX, 2) + pow(accelZ, 2))) * 180 / M_PI;    // roll

        // angles based on gyro (deg/s)
        gx = gx + gyrX * TIME_STEP_MS / 1000 / M_PI * 180;
        gy = gy + gyrY * TIME_STEP_MS / 1000 / M_PI * 180;
        gz = gz + gyrZ * TIME_STEP_MS / 1000 / M_PI * 180;

        // complementary filter
        gx = gx * 0.96 + ax * 0.04;
        gy = gy * 0.96 + ay * 0.04;

        printf("%g %g %g %g %g %g %.4f %.4f %.4f \n", accelX, accelY, accelZ, gyrX, gyrY, gyrZ, gx, gy, gz);
    } else {
        printf("sample fetch/get failed: %d\n", rc);
    }

    return rc;
}

#ifdef CONFIG_ICM20608_TRIGGER
static struct sensor_trigger trigger;

static void handle_icm20608_drdy(const struct device *dev,
                const struct sensor_trigger *trig)
{
	uint32_t start = k_uptime_get_32();

    int rc = process_icm20608(dev);

    if (rc != 0) {
        printf("cancelling trigger due to failure: %d\n", rc);
        (void)sensor_trigger_set(dev, trig, NULL);
        return;
    }

	while( (k_uptime_get_32() - start) < TIME_STEP_MS );
}
#endif /* CONFIG_ ICM20608_TRIGGER */

int main(void)
{
    printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

    const struct device *const icm20608 = DEVICE_DT_GET_ONE(invensense_icm20608);
    if (!device_is_ready(icm20608)) {
        printf("Device %s is not ready\n", icm20608->name);
        return 0;
    }

#ifdef CONFIG_ICM20608_TRIGGER
    trigger = (struct sensor_trigger) {
        .type = SENSOR_TRIG_DATA_READY,
        .chan = SENSOR_CHAN_ALL,
    };
    if (sensor_trigger_set(icm20608, &trigger,
                   handle_icm20608_drdy) < 0) {
        printf("Cannot configure trigger\n");
        return 0;
    }
    printf("Configured for triggered sampling.\n");
#endif

    while (!IS_ENABLED(CONFIG_ICM20608_TRIGGER)) {
		uint32_t start = k_uptime_get_32();

        int rc = process_icm20608(icm20608);

        if (rc != 0) {
            break;
        }
		while( (k_uptime_get_32() - start) < TIME_STEP_MS );
    }

    return 0;
}
