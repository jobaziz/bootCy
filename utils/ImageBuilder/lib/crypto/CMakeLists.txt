add_library(cyclone_crypto "")

target_include_directories(cyclone_crypto
    PUBLIC
        ${PROJECT_SOURCE_DIR}/lib/crypto
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/common
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/core
        ${PROJECT_SOURCE_DIR}/lib/crypto/config
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/
)

# add the main library
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    message("On Linux")
    target_sources(cyclone_crypto
            PUBLIC
            ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/common/os_port_posix.h
            ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/common/os_port_posix.c
    )
endif()
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    message("On Windows")
    target_sources(cyclone_crypto
            PUBLIC
           ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/common/os_port_windows.h
           ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/common/os_port_windows.c
    )
endif()
target_sources(cyclone_crypto
    PUBLIC
        ${PROJECT_SOURCE_DIR}/lib/crypto/crc32.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/crc32.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/config/os_port_config.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/config/crypto_config.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/common/os_port.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/common/cpu_endian.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/common/cpu_endian.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/common/date_time.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/common/date_time.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/hash/sha512.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/hash/sha512.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/hash/sha256.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/hash/sha256.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/hash/sha224.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/hash/sha224.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/hash/sha384.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/hash/sha384.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/hash/sha1.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/hash/sha1.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/hash/md5.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/hash/md5.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/rng/yarrow.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/rng/yarrow.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/cipher/aes.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/cipher/aes.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/cipher_mode/cbc.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/cipher_mode/cbc.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/mac/hmac.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/mac/hmac.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/pkc/rsa.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/pkc/rsa.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/pkc/dsa.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/pkc/dsa.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/encoding/oid.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/encoding/oid.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/encoding/asn1.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/encoding/asn1.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/encoding/base64.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/encoding/base64.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/mpi/mpi.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/mpi/mpi.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/pkix/x509_common.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/pkix/x509_common.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/pkix/x509_key_parse.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/pkix/x509_key_parse.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/pkix/pem_import.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/pkix/pem_import.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/pkix/pkcs8_key_parse.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/pkix/pkcs8_key_parse.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/ecc/ec_curves.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/ecc/ec_curves.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/ecc/ecdsa.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/ecc/ecdsa.h
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/ecc/ec.c
        ${PROJECT_SOURCE_DIR}/lib/crypto/cyclone_crypto/ecc/ec.h
 )