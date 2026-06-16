/*
 * This file is part of the osnmpd project (https://github.com/verrio/osnmpd).
 * Copyright (C) 2016 Olivier Verriest
 * Copyright (C) 2026 thenebular
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SRC_SNMP_MIB_SYSTEM_POWER_CACHE_H_
#define SRC_SNMP_MIB_SYSTEM_POWER_CACHE_H_

/* 
 * =============================================================================
 * BATTERY-MIB (RFC 7577) Data Structures and Enumerations
 * =============================================================================
 */

/* Distinguishes the general classification of a battery (batteryType) */
enum BatteryType {
    BATTERY_TYPE_UNKNOWN = 1,
    BATTERY_TYPE_OTHER = 2,
    BATTERY_TYPE_PRIMARY = 3,       /* Non-rechargeable */
    BATTERY_TYPE_RECHARGEABLE = 4,  /* Secondary/Rechargeable */
    BATTERY_TYPE_CAPACITOR = 5      /* Capacitor acting as a battery */
};

/* Defines the chemical/physical technology of the battery (batteryTechnology) */
enum BatteryTechnology {
    BATTERY_TECH_UNKNOWN = 1,
    BATTERY_TECH_OTHER = 2,
    BATTERY_TECH_ZINC_CARBON = 3,
    BATTERY_TECH_ZINC_CHLORIDE = 4,
    BATTERY_TECH_NICKEL_OXYHYDROXIDE = 5,
    BATTERY_TECH_LITHIUM_COPPER_OXIDE = 6,
    BATTERY_TECH_LITHIUM_IRON_DISULFIDE = 7,
    BATTERY_TECH_LITHIUM_MANGNESE_DIOXIDE = 8,
    BATTERY_TECH_ZINC_AIR = 9,
    BATTERY_TECH_SILVER_OXIDE = 10,
    BATTERY_TECH_ALKALINE = 11,
    BATTERY_TECH_LEAD_ACID = 12,
    BATTERY_TECH_VALVE_REGULATED_LEAD_ACID_GEL = 13,
    BATTERY_TECH_VALVE_REGULATED_LEAD_ACID_AGM = 14,
    BATTERY_TECH_NICKEL_CADMIUM = 15,
    BATTERY_TECH_NICKEL_METAL_HYDRIDE = 16,
    BATTERY_TECH_NICKEL_ZINC = 17,
    BATTERY_TECH_LITHIUM_ION = 18,
    BATTERY_TECH_LITHIUM_POLYMER = 19,
    BATTERY_TECH_DOUBLE_LAYER_CAPACITOR = 20
};

/* Current operational state of the battery (batteryChargingOperState) */
enum BatteryOperState {
    BATTERY_OPER_STATE_UNKNOWN = 1,
    BATTERY_OPER_STATE_CHARGING = 2,
    BATTERY_OPER_STATE_MAINTAINING_CHARGE = 3, /* e.g., Float/Trickle charging */
    BATTERY_OPER_STATE_NO_CHARGING = 4,
    BATTERY_OPER_STATE_DISCHARGING = 5
};

/* Desired administrative state of the battery (batteryChargingAdminState) */
enum BatteryAdminState {
    BATTERY_ADMIN_STATE_NOT_SET = 1,
    BATTERY_ADMIN_STATE_CHARGE = 2,
    BATTERY_ADMIN_STATE_DO_NOT_CHARGE = 3,
    BATTERY_ADMIN_STATE_DISCHARGE = 4
};

/* Represents a single battery unit (e.g., a laptop battery via sysfs) */
typedef struct BatteryEntry {
    uint32_t index;                 /* Index in the SNMP table */
    char identifier[64];            /* Combined Manufacturer/Model string */
    char cell_identifier[64];       /* Serial number or physical location ID */
    char fw_version[64];            /* Firmware version of the battery controller */
    
    enum BatteryType type;
    enum BatteryTechnology technology;
    enum BatteryOperState oper_state;
    enum BatteryAdminState admin_state;
    
    /* Hardware specifications and current state metrics */
    uint32_t design_voltage;           /* Nominal voltage (millivolts) */
    uint32_t design_capacity;          /* Nominal capacity (milliampere hours) */
    uint32_t actual_capacity;          /* Current maximum capacity due to wear (milliampere hours) */
    uint32_t actual_charge;            /* Current remaining charge (milliampere hours) */
    uint32_t actual_voltage;           /* Current voltage output (millivolts) */
    int32_t  actual_current;           /* Current draw/charge rate (+/- milliamperes) */
    int32_t  temperature;              /* Ambient temperature (deci-degrees Celsius) */
    uint32_t num_cells;                /* Number of physical cells in the pack */
    uint32_t max_charging_current;     /* Maximum allowed charging current (milliamperes) */
    uint32_t trickle_charging_current; /* Float charging current (milliamperes) */
    uint32_t charging_cycle_count;     /* Total number of full discharge/recharge cycles */
    uint64_t last_charging_cycle_time; /* Unix timestamp of the last charge cycle */
    
    struct BatteryEntry *next;         /* Pointer to the next battery in the linked list */
} BatteryEntry;


/* 
 * =============================================================================
 * UPS-MIB (RFC 1628) Data Structures and Enumerations
 * =============================================================================
 */

/* General state of the UPS (upsBatteryStatus) */
enum UPSStatus {
    UPS_STATUS_UNKNOWN = 1,
    UPS_STATUS_BATTERY_NORMAL = 2,  /* Operating normally on mains, or battery is healthy */
    UPS_STATUS_BATTERY_LOW = 3,     /* Battery capacity has dropped below safe threshold */
    UPS_STATUS_BATTERY_DEPLETED = 4 /* Battery is empty and cannot supply power */
};

/* Power source currently supplying the load (upsOutputSource) */
enum UPSOutputSource {
    UPS_OUTPUT_SOURCE_OTHER = 1,
    UPS_OUTPUT_SOURCE_NONE = 2,     /* UPS is off or supplying no power */
    UPS_OUTPUT_SOURCE_NORMAL = 3,   /* Supplying from utility/mains power */
    UPS_OUTPUT_SOURCE_BYPASS = 4,   /* Supplying from bypass (utility bypassing inverter) */
    UPS_OUTPUT_SOURCE_BATTERY = 5,  /* Supplying from battery via inverter */
    UPS_OUTPUT_SOURCE_BOOSTER = 6,  /* Voltage boost (AVR) active */
    UPS_OUTPUT_SOURCE_REDUCER = 7   /* Voltage reduction (AVR/trim) active */
};

/* Results of the last requested UPS diagnostic test (upsTestResultsSummary) */
enum UPSTestResults {
    UPS_TEST_RESULTS_DONE_PASS = 1,
    UPS_TEST_RESULTS_DONE_WARNING = 2,
    UPS_TEST_RESULTS_DONE_ERROR = 3,
    UPS_TEST_RESULTS_ABORTED = 4,
    UPS_TEST_RESULTS_IN_PROGRESS = 5,
    UPS_TEST_RESULTS_NO_TESTS_INITIATED = 6
};

/* 
 * Represents the complete state of a Uninterruptible Power Supply.
 * Usually populated by querying an external daemon like NUT (Network UPS Tools).
 * NOTE: Values are scaled to integers (e.g., 0.1 Volts, 0.1 Amps, 0.1 Hertz) 
 * as required by the UPS-MIB definitions.
 */
typedef struct UPSEntry {
    /* Identification Information */
    char manufacturer[64];
    char model[64];
    char fw_version[64];
    char fw_version_aux[64];
    char ident[64];                 /* Admin-assigned name (upsIdentName) */
    char attached_devices[64];      /* Description of what the UPS is powering */
    
    /* Battery State */
    enum UPSStatus status;
    uint32_t seconds_on_battery;    /* Time elapsed since switching to battery power */
    uint32_t minutes_remaining;     /* Estimated runtime left on battery */
    uint32_t charge_remaining;      /* Battery charge percentage (0-100) */
    uint32_t voltage;               /* Battery voltage (0.1 Volt DC) */
    uint32_t current;               /* Battery current (0.1 Amp DC) */
    uint32_t temperature;           /* Ambient battery temperature (degrees Celsius) */
    
    /* Input (Utility/Mains) State */
    uint32_t num_line_bad;          /* Counter of times the input power went out of bounds */
    uint32_t num_lines;             /* Number of input phases/lines (usually 1 or 3) */
    uint32_t input_freq;            /* Current input frequency (0.1 Hertz) */
    uint32_t input_voltage;         /* Current input voltage (Volts RMS) */
    uint32_t input_current;         /* Current input draw (0.1 Amps) */
    uint32_t input_power;           /* True power drawn by the UPS (Watts) */
    
    /* Output State */
    enum UPSOutputSource output_source;
    uint32_t output_freq;           /* Current output frequency (0.1 Hertz) */
    uint32_t output_num_lines;      /* Number of output phases/lines */
    uint32_t output_voltage;        /* Current output voltage (Volts RMS) */
    uint32_t output_current;        /* Current output draw (0.1 Amps) */
    uint32_t output_power;          /* True power supplied to load (Watts) */
    uint32_t output_load;           /* Percentage of total capacity used (0-100) */
    
    /* Bypass State (If the UPS supports hardware bypass) */
    uint32_t bypass_freq;           /* Frequency of the bypass line (0.1 Hertz) */
    uint32_t bypass_num_lines;      /* Number of bypass phases/lines */
    uint32_t bypass_voltage;        /* Voltage of the bypass line (Volts RMS) */
    uint32_t bypass_current;        /* Current drawn from the bypass line (0.1 Amps) */
    uint32_t bypass_power;          /* True power drawn from the bypass line (Watts) */
    
    /* Diagnostics / Test State */
    enum UPSTestResults test_status;
    char test_result[64];           /* Human-readable diagnostic test result (upsTestResultsDetail) */
    uint32_t test_start_time;       /* Timestamp of when the last test was initiated */
    uint32_t test_elapsed_time;     /* Duration of the last test (seconds) */
    
    /* Configuration / Control Variables */
    uint32_t shutdown_delay;        /* Seconds to wait before shutting off output (upsShutdownAfterDelay) */
    uint32_t startup_delay;         /* Seconds to wait before turning output on (upsStartupAfterDelay) */
    uint32_t reboot_duration;       /* Seconds to wait between off and on during a reboot (upsRebootWithDuration) */
    uint32_t auto_restart;          /* TruthValue: 1=True, 2=False. Whether UPS turns on when mains return */
    uint32_t config_input_voltage;  /* Nominal/Expected input voltage (Volts RMS) */
    uint32_t config_input_freq;     /* Nominal/Expected input frequency (0.1 Hertz) */
    uint32_t config_output_voltage; /* Nominal/Expected output voltage (Volts RMS) */
    uint32_t config_output_freq;    /* Nominal/Expected output frequency (0.1 Hertz) */
    uint32_t config_output_va;      /* Nominal output capacity in Volt-Amps */
    uint32_t config_output_power;   /* Nominal output capacity in Watts */
    uint32_t config_low_batt_time;  /* Threshold in minutes for firing a Low Battery warning */
    uint32_t config_beeper;         /* TruthValue: 1=True/On, 2=False/Muted. Alarm state */
    uint32_t config_low_voltage_transfer;  /* Voltage threshold to switch to battery (Brownout) */
    uint32_t config_high_voltage_transfer; /* Voltage threshold to switch to battery (Overvoltage) */
} UPSEntry;

/*
 * =============================================================================
 * Function Declarations
 * =============================================================================
 */

/**
 * @internal
 * get_battery_list - Fetches the cached linked list of battery entries.
 *
 * Checks if the cache is still valid. If it has expired, triggers an 
 * internal update by parsing sysfs data.
 *
 * @return list of battery entries, or NULL if none are available or an error occurred.
 */
BatteryEntry *get_battery_list(void);

/**
 * @internal
 * get_ups_info - Fetches the cached UPS information.
 *
 * Checks if the cache is still valid. If it has expired, triggers an 
 * internal update by connecting to the local NUT (upsd) socket.
 *
 * @return UPS status info struct, or NULL if a UPS is not available or an error occurred.
 */
UPSEntry *get_ups_info(void);

#endif /* SRC_SNMP_MIB_SYSTEM_POWER_CACHE_H_ */
