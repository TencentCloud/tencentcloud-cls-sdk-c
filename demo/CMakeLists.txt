project (cls_logcls_c_demo)

include_directories(${CMAKE_SOURCE_DIR})

set(POST_LOGS_SOURCE_FILES log_post.c)

include_directories (${CURL_INCLUDE_DIR})
include_directories ("${CMAKE_SOURCE_DIR}/src")


find_library(CURL_LIBRARY curl)
find_library(PTHREAD_LIBRARY pthread)
find_library(M_LIBRARY m)
find_library(SSL_LIBRARY ssl)
find_library(CRYPTO_LIBRARY crypto)
add_executable(post_log_demo ${POST_LOGS_SOURCE_FILES})
target_link_libraries(post_log_demo cls_log_csdk_static)
target_link_libraries(post_log_demo ${PTHREAD_LIBRARY})
target_link_libraries(post_log_demo ${CURL_LIBRARY})
target_link_libraries(post_log_demo ${M_LIBRARY})
target_link_libraries(post_log_demo ${SSL_LIBRARY})
target_link_libraries(post_log_demo ${CRYPTO_LIBRARY})
