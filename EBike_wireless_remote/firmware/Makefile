PROJECT_NAME     := TSDZ2_wireless_remote
TARGETS          := TSDZ2_wireless_remote

OUTPUT_DIRECTORY := _build

SDK_ROOT := ./nRF5_SDK_16.0.0
PROJ_DIR := .
SD_REQ := 0xB9 #s340 sd is required


$(OUTPUT_DIRECTORY)/TSDZ2_wireless_remote.out: \
  LINKER_SCRIPT  := TSDZ2_wireless_gcc_nrf52.ld

BOOTLOADER_DIR = $(PROJ_DIR)/bootloader/firmware
BOOTLOADER_HEX = $(BOOTLOADER_DIR)/_build/TSDZ2_bootloader_with_sd.hex



APP_VER_MAJOR = 1
APP_VER_MINOR = 0
# Source files common to all targets
SRC_FILES += \
  $(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf52840.S \
  $(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_clock.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_clock.c \
  $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_rtt.c \
  $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_serial.c \
  $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_uart.c \
  $(SDK_ROOT)/components/libraries/log/src/nrf_log_default_backends.c \
  $(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c \
  $(SDK_ROOT)/components/libraries/log/src/nrf_log_str_formatter.c \
  $(SDK_ROOT)/modules/nrfx/mdk/system_nrf52840.c \
  $(SDK_ROOT)/components/libraries/button/app_button.c \
  $(SDK_ROOT)/components/libraries/util/app_error.c \
  $(SDK_ROOT)/components/libraries/util/app_error_handler_gcc.c \
  $(SDK_ROOT)/components/libraries/util/app_error_weak.c \
  $(SDK_ROOT)/components/libraries/scheduler/app_scheduler.c \
  $(SDK_ROOT)/components/libraries/timer/app_timer2.c \
  $(SDK_ROOT)/components/libraries/util/app_util_platform.c \
  $(SDK_ROOT)/components/libraries/timer/drv_rtc.c \
  $(SDK_ROOT)/components/libraries/hardfault/nrf52/handler/hardfault_handler_gcc.c \
  $(SDK_ROOT)/components/libraries/hardfault/hardfault_implementation.c \
  $(SDK_ROOT)/components/libraries/util/nrf_assert.c \
  $(SDK_ROOT)/components/libraries/atomic_fifo/nrf_atfifo.c \
  $(SDK_ROOT)/components/libraries/atomic/nrf_atomic.c \
  $(SDK_ROOT)/components/libraries/balloc/nrf_balloc.c \
  $(SDK_ROOT)/components/libraries/low_power_pwm/low_power_pwm.c \
  $(SDK_ROOT)/external/fprintf/nrf_fprintf.c \
  $(SDK_ROOT)/external/fprintf/nrf_fprintf_format.c \
  $(SDK_ROOT)/components/libraries/memobj/nrf_memobj.c \
  $(SDK_ROOT)/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c \
  $(SDK_ROOT)/components/libraries/ringbuf/nrf_ringbuf.c \
  $(SDK_ROOT)/components/libraries/experimental_section_vars/nrf_section_iter.c \
  $(SDK_ROOT)/components/libraries/sortlist/nrf_sortlist.c \
  $(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c \
  $(SDK_ROOT)/components/libraries/sensorsim/sensorsim.c \
  $(SDK_ROOT)/modules/nrfx/soc/nrfx_atomic.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_gpiote.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/prs/nrfx_prs.c \
  $(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c \
  $(SDK_ROOT)/components/libraries/sensorsim/sensorsim.c \
  $(SDK_ROOT)/modules/nrfx/soc/nrfx_atomic.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_gpiote.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/prs/nrfx_prs.c \
	$(SDK_ROOT)/modules/nrfx/hal/nrf_nvmc.c \
  $(SDK_ROOT)/components/libraries/bsp/bsp.c \
  $(SDK_ROOT)/components/libraries/led_softblink/led_softblink.c \
  $(SDK_ROOT)/external/segger_rtt/SEGGER_RTT.c \
  $(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c \
  $(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_printf.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh_ant.c \
	$(SDK_ROOT)/components/libraries/fifo/app_fifo.c \
	$(SDK_ROOT)/components/libraries/atomic_flags/nrf_atflags.c \
	$(SDK_ROOT)/components/libraries/fstorage/nrf_fstorage.c \
	$(SDK_ROOT)/components/libraries/fstorage/nrf_fstorage_sd.c \
	$(SDK_ROOT)/components/libraries/fds/fds.c \
	$(SDK_ROOT)/components/ble/common/ble_advdata.c \
  $(SDK_ROOT)/components/ble/common/ble_conn_params.c \
  $(SDK_ROOT)/components/ble/common/ble_conn_state.c \
  $(SDK_ROOT)/components/ble/common/ble_srv_common.c \
  $(SDK_ROOT)/components/ble/nrf_ble_gatt/nrf_ble_gatt.c \
  $(SDK_ROOT)/components/ble/nrf_ble_qwr/nrf_ble_qwr.c \
  $(SDK_ROOT)/components/ble/peer_manager/gatt_cache_manager.c \
  $(SDK_ROOT)/components/ble/peer_manager/gatts_cache_manager.c \
  $(SDK_ROOT)/components/ble/peer_manager/id_manager.c \
  $(SDK_ROOT)/components/ble/nrf_ble_gq/nrf_ble_gq.c \
  $(SDK_ROOT)/components/ble/peer_manager/peer_data_storage.c \
  $(SDK_ROOT)/components/ble/peer_manager/peer_database.c \
  $(SDK_ROOT)/components/ble/peer_manager/peer_id.c \
  $(SDK_ROOT)/components/ble/peer_manager/peer_manager.c \
  $(SDK_ROOT)/components/ble/peer_manager/peer_manager_handler.c \
  $(SDK_ROOT)/components/ble/peer_manager/pm_buffer.c \
  $(SDK_ROOT)/components/ble/peer_manager/security_dispatcher.c \
  $(SDK_ROOT)/components/ble/peer_manager/security_manager.c \
	$(SDK_ROOT)/components/ble/common/ble_advdata.c \
  $(SDK_ROOT)/components/ble/ble_advertising/ble_advertising.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_clock.c \
  $(SDK_ROOT)/components/libraries/bsp/bsp.c \
  $(SDK_ROOT)/external/segger_rtt/SEGGER_RTT.c \
  $(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c \
  $(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_printf.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh_ant.c \
  $(SDK_ROOT)/components/ant/ant_profiles/ant_common/ant_request_controller/ant_request_controller.c \
  $(SDK_ROOT)/components/ant/ant_channel_config/ant_channel_config.c \
  $(SDK_ROOT)/components/ant/ant_key_manager/ant_key_manager.c \
  $(SDK_ROOT)/components/ant/ant_state_indicator/ant_state_indicator.c \
  $(SDK_ROOT)/components/ant/ant_profiles/ant_common/pages/ant_common_page_70.c \
  $(SDK_ROOT)/components/ant/ant_profiles/ant_common/pages/ant_common_page_80.c \
  $(SDK_ROOT)/components/ant/ant_profiles/ant_common/pages/ant_common_page_81.c \
  $(SDK_ROOT)/components/ant/ant_search_config/ant_search_config.c \
  $(PROJ_DIR)/antplus_controls/antplus_controls.c \
  $(PROJ_DIR)/antplus_controls/pages/antplus_controls_page_73.c \
  $(PROJ_DIR)/antplus_controls/pages/antplus_controls_page_82.c \
  $(PROJ_DIR)/ant_lev/ant_lev.c \
  $(PROJ_DIR)/ant_lev/pages/ant_lev_page_1.c \
  $(PROJ_DIR)/ant_lev/pages/ant_lev_page_2.c \
  $(PROJ_DIR)/ant_lev/pages/ant_lev_page_3.c \
  $(PROJ_DIR)/ant_lev/pages/ant_lev_page_4.c \
  $(PROJ_DIR)/ant_lev/pages/ant_lev_page_5.c \
  $(PROJ_DIR)/ant_lev/pages/ant_lev_page_34.c \
  $(PROJ_DIR)/ant_lev/pages/ant_lev_page_16.c \
  $(PROJ_DIR)/ant_lev/pages/ant_lev_common_data.c \
  $(PROJ_DIR)/main.c \
  $(PROJ_DIR)/eeprom.c \
  $(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_clock.c \
  $(SDK_ROOT)/components/libraries/util/app_util_platform.c \
  $(SDK_ROOT)/components/boards/boards.c \
  $(PROJ_DIR)/ble_services.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh_ble.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh_soc.c \

# Include folders common to all targets
INC_FOLDERS += \
  $(SDK_ROOT)/ \
  $(SDK_ROOT)/components/libraries/sensorsim \
  $(SDK_ROOT)/external/fprintf \
  $(SDK_ROOT)/components/softdevice/s340/headers \
  $(SDK_ROOT)/components/softdevice/s340/headers/nrf52 \
  $(SDK_ROOT)/components/libraries/hardfault/nrf52 \
  $(SDK_ROOT)/integration/nrfx \
  $(SDK_ROOT)/components/libraries/scheduler \
  $(SDK_ROOT)/components/libraries/experimental_section_vars \
  $(SDK_ROOT)/components/libraries/atomic_fifo \
  $(SDK_ROOT)/components/libraries/fifo \
	$(SDK_ROOT)/components/libraries/fstorage \
	$(SDK_ROOT)/components/libraries/fds \
  $(SDK_ROOT)/modules/nrfx/mdk \
  $(SDK_ROOT)/components/libraries/strerror \
  $(SDK_ROOT)/integration/nrfx/legacy \
  $(SDK_ROOT)/components/boards \
  $(SDK_ROOT)/components/softdevice/s340/headers \
  $(SDK_ROOT)/components/libraries/button \
  $(SDK_ROOT)/components/libraries/log \
  $(SDK_ROOT)/modules/nrfx/hal \
  $(SDK_ROOT)/components/libraries/log/src \
  $(SDK_ROOT)/components/libraries/bsp \
  $(SDK_ROOT)/components/libraries/bootloader \
  $(SDK_ROOT)/components/libraries/sortlist \
  $(SDK_ROOT)/modules/nrfx/drivers/include \
  $(SDK_ROOT)/components/libraries/hardfault \
  $(SDK_ROOT)/components/libraries/balloc \
  $(SDK_ROOT)/components/libraries/util \
  $(SDK_ROOT)/components/libraries/svc \
  $(SDK_ROOT)/components/libraries/low_power_pwm \
  $(SDK_ROOT)/modules/nrfx \
  $(SDK_ROOT)/components/softdevice/common \
  $(SDK_ROOT)/components \
  $(SDK_ROOT)/external/segger_rtt \
  $(SDK_ROOT)/components/libraries/ringbuf \
  $(SDK_ROOT)/components/libraries/led_softblink \
  $(SDK_ROOT)/components/libraries/memobj \
  $(SDK_ROOT)/components/libraries/atomic \
  $(SDK_ROOT)/components/libraries/delay \
  $(SDK_ROOT)/components/toolchain/cmsis/include \
  $(SDK_ROOT)/components/libraries/mutex \
  $(SDK_ROOT)/components/libraries/pwr_mgmt \
  $(SDK_ROOT)/components/libraries/timer \
  $(SDK_ROOT)/components/libraries/atomic_flags \
	$(SDK_ROOT)/components/ble/common \
	$(SDK_ROOT)/components/ble/ble_services/ble_ancs_c \
  $(SDK_ROOT)/components/ble/ble_services/ble_ias_c \
	$(SDK_ROOT)/components/ble/ble_services/ble_gls \
	$(SDK_ROOT)/components/ble/ble_advertising \
	$(SDK_ROOT)/components/ble/ble_dtm \
	$(SDK_ROOT)/components/ble/ble_services/ble_rscs_c \
	$(SDK_ROOT)/components/ble/ble_services/ble_lls \
	$(SDK_ROOT)/components/ble/ble_services/ble_bas \
	$(SDK_ROOT)/components/ble/ble_services/ble_ans_c \
	$(SDK_ROOT)/components/ble/ble_services/ble_nus_c \
	$(SDK_ROOT)/components/nfc/ndef/conn_hand_parser/ble_oob_advdata_parser \
	$(SDK_ROOT)/components/ble/ble_services/ble_lbs \
  $(SDK_ROOT)/components/ble/ble_services/ble_hts \
  $(SDK_ROOT)/components/ble/ble_services/ble_ias \
	$(SDK_ROOT)/components/ble/ble_services/ble_cts_c \
  $(SDK_ROOT)/components/ble/ble_services/ble_nus \
  $(SDK_ROOT)/components/ble/ble_services/ble_hids \
	$(SDK_ROOT)/components/ble/peer_manager \
	$(SDK_ROOT)/components/ble/ble_services/ble_tps \
	$(SDK_ROOT)/components/ble/ble_services/ble_dis \
	$(SDK_ROOT)/components/ble/nrf_ble_gatt \
  $(SDK_ROOT)/components/ble/nrf_ble_qwr \
	$(SDK_ROOT)/components/ble/ble_services/ble_lbs_c \
	$(SDK_ROOT)/components/ble/ble_racp \
	$(SDK_ROOT)/components/ble/ble_services/ble_hrs \
  $(SDK_ROOT)/components/ble/ble_services/ble_rscs \
	$(SDK_ROOT)/components/ble/peer_manager \
	$(SDK_ROOT)/components/ble/ble_advertising \
  $(SDK_ROOT)/components/ant/ant_state_indicator \
  $(SDK_ROOT)/components/ant/ant_key_manager \
  $(SDK_ROOT)/components/ant/ant_search_config \
  $(SDK_ROOT)/components/ant/ant_key_manager/config \
  $(SDK_ROOT)/components/ant/ant_channel_config \
  $(SDK_ROOT)/components/ant/ant_profiles/ant_common/ant_request_controller \
  $(SDK_ROOT)/components/ant/ant_profiles/ant_common/pages \
  $(PROJ_DIR)/ant_lev \
  $(PROJ_DIR)/ant_lev/pages \
  $(PROJ_DIR)/antplus_controls \
  $(PROJ_DIR)/antplus_controls/pages \
  $(PROJ_DIR)/config \
  $(PROJ_DIR)/ \
  $(PROJ_DIR)/include \
  $(SDK_ROOT)/integration/nrfx/legacy \
  

# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -O0 -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DAPP_TIMER_V2
CFLAGS += -DAPP_TIMER_V2_RTC1_ENABLED
#CFLAGS += -DBOARD_CUSTOM -DNRF52840_MDK_USB_DONGLE
CFLAGS += -DBOARD_PCA10059
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DNRF52840_XXAA
CFLAGS += -DNRF_SD_BLE_API_VERSION=6
CFLAGS += -DS340
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -BL_SETTINGS_ACCESS_ONLY
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -DCONFIG_NFCT_PINS_AS_GPIOS 
#CFLAGS += -Wall -Werror
CFLAGS += -Wall
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums


# C++ flags common to all targets
CXXFLAGS += $(OPT)
# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DAPP_TIMER_V2
ASMFLAGS += -DAPP_TIMER_V2_RTC1_ENABLED
#ASMFLAGS += -DBOARD_CUSTOM -DNRF52840_MDK_USB_DONGLE
ASMFLAGS += -DBOARD_PCA10059
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -DNRF52840_XXAA
ASMFLAGS += -DNRF_SD_BLE_API_VERSION=6
ASMFLAGS += -DS340
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DCONFIG_NFCT_PINS_AS_GPIOS 


# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

TSDZ2_wireless_remote: CFLAGS += -D__HEAP_SIZE=8192
TSDZ2_wireless_remote: CFLAGS += -D__STACK_SIZE=8192
TSDZ2_wireless_remote: ASMFLAGS += -D__HEAP_SIZE=8192
TSDZ2_wireless_remote: ASMFLAGS += -D__STACK_SIZE=8192

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default help


# Default target - first one defined
default: TSDZ2_wireless_remote bl_settings final OTA

final:
	srec_cat $(OUTPUT_DIRECTORY)/TSDZ2_wireless_remote.hex -intel $(OUTPUT_DIRECTORY)/bl_settings.hex -intel $(BOOTLOADER_HEX) -intel -o $(OUTPUT_DIRECTORY)/TSDZ2_wireless_remote_combined.hex 

bl_settings:
	$(SREC_PATH)nrfutil settings generate --family NRF52840 --application $(OUTPUT_DIRECTORY)/TSDZ2_wireless_remote.hex --application-version-string $(APP_VER_MAJOR).$(APP_VER_MINOR).0 --bootloader-version 10 --bl-settings-version 2 $(OUTPUT_DIRECTORY)/bl_settings.hex 

OTA:
	$(SREC_PATH)nrfutil pkg generate --hw-version 52 --sd-req $(SD_REQ) --application-version-string $(APP_VER_MAJOR).$(APP_VER_MINOR).0 --application $(OUTPUT_DIRECTORY)/TSDZ2_wireless_remote.hex --key-file $(PROJ_DIR)/include/private.key --app-boot-validation VALIDATE_ECDSA_P256_SHA256 $(OUTPUT_DIRECTORY)/TSDZ2_wireless_remote_ota_update.zip


TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc

include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash erase

# Flash the program
flash: default
	@echo Flashing: $(OUTPUT_DIRECTORY)/TSDZ2_WIRELESS_REMOTE.hex
	nrfjprog -f nrf52 --program $(OUTPUT_DIRECTORY)/TSDZ2_WIRELESS_REMOTE.hex --sectorerase
	nrfjprog -f nrf52 --reset

erase:
	nrfjprog -f nrf52 --eraseall
SDK_CONFIG_FILE := .$(PROJ_DIR)/config/sdk_config.h
CMSIS_CONFIG_TOOL := $(SDK_ROOT)/external_tools/cmsisconfig/CMSIS_Configuration_Wizard.jar
sdk_config:
	java -jar $(CMSIS_CONFIG_TOOL) $(SDK_CONFIG_FILE)
