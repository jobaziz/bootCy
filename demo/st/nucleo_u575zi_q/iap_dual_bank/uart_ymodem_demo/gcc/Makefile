RESULT ?= iap_dual_bank_uart_ymodem_demo

DEFINES = \
	-DSTM32U575xx \
	-DUSE_HAL_DRIVER \
	-DUSE_STM32U5XX_NUCLEO \
	-DFLASH_DB_MODE \
	-D__USE_C99_MATH \
	-D_WINSOCK_H \
	-D__error_t_defined

INCLUDES = \
	-I../src \
	-I../../../../../../third_party/cmsis/include \
	-I../../../../../../third_party/st/devices/stm32u5xx \
	-I../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc \
	-I../../../../../../third_party/st/boards/stm32u5xx_nucleo \
	-I../../../../../../third_party/freertos/include \
	-I../../../../../../third_party/freertos/portable/gcc/arm_cm33_ntz/non_secure \
	-I../../../../../../common \
	-I../../../../../../cyclone_crypto \
	-I../../../../../../cyclone_boot

ASM_SOURCES = \
	./startup_stm32u575xx.S

C_SOURCES = \
	../src/system_stm32u5xx.c \
	../src/stm32u5xx_it.c \
	../src/stm32u5xx_hal_timebase_tim.c \
	./syscalls.c \
	../src/main.c \
	../src/debug.c \
	../src/res.c \
	../src/uart_user.c \
	../src/common.c \
	../src/ymodem.c \
	../../../../../../common/cpu_endian.c \
	../../../../../../common/os_port_freertos.c \
	../../../../../../common/date_time.c \
	../../../../../../common/str.c \
	../../../../../../common/path.c \
	../../../../../../common/resource_manager.c \
	../../../../../../cyclone_boot/core/crc32.c \
	../../../../../../cyclone_boot/drivers/mcu/arm/stm32u5xx_mcu_driver.c \
	../../../../../../cyclone_boot/drivers/memory/flash/internal/stm32u5xx_flash_driver.c \
	../../../../../../cyclone_boot/image/image.c \
	../../../../../../cyclone_boot/image/image_process.c \
	../../../../../../cyclone_boot/image/image_utils.c \
	../../../../../../cyclone_boot/memory/memory.c \
	../../../../../../cyclone_boot/memory/memory_ex.c \
	../../../../../../cyclone_boot/security/verify.c \
	../../../../../../cyclone_boot/security/verify_auth.c \
	../../../../../../cyclone_boot/security/verify_sign.c \
	../../../../../../cyclone_boot/security/cipher.c \
	../../../../../../cyclone_boot/update/update.c \
	../../../../../../cyclone_boot/update/update_misc.c \
	../../../../../../cyclone_boot/update/update_fallback.c \
	../../../../../../cyclone_crypto/hardware/stm32u5xx/stm32u5xx_crypto.c \
	../../../../../../cyclone_crypto/hardware/stm32u5xx/stm32u5xx_crypto_trng.c \
	../../../../../../cyclone_crypto/hash/md5.c \
	../../../../../../cyclone_crypto/hash/sha1.c \
	../../../../../../cyclone_crypto/hash/sha224.c \
	../../../../../../cyclone_crypto/hash/sha256.c \
	../../../../../../cyclone_crypto/hash/sha384.c \
	../../../../../../cyclone_crypto/hash/sha512.c \
	../../../../../../cyclone_crypto/mac/hmac.c \
	../../../../../../cyclone_crypto/cipher/rc4.c \
	../../../../../../cyclone_crypto/cipher/idea.c \
	../../../../../../cyclone_crypto/cipher/des.c \
	../../../../../../cyclone_crypto/cipher/des3.c \
	../../../../../../cyclone_crypto/cipher/aes.c \
	../../../../../../cyclone_crypto/cipher/camellia.c \
	../../../../../../cyclone_crypto/cipher/seed.c \
	../../../../../../cyclone_crypto/cipher/aria.c \
	../../../../../../cyclone_crypto/cipher_modes/cbc.c \
	../../../../../../cyclone_crypto/aead/ccm.c \
	../../../../../../cyclone_crypto/aead/gcm.c \
	../../../../../../cyclone_crypto/cipher/chacha.c \
	../../../../../../cyclone_crypto/mac/poly1305.c \
	../../../../../../cyclone_crypto/aead/chacha20_poly1305.c \
	../../../../../../cyclone_crypto/pkc/dh.c \
	../../../../../../cyclone_crypto/pkc/rsa.c \
	../../../../../../cyclone_crypto/pkc/dsa.c \
	../../../../../../cyclone_crypto/ecc/ec.c \
	../../../../../../cyclone_crypto/ecc/ec_curves.c \
	../../../../../../cyclone_crypto/ecc/ecdh.c \
	../../../../../../cyclone_crypto/ecc/ecdsa.c \
	../../../../../../cyclone_crypto/ecc/eddsa.c \
	../../../../../../cyclone_crypto/ecc/curve25519.c \
	../../../../../../cyclone_crypto/ecc/curve448.c \
	../../../../../../cyclone_crypto/ecc/x25519.c \
	../../../../../../cyclone_crypto/ecc/x448.c \
	../../../../../../cyclone_crypto/ecc/ed25519.c \
	../../../../../../cyclone_crypto/ecc/ed448.c \
	../../../../../../cyclone_crypto/mpi/mpi.c \
	../../../../../../cyclone_crypto/encoding/base64.c \
	../../../../../../cyclone_crypto/encoding/asn1.c \
	../../../../../../cyclone_crypto/encoding/oid.c \
	../../../../../../cyclone_crypto/pkix/pem_import.c \
	../../../../../../cyclone_crypto/pkix/pem_decrypt.c \
	../../../../../../cyclone_crypto/pkix/pem_common.c \
	../../../../../../cyclone_crypto/pkix/pkcs5_decrypt.c \
	../../../../../../cyclone_crypto/pkix/pkcs5_common.c \
	../../../../../../cyclone_crypto/pkix/pkcs8_key_parse.c \
	../../../../../../cyclone_crypto/pkix/x509_key_parse.c \
	../../../../../../cyclone_crypto/pkix/x509_cert_parse.c \
	../../../../../../cyclone_crypto/pkix/x509_cert_validate.c \
	../../../../../../cyclone_crypto/pkix/x509_cert_ext_parse.c \
	../../../../../../cyclone_crypto/pkix/x509_common.c \
	../../../../../../cyclone_crypto/pkix/x509_sign_parse.c \
	../../../../../../cyclone_crypto/pkix/x509_sign_verify.c \
	../../../../../../cyclone_crypto/kdf/hkdf.c \
	../../../../../../cyclone_crypto/rng/yarrow.c \
	../../../../../../third_party/freertos/portable/gcc/arm_cm33_ntz/non_secure/port.c \
	../../../../../../third_party/freertos/portable/gcc/arm_cm33_ntz/non_secure/portasm.c \
	../../../../../../third_party/freertos/croutine.c \
	../../../../../../third_party/freertos/list.c \
	../../../../../../third_party/freertos/queue.c \
	../../../../../../third_party/freertos/tasks.c \
	../../../../../../third_party/freertos/timers.c \
	../../../../../../third_party/freertos/portable/memmang/heap_3.c \
	../../../../../../third_party/st/boards/stm32u5xx_nucleo/stm32u5xx_nucleo.c \
	../../../../../../third_party/st/boards/stm32u5xx_nucleo/stm32u5xx_nucleo_usbpd_pwr.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_adc.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_adc_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_comp.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_cordic.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_cortex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_crc.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_crc_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_cryp.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_cryp_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_dac.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_dac_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_dcache.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_dcmi.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_dma.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_dma2d.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_dma_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_exti.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_fdcan.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_flash.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_flash_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_fmac.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_gpio.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_gtzc.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_hash.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_hash_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_hcd.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_i2c.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_i2c_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_icache.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_irda.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_iwdg.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_lptim.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_mdf.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_mmc.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_mmc_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_nand.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_nor.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_opamp.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_opamp_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_ospi.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_otfdec.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_pcd.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_pcd_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_pka.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_pssi.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_pwr.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_pwr_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_ramcfg.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_rcc.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_rcc_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_rng.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_rng_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_rtc.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_rtc_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_sai.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_sai_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_sd.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_sd_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_smartcard.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_smartcard_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_smbus.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_smbus_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_spi.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_spi_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_sram.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_tim.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_tim_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_tsc.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_uart.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_uart_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_usart.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_usart_ex.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_hal_wwdg.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_adc.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_comp.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_cordic.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_crc.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_crs.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_dac.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_dlyb.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_dma.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_dma2d.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_exti.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_fmac.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_fmc.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_gpio.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_i2c.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_icache.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_lpgpio.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_lptim.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_lpuart.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_opamp.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_pka.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_pwr.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_rcc.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_rng.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_rtc.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_sdmmc.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_spi.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_tim.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_ucpd.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_usart.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_usb.c \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/src/stm32u5xx_ll_utils.c

HEADERS = \
	../src/os_port_config.h \
	../src/net_config.h \
	../src/FreeRTOSConfig.h \
	../src/stm32u5xx_hal_conf.h \
	../src/stm32u5xx_nucleo_conf.h \
	../src/stm32u5xx_it.h \
	../src/uart_user.h \
	../src/boot_config.h \
	../src/crypto_config.h \
	../src/uart_user.h \
	../src/common.h \
	../src/ymodem.h \
	../src/version.h \
	../../../../../../common/cpu_endian.h \
	../../../../../../common/os_port.h \
	../../../../../../common/os_port_freertos.h \
	../../../../../../common/date_time.h \
	../../../../../../common/str.h \
	../../../../../../common/path.h \
	../../../../../../common/resource_manager.h \
	../../../../../../common/error.h \
	../../../../../../common/debug.h \
	../../../../../../cyclone_boot/core/cboot_error.h \
	../../../../../../cyclone_boot/core/crc32.h \
	../../../../../../cyclone_boot/core/flash.h \
	../../../../../../cyclone_boot/core/mcu.h \
	../../../../../../cyclone_boot/drivers/mcu/arm/stm32u5xx_mcu_driver.h \
	../../../../../../cyclone_boot/drivers/memory/flash/internal/stm32u5xx_flash_driver.h \
	../../../../../../cyclone_boot/image/image.h \
	../../../../../../cyclone_boot/image/image_process.h \
	../../../../../../cyclone_boot/image/image_utils.h \
	../../../../../../cyclone_boot/memory/memory.h \
	../../../../../../cyclone_boot/memory/memory_ex.h \
	../../../../../../cyclone_boot/security/verify.h \
	../../../../../../cyclone_boot/security/verify_auth.h \
	../../../../../../cyclone_boot/security/verify_sign.h \
	../../../../../../cyclone_boot/security/cipher.h \
	../../../../../../cyclone_boot/update/update.h \
	../../../../../../cyclone_boot/update/update_misc.h \
	../../../../../../cyclone_boot/update/update_fallback.h \
	../../../../../../cyclone_crypto/core/crypto.h \
	../../../../../../cyclone_crypto/hardware/stm32u5xx/stm32u5xx_crypto.h \
	../../../../../../cyclone_crypto/hardware/stm32u5xx/stm32u5xx_crypto_trng.h \
	../../../../../../cyclone_crypto/hash/md5.h \
	../../../../../../cyclone_crypto/hash/sha1.h \
	../../../../../../cyclone_crypto/hash/sha224.h \
	../../../../../../cyclone_crypto/hash/sha256.h \
	../../../../../../cyclone_crypto/hash/sha384.h \
	../../../../../../cyclone_crypto/hash/sha512.h \
	../../../../../../cyclone_crypto/mac/hmac.h \
	../../../../../../cyclone_crypto/cipher/rc4.h \
	../../../../../../cyclone_crypto/cipher/idea.h \
	../../../../../../cyclone_crypto/cipher/des.h \
	../../../../../../cyclone_crypto/cipher/des3.h \
	../../../../../../cyclone_crypto/cipher/aes.h \
	../../../../../../cyclone_crypto/cipher/camellia.h \
	../../../../../../cyclone_crypto/cipher/seed.h \
	../../../../../../cyclone_crypto/cipher/aria.h \
	../../../../../../cyclone_crypto/cipher_modes/cbc.h \
	../../../../../../cyclone_crypto/aead/ccm.h \
	../../../../../../cyclone_crypto/aead/gcm.h \
	../../../../../../cyclone_crypto/cipher/chacha.h \
	../../../../../../cyclone_crypto/mac/poly1305.h \
	../../../../../../cyclone_crypto/aead/chacha20_poly1305.h \
	../../../../../../cyclone_crypto/pkc/dh.h \
	../../../../../../cyclone_crypto/pkc/rsa.h \
	../../../../../../cyclone_crypto/pkc/dsa.h \
	../../../../../../cyclone_crypto/ecc/ec.h \
	../../../../../../cyclone_crypto/ecc/ec_curves.h \
	../../../../../../cyclone_crypto/ecc/ecdh.h \
	../../../../../../cyclone_crypto/ecc/ecdsa.h \
	../../../../../../cyclone_crypto/ecc/eddsa.h \
	../../../../../../cyclone_crypto/ecc/curve25519.h \
	../../../../../../cyclone_crypto/ecc/curve448.h \
	../../../../../../cyclone_crypto/ecc/x25519.h \
	../../../../../../cyclone_crypto/ecc/x448.h \
	../../../../../../cyclone_crypto/ecc/ed25519.h \
	../../../../../../cyclone_crypto/ecc/ed448.h \
	../../../../../../cyclone_crypto/mpi/mpi.h \
	../../../../../../cyclone_crypto/encoding/base64.h \
	../../../../../../cyclone_crypto/encoding/asn1.h \
	../../../../../../cyclone_crypto/encoding/oid.h \
	../../../../../../cyclone_crypto/pkix/pem_import.h \
	../../../../../../cyclone_crypto/pkix/pem_decrypt.h \
	../../../../../../cyclone_crypto/pkix/pem_common.h \
	../../../../../../cyclone_crypto/pkix/pkcs5_decrypt.h \
	../../../../../../cyclone_crypto/pkix/pkcs5_common.h \
	../../../../../../cyclone_crypto/pkix/pkcs8_key_parse.h \
	../../../../../../cyclone_crypto/pkix/x509_key_parse.h \
	../../../../../../cyclone_crypto/pkix/x509_cert_parse.h \
	../../../../../../cyclone_crypto/pkix/x509_cert_validate.h \
	../../../../../../cyclone_crypto/pkix/x509_cert_ext_parse.h \
	../../../../../../cyclone_crypto/pkix/x509_common.h \
	../../../../../../cyclone_crypto/pkix/x509_sign_parse.h \
	../../../../../../cyclone_crypto/pkix/x509_sign_verify.h \
	../../../../../../cyclone_crypto/kdf/hkdf.h \
	../../../../../../cyclone_crypto/rng/yarrow.h \
	../../../../../../third_party/freertos/portable/gcc/arm_cm33_ntz/non_secure/portmacro.h \
	../../../../../../third_party/freertos/portable/gcc/arm_cm33_ntz/non_secure/portasm.h \
	../../../../../../third_party/freertos/include/croutine.h \
	../../../../../../third_party/freertos/include/FreeRTOS.h \
	../../../../../../third_party/freertos/include/list.h \
	../../../../../../third_party/freertos/include/mpu_wrappers.h \
	../../../../../../third_party/freertos/include/portable.h \
	../../../../../../third_party/freertos/include/projdefs.h \
	../../../../../../third_party/freertos/include/queue.h \
	../../../../../../third_party/freertos/include/semphr.h \
	../../../../../../third_party/freertos/include/stack_macros.h \
	../../../../../../third_party/freertos/include/task.h \
	../../../../../../third_party/freertos/include/timers.h \
	../../../../../../third_party/st/boards/stm32u5xx_nucleo/stm32u5xx_nucleo.h \
	../../../../../../third_party/st/boards/stm32u5xx_nucleo/stm32u5xx_nucleo_usbpd_pwr.h \
	../../../../../../third_party/st/boards/stm32u5xx_nucleo/stm32u5xx_nucleo_errno.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_adc.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_adc_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_comp.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_cordic.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_cortex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_crc.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_crc_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_cryp.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_cryp_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_dac.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_dac_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_dcache.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_dcmi.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_def.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_dma.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_dma2d.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_dma_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_exti.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_fdcan.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_flash.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_flash_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_fmac.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_gpio.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_gpio_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_gtzc.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_hash.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_hash_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_hcd.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_i2c.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_i2c_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_icache.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_irda.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_irda_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_iwdg.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_lptim.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_mdf.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_mmc.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_mmc_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_nand.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_nor.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_opamp.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_opamp_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_ospi.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_otfdec.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_pcd.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_pcd_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_pka.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_pssi.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_pwr.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_pwr_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_ramcfg.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_rcc.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_rcc_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_rng.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_rng_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_rtc.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_rtc_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_sai.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_sai_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_sd.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_sd_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_smartcard.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_smartcard_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_smbus.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_smbus_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_spi.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_spi_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_sram.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_tim.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_tim_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_tsc.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_uart.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_uart_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_usart.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_usart_ex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_hal_wwdg.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_adc.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_bus.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_comp.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_cordic.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_cortex.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_crc.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_crs.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_dac.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_dcache.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_dlyb.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_dma.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_dma2d.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_exti.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_fmac.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_fmc.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_gpio.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_i2c.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_icache.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_iwdg.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_lpgpio.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_lptim.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_lpuart.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_opamp.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_pka.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_pwr.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_rcc.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_rng.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_rtc.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_sdmmc.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_spi.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_system.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_tim.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_ucpd.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_usart.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_usb.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_utils.h \
	../../../../../../third_party/st/drivers/stm32u5xx_hal_driver/inc/stm32u5xx_ll_wwdg.h

ASM_OBJECTS = $(patsubst %.S, %.o, $(ASM_SOURCES))

C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))

OBJ_DIR = obj

LINKER_SCRIPT = stm32u575_flash.ld

CFLAGS += -fno-common -Wall -Os -g3
CFLAGS += -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=softfp
CFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections
CFLAGS += $(DEFINES)
CFLAGS += $(INCLUDES)

CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size

THIS_MAKEFILE := $(lastword $(MAKEFILE_LIST))

all:
	$(MAKE) pre-build build size
	$(MAKE) post-build 

install:
	$(MAKE) pre-build build size
	$(MAKE) post-build
	$(MAKE) flash

pre-build:
	../../../../../../utils/ResourceCompiler/bin/rc.exe ../resources/ ../src/res.c

post-build:
	python ../scripts/misc/copy_file.py iap_dual_bank_uart_ymodem_demo.bin ../

build: $(RESULT).elf $(RESULT).lst $(RESULT).bin $(RESULT).hex
	
$(RESULT).elf: $(ASM_OBJECTS) $(C_OBJECTS) $(HEADERS) $(LINKER_SCRIPT) $(THIS_MAKEFILE)
	$(CC) -Wl,-M=$(RESULT).map -Wl,-T$(LINKER_SCRIPT) $(CFLAGS) $(addprefix $(OBJ_DIR)/, $(notdir $(ASM_OBJECTS))) $(addprefix $(OBJ_DIR)/, $(notdir $(C_OBJECTS))) -o $@

$(ASM_OBJECTS): | $(OBJ_DIR)

$(C_OBJECTS): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir -p $@

%.o: %.c $(HEADERS) $(THIS_MAKEFILE)
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(OBJ_DIR)/, $(notdir $@))

%.o: %.S $(HEADERS) $(THIS_MAKEFILE)
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(OBJ_DIR)/, $(notdir $@))

%.lst: %.elf
	$(OBJDUMP) -x -S $(RESULT).elf > $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

size: $(RESULT).elf
	$(SIZE) $(RESULT).elf

flash:
	openocd -f board/st_nucleo_u5.cfg -c "init; reset halt; flash write_image erase $(RESULT).bin 0x08000000; reset run; shutdown"

clean:
	rm -f $(RESULT).elf
	rm -f $(RESULT).bin
	rm -f $(RESULT).map
	rm -f $(RESULT).hex
	rm -f $(RESULT).lst
	rm -f $(OBJ_DIR)/*.o
