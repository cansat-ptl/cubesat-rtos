/*
 * string.h
 * 
 * Created: 20.03.2021 02:32:33
 * Author: awend0
 */


#ifndef KERNEL_STRING_H_
#define KERNEL_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

int common_isdigit(char c);
int common_strlen(char *str);
int common_memcmp(const void *s1, const void *s2, size_t n);
void common_memset(void *b, int c, size_t len);
void common_memcpy(void *dst, const void *src, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_STRING_H_ */