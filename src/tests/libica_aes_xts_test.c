/* This program is released under the Common Public License V1.0
 *
 * You should have received a copy of Common Public License V1.0 along with
 * with this program.
 */

/* Copyright IBM Corp. 2010, 2011 */
#include <fcntl.h>
#include <sys/errno.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "ica_api.h"
#include "testcase.h"

#define NR_TESTS 5
#define NR_RANDOM_TESTS 20000

/* XTS data -1- AES128 */
unsigned char NIST_KEY_XTS_E1[] = {
	0x46, 0xe6, 0xed, 0x9e, 0xf4, 0x2d, 0xcd, 0xb3,
	0xc8, 0x93, 0x09, 0x3c, 0x28, 0xe1, 0xfc, 0x0f,
	0x91, 0xf5, 0xca, 0xa3, 0xb6, 0xe0, 0xbc, 0x5a,
	0x14, 0xe7, 0x83, 0x21, 0x5c, 0x1d, 0x5b, 0x61,
};

unsigned char NIST_TWEAK_XTS_E1[] = {
	0x72, 0xf3, 0xb0, 0x54, 0xcb, 0xdc, 0x2f, 0x9e,
	0x3c, 0x5b, 0xc5, 0x51, 0xd4, 0x4d, 0xdb, 0xa0,
};

/* TWEAK should not be updated, so the exptected tweak is the same as the
 * original TWEAK.
 */
unsigned char NIST_EXPECTED_TWEAK_XTS_E1[] = {
	0x72, 0xf3, 0xb0, 0x54, 0xcb, 0xdc, 0x2f, 0x9e,
	0x3c, 0x5b, 0xc5, 0x51, 0xd4, 0x4d, 0xdb, 0xa0,
};

unsigned char NIST_TEST_DATA_XTS_E1[] = {
	0xe3, 0x77, 0x8d, 0x68, 0xe7, 0x30, 0xef, 0x94,
	0x5b, 0x4a, 0xe3, 0xbc, 0x5b, 0x93, 0x6b, 0xdd,
};

unsigned char NIST_TEST_RESULT_XTS_E1[] = {
	0x97, 0x40, 0x9f, 0x1f, 0x71, 0xae, 0x45, 0x21,
	0xcb, 0x49, 0xa3, 0x29, 0x73, 0xde, 0x4d, 0x05,
};

/* XTS data -2- AES128 */
unsigned char NIST_KEY_XTS_E2[] = {
	0x93, 0x56, 0xcd, 0xad, 0x25, 0x1a, 0xb6, 0x11,
	0x14, 0xce, 0xc2, 0xc4, 0x4a, 0x60, 0x92, 0xdd,
	0xe9, 0xf7, 0x46, 0xcc, 0x65, 0xae, 0x3b, 0xd4,
	0x96, 0x68, 0x64, 0xaa, 0x36, 0x26, 0xd1, 0x88,
};

unsigned char NIST_TWEAK_XTS_E2[] = {
	0x68, 0x88, 0x27, 0x83, 0x65, 0x24, 0x36, 0xc4,
	0x85, 0x7a, 0x88, 0xc0, 0xc3, 0x73, 0x41, 0x7e,
};

unsigned char NIST_EXPECTED_TWEAK_XTS_E2[] = {
	0x68, 0x88, 0x27, 0x83, 0x65, 0x24, 0x36, 0xc4,
	0x85, 0x7a, 0x88, 0xc0, 0xc3, 0x73, 0x41, 0x7e,
};

unsigned char NIST_TEST_DATA_XTS_E2[] = {
	0xce, 0x17, 0x6b, 0xdd, 0xe3, 0x39, 0x50, 0x5b,
	0xa1, 0x5d, 0xea, 0x36, 0xd2, 0x8c, 0xe8, 0x7d,
};

unsigned char NIST_TEST_RESULT_XTS_E2[] = {
	0x22, 0xf5, 0xf9, 0x37, 0xdf, 0xb3, 0x9e, 0x5b,
	0x74, 0x25, 0xed, 0x86, 0x3d, 0x31, 0x0b, 0xe1,
};

/* XTS data -3- AES128 */
unsigned char NIST_KEY_XTS_E3[] = {
	0x63, 0xf3, 0x6e, 0x9c, 0x39, 0x7c, 0x65, 0x23,
	0xc9, 0x9f, 0x16, 0x44, 0xec, 0xb1, 0xa5, 0xd9,
	0xbc, 0x0f, 0x2f, 0x55, 0xfb, 0xe3, 0x24, 0x44,
	0x4c, 0x39, 0x0f, 0xae, 0x75, 0x2a, 0xd4, 0xd7,
};

unsigned char NIST_TWEAK_XTS_E3[] = {
	0xcd, 0xb1, 0xbd, 0x34, 0x86, 0xf3, 0x53, 0xcc,
	0x16, 0x0a, 0x84, 0x0b, 0xea, 0xdf, 0x03, 0x29,
};

unsigned char NIST_EXPECTED_TWEAK_XTS_E3[] = {
	0xcd, 0xb1, 0xbd, 0x34, 0x86, 0xf3, 0x53, 0xcc,
	0x16, 0x0a, 0x84, 0x0b, 0xea, 0xdf, 0x03, 0x29,
};

unsigned char NIST_TEST_DATA_XTS_E3[] = {
	0x9a, 0x01, 0x49, 0x88, 0x8b, 0xf7, 0x61, 0x60,
	0xa8, 0x14, 0x28, 0xbc, 0x91, 0x40, 0xec, 0xcd,
	0x26, 0xed, 0x18, 0x36, 0x8e, 0x24, 0xd4, 0x9b,
	0x9c, 0xc5, 0x12, 0x92, 0x9a, 0x88, 0xad, 0x1e,
	0x66, 0xc7, 0x63, 0xf4, 0xf5, 0x6b, 0x63, 0xbb,
	0x9d, 0xd9, 0x50, 0x8c, 0x5d, 0x4d, 0xf4, 0x65,
	0xad, 0x98, 0x82, 0x14, 0x82, 0xfc, 0x71, 0x94,
	0xee, 0x23, 0x54, 0xa3, 0xfa, 0xdc, 0xe9, 0x23,
	0x18, 0x54, 0x8e, 0x8c, 0xe9, 0x45, 0x20, 0x81,
	0x60, 0x49, 0x7b, 0x93, 0x05, 0xd9, 0xab, 0x10,
	0x91, 0xab, 0x41, 0xd1, 0xf0, 0x9a, 0x0c, 0x7b,
	0xfa, 0xf9, 0xf9, 0x4f, 0xe7, 0xc8, 0xf1, 0xea,
	0x96, 0x8f, 0x8f, 0x9a, 0x71, 0x3a, 0xca, 0xde,
	0x18, 0xb6, 0x82, 0x32, 0x10, 0x6f, 0xfd, 0x6d,
	0x42, 0x81, 0xe9, 0x9e, 0x11, 0xd6, 0xa4, 0x28,
	0xb5, 0x16, 0x53, 0xc0, 0xc7, 0xdd, 0xe5, 0xa0,
	0xf2, 0x73, 0xe7, 0x4f, 0xf0, 0x15, 0xce, 0x80,
	0x27, 0x7d, 0x74, 0x30, 0xf5, 0xda, 0xea, 0x8f,
	0x73, 0x40, 0x64, 0x5e, 0x0b, 0xec, 0x25, 0xf4,
	0x04, 0x0f, 0xa1, 0x3c, 0x0b, 0x33, 0x06, 0x93,
	0xb1, 0x00, 0x83, 0xa8, 0xb9, 0xbc, 0x10, 0x8f,
	0xe6, 0x4f, 0x3a, 0x5b, 0x61, 0x3c, 0xbb, 0x56,
	0x5a, 0xee, 0x2f, 0x09, 0xf5, 0xb2, 0x04, 0xae,
	0xe1, 0x72, 0x28, 0xfe, 0x65, 0x31, 0xc7, 0x0c,
	0x0e, 0xc9, 0x47, 0xd2, 0xa5, 0x14, 0x7b, 0x45,
	0xc5, 0x1a, 0xc7, 0xdc, 0x8e, 0x85, 0x87, 0x03,
	0x87, 0xeb, 0x8d, 0xb6, 0x25, 0x13, 0x68, 0x36,
	0x8b, 0xf5, 0xf2, 0x46, 0xb2, 0x95, 0x7d, 0xaf,
	0xf7, 0x02, 0xe3, 0x79, 0x02, 0x2e, 0x99, 0x16,
	0x17, 0x49, 0xe6, 0xbe, 0x8e, 0xb7, 0x9d, 0x51,
	0x97, 0x99, 0xaa, 0xe0, 0x7c, 0x18, 0x31, 0xbd,
	0x0e, 0xe7, 0x25, 0x50, 0xb8, 0x53, 0x33, 0xab,
	0x9e, 0x96, 0xa5, 0x33, 0xe2, 0x97, 0x25, 0xd7,
	0x02, 0x3d, 0x82, 0x1a, 0xbe, 0x1c, 0xe3, 0xa7,
	0x44, 0xbe, 0x02, 0xe0, 0x52, 0x56, 0x8f, 0x84,
	0xe6, 0xe3, 0xf7, 0x44, 0x42, 0xbb, 0xa5, 0x0d,
	0x02, 0xad, 0x2d, 0x6c, 0xa5, 0x8a, 0x69, 0x1f,
	0xd2, 0x43, 0x9a, 0xa3, 0xaf, 0x0c, 0x03, 0x3a,
	0x68, 0xc4, 0x38, 0xb2, 0xd9, 0xa0, 0xa0, 0x1d,
	0x78, 0xc4, 0xf8, 0x7c, 0x50, 0x9f, 0xea, 0x0a,
	0x43, 0x5b, 0xe7, 0x1b, 0xa2, 0x37, 0x06, 0xd6,
	0x08, 0x2d, 0xcb, 0xa6, 0x26, 0x25, 0x99, 0x9e,
	0xce, 0x09, 0xdf, 0xb3, 0xfc, 0xbe, 0x08, 0xeb,
	0xb6, 0xf2, 0x15, 0x1e, 0x2f, 0x12, 0xeb, 0xe8,
	0xa5, 0xbf, 0x11, 0x62, 0xc2, 0x59, 0xf2, 0x02,
	0xc1, 0xba, 0x47, 0x8b, 0x5f, 0x46, 0x8a, 0x28,
	0x69, 0xf1, 0xe7, 0x6c, 0xf5, 0xed, 0x38, 0xde,
	0x53, 0x86, 0x9a, 0xdc, 0x83, 0x70, 0x9e, 0x21,
	0xb3, 0xf8, 0xdc, 0x13, 0xba, 0x3d, 0x6a, 0xa7,
	0xf6, 0xb0, 0xcf, 0xb3, 0xe5, 0xa4, 0x3c, 0x23,
	0x72, 0xe0, 0xee, 0x60, 0x99, 0x1c, 0xe1, 0xca,
	0xd1, 0x22, 0xa3, 0x1d, 0x93, 0x97, 0xe3, 0x0b,
	0x92, 0x1f, 0xd2, 0xf6, 0xee, 0x69, 0x6e, 0x68,
	0x49, 0xae, 0xee, 0x29, 0xe2, 0xb4, 0x45, 0xc0,
	0xfd, 0x9a, 0xde, 0x65, 0x56, 0xc3, 0xc0, 0x69,
	0xc5, 0xd6, 0x05, 0x95, 0xab, 0xbd, 0xf5, 0xba,
	0xe2, 0xcc, 0xc7, 0x9a, 0x49, 0x6e, 0x83, 0xcc,
	0xab, 0x95, 0x74, 0x0e, 0xb8, 0xe4, 0xf2, 0x92,
	0x5d, 0xbf, 0x72, 0x97, 0xa8, 0xc9, 0x92, 0x75,
	0x6e, 0x62, 0x87, 0x0e, 0xdc, 0xe9, 0x8f, 0x6c,
	0xba, 0x1a, 0xa0, 0xd5, 0xb8, 0x6f, 0x09, 0x21,
	0x43, 0xb1, 0x6d, 0xa1, 0x44, 0x15, 0x47, 0xd1,
	0xd4, 0x2b, 0x80, 0x06, 0xfa, 0xce, 0x69, 0x5b,
	0x03, 0xfd, 0xfa, 0xe6, 0x45, 0xf9, 0x5b, 0xd6,
};

unsigned char NIST_TEST_RESULT_XTS_E3[] = {
	0x0e, 0xee, 0xf2, 0x8c, 0xa1, 0x59, 0xb8, 0x05,
	0xf5, 0xc2, 0x15, 0x61, 0x05, 0x51, 0x67, 0x8a,
	0xb7, 0x72, 0xf2, 0x79, 0x37, 0x4f, 0xb1, 0x40,
	0xab, 0x55, 0x07, 0x68, 0xdb, 0x42, 0xcf, 0x6c,
	0xb7, 0x36, 0x37, 0x64, 0x19, 0x34, 0x19, 0x5f,
	0xfc, 0x08, 0xcf, 0x5a, 0x91, 0x88, 0xb8, 0x2b,
	0x84, 0x0a, 0x00, 0x7d, 0x52, 0x72, 0x39, 0xea,
	0x3f, 0x0d, 0x7d, 0xd1, 0xf2, 0x51, 0x86, 0xec,
	0xae, 0x30, 0x87, 0x7d, 0xad, 0xa7, 0x7f, 0x24,
	0x3c, 0xdd, 0xb2, 0xc8, 0x8e, 0x99, 0x04, 0x82,
	0x7d, 0x3e, 0x09, 0x82, 0xda, 0x0d, 0x13, 0x91,
	0x1d, 0x0e, 0x2d, 0xbb, 0xbb, 0x2d, 0x01, 0x6c,
	0xbe, 0x4d, 0x06, 0x76, 0xb1, 0x45, 0x9d, 0xa8,
	0xc5, 0x3a, 0x91, 0x45, 0xe8, 0x3c, 0xf4, 0x2f,
	0x30, 0x11, 0x2c, 0xa6, 0x5d, 0x77, 0xc8, 0x93,
	0x4a, 0x26, 0xee, 0x00, 0x1f, 0x39, 0x0f, 0xfc,
	0xc1, 0x87, 0x03, 0x66, 0x2a, 0x8f, 0x71, 0xf9,
	0xda, 0x0e, 0x7b, 0x68, 0xb1, 0x04, 0x3c, 0x1c,
	0xb5, 0x26, 0x08, 0xcf, 0x0e, 0x69, 0x51, 0x0d,
	0x38, 0xc8, 0x0f, 0xa0, 0x0d, 0xe4, 0x3d, 0xef,
	0x98, 0x4d, 0xff, 0x2f, 0x32, 0x4e, 0xcf, 0x39,
	0x89, 0x44, 0x53, 0xd3, 0xe0, 0x1b, 0x3d, 0x7b,
	0x3b, 0xc0, 0x57, 0x04, 0x9d, 0x19, 0x5c, 0x8e,
	0xb9, 0x3f, 0xe4, 0xd9, 0x5a, 0x83, 0x00, 0xa5,
	0xe6, 0x0a, 0x7c, 0x89, 0xe4, 0x0c, 0x69, 0x16,
	0x79, 0xfb, 0xca, 0xfa, 0xd8, 0xeb, 0x41, 0x8f,
	0x8d, 0x1f, 0xf7, 0xb9, 0x11, 0x75, 0xf8, 0xeb,
	0x3c, 0x6f, 0xf2, 0x87, 0x2d, 0x32, 0xee, 0x4c,
	0x57, 0x36, 0x9e, 0x61, 0xb6, 0x6d, 0x16, 0x6f,
	0xd0, 0xa4, 0x34, 0x57, 0x47, 0x82, 0x75, 0xfe,
	0x14, 0xbf, 0x34, 0x63, 0x8a, 0x9e, 0x4e, 0x1d,
	0x25, 0xcc, 0x5a, 0x5f, 0x9e, 0x25, 0x7e, 0x61,
	0x7a, 0xdc, 0xdd, 0xe6, 0x5e, 0x25, 0x57, 0x40,
	0x53, 0x62, 0xc8, 0x91, 0xe6, 0x54, 0x6a, 0x6d,
	0xee, 0xaa, 0x8f, 0xc0, 0x3b, 0x12, 0x2a, 0x55,
	0x87, 0x4d, 0x33, 0xe0, 0xa7, 0x73, 0x52, 0x34,
	0x68, 0x32, 0x5e, 0xc2, 0x4d, 0x4f, 0xaf, 0xfb,
	0x63, 0xc0, 0x52, 0xc8, 0x11, 0xa1, 0xc0, 0x22,
	0xba, 0xfc, 0xcb, 0x97, 0x98, 0x8b, 0x7e, 0x45,
	0x67, 0xb2, 0x47, 0xd4, 0x04, 0x4b, 0x05, 0x2f,
	0xf7, 0x3f, 0x4c, 0x67, 0x1d, 0x27, 0xe0, 0x52,
	0xe2, 0xeb, 0xc7, 0x2d, 0x00, 0x57, 0xcb, 0x21,
	0x7c, 0x52, 0x59, 0xb6, 0x09, 0x50, 0xe3, 0xc8,
	0xb3, 0xd9, 0xe3, 0xe7, 0x63, 0x0f, 0x9e, 0xcb,
	0xe5, 0x48, 0xb9, 0xe3, 0x62, 0x20, 0xf3, 0x3c,
	0x2b, 0x45, 0x68, 0x30, 0x7c, 0xd0, 0x37, 0x5b,
	0xba, 0x13, 0x35, 0xe5, 0x8b, 0xfb, 0xcd, 0xe8,
	0x5c, 0xc8, 0x4c, 0x9c, 0x9c, 0x1c, 0xe7, 0x4f,
	0x44, 0xb2, 0x8e, 0xa1, 0xb6, 0x97, 0x30, 0x5b,
	0xb6, 0xba, 0x3b, 0x46, 0x4e, 0x5a, 0xb7, 0x45,
	0x01, 0x29, 0x3e, 0xf9, 0x15, 0x2c, 0x0f, 0x5d,
	0x33, 0x07, 0xd2, 0x6a, 0x1f, 0x07, 0x41, 0xc5,
	0xe5, 0x72, 0x1a, 0x71, 0x3d, 0x1b, 0x86, 0xc1,
	0x80, 0x82, 0x11, 0xf5, 0x7a, 0xad, 0x09, 0xa9,
	0x50, 0xb6, 0x86, 0x30, 0xaf, 0xce, 0x4f, 0x0a,
	0xd9, 0xf3, 0x2e, 0x67, 0x69, 0xb5, 0xfe, 0x31,
	0x92, 0x9c, 0x44, 0x6f, 0x7a, 0x33, 0x55, 0xf4,
	0x58, 0x84, 0xc7, 0x48, 0xc9, 0x05, 0x54, 0x15,
	0xe6, 0x37, 0xd9, 0xad, 0x87, 0xd9, 0x4c, 0x46,
	0x57, 0xb1, 0xad, 0x03, 0x4c, 0xb1, 0x4d, 0x9a,
	0x72, 0xea, 0x74, 0x5f, 0xe5, 0x2d, 0x7a, 0x71,
	0x1b, 0xa4, 0x1c, 0xa0, 0x35, 0x85, 0x6a, 0x5a,
	0x44, 0x89, 0xa4, 0x27, 0x0b, 0xb3, 0x0d, 0x5b,
	0x63, 0xf4, 0x9c, 0x05, 0x12, 0xfe, 0xd4, 0xb4
};

/* XTS data -4- AES256 */
unsigned char NIST_KEY_XTS_E4[] = {
	0x97, 0x09, 0x8b, 0x46, 0x5a, 0x44, 0xca, 0x75,
	0xe7, 0xa1, 0xc2, 0xdb, 0xfc, 0x40, 0xb7, 0xa6,
	0x1a, 0x20, 0xe3, 0x2c, 0x6d, 0x9d, 0xbf, 0xda,
	0x80, 0x72, 0x6f, 0xee, 0x10, 0x54, 0x1b, 0xab,
	0x47, 0x54, 0x63, 0xca, 0x07, 0xc1, 0xc1, 0xe4,
	0x49, 0x61, 0x73, 0x32, 0x14, 0x68, 0xd1, 0xab,
	0x3f, 0xad, 0x8a, 0xd9, 0x1f, 0xcd, 0xc6, 0x2a,
	0xbe, 0x07, 0xbf, 0xf8, 0xef, 0x96, 0x1b, 0x6b,
};

unsigned char NIST_TWEAK_XTS_E4[] = {
	0x15, 0x60, 0x1e, 0x2e, 0x35, 0x85, 0x10, 0xa0,
	0x9d, 0xdc, 0xa4, 0xea, 0x17, 0x51, 0xf4, 0x3c,
};

unsigned char NIST_EXPECTED_TWEAK_XTS_E4[] = {
	0x15, 0x60, 0x1e, 0x2e, 0x35, 0x85, 0x10, 0xa0,
	0x9d, 0xdc, 0xa4, 0xea, 0x17, 0x51, 0xf4, 0x3c,
};

unsigned char NIST_TEST_DATA_XTS_E4[] = {
	0xd1, 0x9c, 0xfb, 0x38, 0x3b, 0xaf, 0x87, 0x2e,
	0x6f, 0x12, 0x16, 0x87, 0x45, 0x1d, 0xe1, 0x5c,
};

unsigned char NIST_TEST_RESULT_XTS_E4[] = {
	0xeb, 0x22, 0x26, 0x9b, 0x14, 0x90, 0x50, 0x27,
	0xdc, 0x73, 0xc4, 0xa4, 0x0f, 0x93, 0x80, 0x69,
};


/* XTS data -5- AES256 */
unsigned char NIST_KEY_XTS_E5[] = {
	0xfb, 0xf0, 0x77, 0x6e, 0x7d, 0xbe, 0x49, 0x10,
	0xfb, 0x0c, 0x12, 0x0f, 0x41, 0x85, 0x71, 0x21,
	0x92, 0x6c, 0x05, 0x2f, 0xd6, 0x5a, 0x27, 0x8c,
	0xd2, 0xf0, 0xd9, 0x8d, 0xa5, 0x4e, 0xdf, 0xd5,
	0x08, 0x03, 0xa4, 0x2f, 0xbe, 0x6f, 0xd1, 0x33,
	0x58, 0x49, 0x00, 0xe8, 0xdc, 0x7a, 0x11, 0x52,
	0x39, 0x1f, 0x82, 0x2d, 0x76, 0xa7, 0x56, 0x68,
	0xcf, 0xce, 0x7f, 0x8d, 0xde, 0x20, 0x3e, 0xc8,
};

unsigned char NIST_TWEAK_XTS_E5[] = {
	0x39, 0x5b, 0x6a, 0xcf, 0x9a, 0xdc, 0xd2, 0x91,
	0xc2, 0xc9, 0x48, 0x86, 0x36, 0x33, 0xaf, 0xf8,
};

unsigned char NIST_EXPECTED_TWEAK_XTS_E5[] = {
	0x39, 0x5b, 0x6a, 0xcf, 0x9a, 0xdc, 0xd2, 0x91,
	0xc2, 0xc9, 0x48, 0x86, 0x36, 0x33, 0xaf, 0xf8,
};

unsigned char NIST_TEST_DATA_XTS_E5[] = {
	0x3e, 0x2e, 0x26, 0x9d, 0x78, 0x3a, 0x2b, 0x29,
	0xe8, 0x73, 0xd6, 0x73, 0x47, 0x9f, 0x51, 0x16,
	0x73, 0x4f, 0xe0, 0x3e, 0xe3, 0x29, 0x65, 0xed,
	0xc4, 0x79, 0x35, 0xc0, 0xea, 0x99, 0xa0, 0x64,
	0xbd, 0x44, 0x4b, 0xec, 0x12, 0x5b, 0x2c, 0x78,
	0x9d, 0xb9, 0xde, 0x6d, 0x18, 0x35, 0x92, 0x05,
	0x3b, 0x48, 0xa8, 0x77, 0xa9, 0x5a, 0xc2, 0x55,
	0x9c, 0x3d, 0xdf, 0xc7, 0xb4, 0xdb, 0x99, 0x07,
};

unsigned char NIST_TEST_RESULT_XTS_E5[] = {
	0x4c, 0x70, 0xbd, 0xbb, 0x77, 0x30, 0x2b, 0x7f,
	0x1f, 0xdd, 0xca, 0x50, 0xdc, 0x70, 0x73, 0x1e,
	0x00, 0x8a, 0x26, 0x55, 0xd2, 0x2a, 0xd0, 0x20,
	0x0c, 0x11, 0x1f, 0xd3, 0x2a, 0x67, 0x5a, 0x7e,
	0x09, 0x97, 0x11, 0x43, 0x6f, 0x98, 0xd2, 0x1c,
	0x72, 0x77, 0x2e, 0x0d, 0xd7, 0x67, 0x2f, 0xf5,
	0xfd, 0x00, 0xdd, 0xcb, 0xe1, 0x1e, 0xb9, 0x7e,
	0x69, 0x87, 0x83, 0xbf, 0xa4, 0x05, 0x46, 0xe3,
};

void dump_xts_data(unsigned char *tweak, unsigned int tweak_length,
		   unsigned char *key, unsigned int key_length,
		   unsigned char *input_data, unsigned int data_length,
		   unsigned char *output_data)
{
	VV_(printf("TWEAK \n"));
	dump_array(tweak, tweak_length);
	VV_(printf("Key \n"));
	dump_array(key, key_length);
	VV_(printf("Input Data\n"));
	dump_array(input_data, data_length);
	VV_(printf("Output Data\n"));
	dump_array(output_data, data_length);
}

void get_sizes(unsigned int *data_length, unsigned int *tweak_length,
	       unsigned int *key_length, unsigned int iteration)
{
	switch (iteration) {
		case 1:
			*data_length = sizeof(NIST_TEST_DATA_XTS_E1);
			*tweak_length = sizeof(NIST_TWEAK_XTS_E1);
			*key_length = sizeof(NIST_KEY_XTS_E1);
			break;
		case 2:
			*data_length = sizeof(NIST_TEST_DATA_XTS_E2);
			*tweak_length = sizeof(NIST_TWEAK_XTS_E2);
			*key_length = sizeof(NIST_KEY_XTS_E2);
			break;
		case 3:
			*data_length = sizeof(NIST_TEST_DATA_XTS_E3);
			*tweak_length = sizeof(NIST_TWEAK_XTS_E3);
			*key_length = sizeof(NIST_KEY_XTS_E3);
			break;
		case 4:
			*data_length = sizeof(NIST_TEST_DATA_XTS_E4);
			*tweak_length = sizeof(NIST_TWEAK_XTS_E4);
			*key_length = sizeof(NIST_KEY_XTS_E4);
			break;
		case 5:
			*data_length = sizeof(NIST_TEST_DATA_XTS_E5);
			*tweak_length = sizeof(NIST_TWEAK_XTS_E5);
			*key_length = sizeof(NIST_KEY_XTS_E5);
			break;
	}

}

void load_test_data(unsigned char *data, unsigned int data_length,
		    unsigned char *result,
		    unsigned char *tweak, unsigned char *expected_tweak,
		    unsigned int tweak_length,
		    unsigned char *key, unsigned int key_length,
		    unsigned int iteration)
{
	switch (iteration) {
		case 1:
			memcpy(data, NIST_TEST_DATA_XTS_E1, data_length);
			memcpy(result, NIST_TEST_RESULT_XTS_E1, data_length);
			memcpy(tweak, NIST_TWEAK_XTS_E1, tweak_length);
			memcpy(expected_tweak, NIST_EXPECTED_TWEAK_XTS_E1,
			       tweak_length);
			memcpy(key, NIST_KEY_XTS_E1, key_length);
			break;
		case 2:
			memcpy(data, NIST_TEST_DATA_XTS_E2, data_length);
			memcpy(result, NIST_TEST_RESULT_XTS_E2, data_length);
			memcpy(tweak, NIST_TWEAK_XTS_E2, tweak_length);
			memcpy(expected_tweak, NIST_EXPECTED_TWEAK_XTS_E2,
			       tweak_length);
			memcpy(key, NIST_KEY_XTS_E2, key_length);
			break;
		case 3:
			memcpy(data, NIST_TEST_DATA_XTS_E3, data_length);
			memcpy(result, NIST_TEST_RESULT_XTS_E3, data_length);
			memcpy(tweak, NIST_TWEAK_XTS_E3, tweak_length);
			memcpy(expected_tweak, NIST_EXPECTED_TWEAK_XTS_E3,
			       tweak_length);
			memcpy(key, NIST_KEY_XTS_E3, key_length);
			break;
		case 4:
			memcpy(data, NIST_TEST_DATA_XTS_E4, data_length);
			memcpy(result, NIST_TEST_RESULT_XTS_E4, data_length);
			memcpy(tweak, NIST_TWEAK_XTS_E4, tweak_length);
			memcpy(expected_tweak, NIST_EXPECTED_TWEAK_XTS_E4,
			       tweak_length);
			memcpy(key, NIST_KEY_XTS_E4, key_length);
			break;
		case 5:
			memcpy(data, NIST_TEST_DATA_XTS_E5, data_length);
			memcpy(result, NIST_TEST_RESULT_XTS_E5, data_length);
			memcpy(tweak, NIST_TWEAK_XTS_E5, tweak_length);
			memcpy(expected_tweak, NIST_EXPECTED_TWEAK_XTS_E5,
			       tweak_length);
			memcpy(key, NIST_KEY_XTS_E5, key_length);
			break;
	}

}

int kat_aes_xts(int iteration)
{
	unsigned int data_length;
	unsigned int tweak_length;
	unsigned int key_length;

	get_sizes(&data_length, &tweak_length, &key_length, iteration);

	unsigned char tweak[tweak_length];
	unsigned char tmp_tweak[tweak_length];
	unsigned char expected_tweak[tweak_length];
	unsigned char key[key_length];
	unsigned char input_data[data_length];
	unsigned char encrypt[data_length];
	unsigned char decrypt[data_length];
	unsigned char result[data_length];

	int rc = 0;

	memset(encrypt, 0x00, data_length);
	memset(decrypt, 0x00, data_length);

	load_test_data(input_data, data_length, result, tweak, expected_tweak,
		       tweak_length, key, key_length, iteration);
	memcpy(tmp_tweak, tweak, tweak_length);

	VV_(printf("Test Parameters for iteration = %i\n", iteration));
	VV_(printf("key length = %i, data length = %i, tweak length = %i,",
		key_length, data_length, tweak_length));
	rc = ica_aes_xts(input_data, encrypt, data_length,
			 key, key+(key_length/2), (key_length/2),
			 tmp_tweak, 1);
	if (rc) {
		VV_(printf("ica_aes_xts encrypt failed with rc = %i\n", rc));
		dump_xts_data(tweak, tweak_length, key, key_length, input_data,
			      data_length, encrypt);
	}
	if (!rc) {
		VV_(printf("Encrypt:\n"));
		dump_xts_data(tweak, tweak_length, key, key_length, input_data,
			      data_length, encrypt);
	}

	if (memcmp(result, encrypt, data_length)) {
		VV_(printf("Encryption Result does not match the known ciphertext!\n"));
		VV_(printf("Expected data:\n"));
		dump_array(result, data_length);
		VV_(printf("Encryption Result:\n"));
		dump_array(encrypt, data_length);
		rc++;
	}

	if (memcmp(expected_tweak, tmp_tweak, tweak_length)) {
		VV_(printf("Update of TWEAK does not match the expected TWEAK!\n"));
		VV_(printf("Expected TWEAK:\n"));
		dump_array(expected_tweak, tweak_length);
		VV_(printf("Updated TWEAK:\n"));
		dump_array(tmp_tweak, tweak_length);
		VV_(printf("Original TWEAK:\n"));
		dump_array(tweak, tweak_length);
		rc++;
	}
	if (rc) {
		VV_(printf("AES XTS test exited after encryption\n"));
		return rc;
	}

	memcpy(tmp_tweak, tweak, tweak_length);
	rc = ica_aes_xts(encrypt, decrypt, data_length,
			 key, key+(key_length/2), (key_length/2),
			 tmp_tweak, 0);
	if (rc) {
		VV_(printf("ica_aes_xts decrypt failed with rc = %i\n", rc));
		dump_xts_data(tweak, tweak_length, key, key_length, encrypt,
			      data_length, decrypt);
		return rc;
	}


	if (!rc) {
		VV_(printf("Decrypt:\n"));
		dump_xts_data(tweak, tweak_length, key, key_length, encrypt,
			      data_length, decrypt);
	}

	if (memcmp(decrypt, input_data, data_length)) {
		VV_(printf("Decryption Result does not match the original data!\n"));
		VV_(printf("Original data:\n"));
		dump_array(input_data, data_length);
		VV_(printf("Decryption Result:\n"));
		dump_array(decrypt, data_length);
		rc++;
	}
	return rc;
}

int load_random_test_data(unsigned char *data, unsigned int data_length,
			   unsigned char *iv, unsigned int iv_length,
			   unsigned char *key, unsigned int key_length)
{
	int rc;

	rc = ica_random_number_generate(data_length, data);
	if (rc) {
		VV_(printf("ica_random_number_generate with rc = %i errnor = %i\n",
		       rc, errno));
		return rc;
	}
	rc = ica_random_number_generate(iv_length, iv);
	if (rc) {
		VV_(printf("ica_random_number_generate with rc = %i errnor = %i\n",
		       rc, errno));
		return rc;
	}
	rc = ica_random_number_generate(key_length, key);
	if (rc) {
		VV_(printf("ica_random_number_generate with rc = %i errnor = %i\n",
		       rc, errno));
		return rc;
	}
	return rc;
}

int random_aes_xts(int iteration, unsigned int data_length)
{
	int i;
	int rc = 0;
	unsigned int iv_length = sizeof(ica_aes_vector_t);
	unsigned int key_length = AES_KEY_LEN128 * 2;
	unsigned char iv[iv_length];
	unsigned char tmp_iv[iv_length];
	unsigned char input_data[data_length];
	unsigned char encrypt[data_length];
	unsigned char decrypt[data_length];

	for (i = 1; i <= 2; i++) {
		unsigned char key[key_length];

		memset(encrypt, 0x00, data_length);
		memset(decrypt, 0x00, data_length);

		load_random_test_data(input_data, data_length, iv, iv_length, key,
				      key_length);
		memcpy(tmp_iv, iv, iv_length);

		VV_(printf("Test Parameters for iteration = %i\n", iteration));
		VV_(printf("key length = %i, data length = %i, iv length = %i\n",
			key_length, data_length, iv_length));
		rc = ica_aes_xts(input_data, encrypt, data_length,
				 key, key+(key_length/2), (key_length/2),
				 tmp_iv, 1);
		if (rc) {
			VV_(printf("ica_aes_xts encrypt failed with rc = %i\n", rc));
			dump_xts_data(iv, iv_length, key, key_length, input_data,
				      data_length, encrypt);
		}
		if (!rc) {
			VV_(printf("Encrypt:\n"));
			dump_xts_data(iv, iv_length, key, key_length, input_data,
				      data_length, encrypt);
		}

		if (rc) {
			VV_(printf("AES XTS test exited after encryption\n"));
			return rc;
		}

		memcpy(tmp_iv, iv, iv_length);

		rc = ica_aes_xts(encrypt, decrypt, data_length,
				 key, key+(key_length/2), (key_length/2),
				 tmp_iv, 0);
		if (rc) {
			VV_(printf("ica_aes_xts decrypt failed with rc = %i\n", rc));
			dump_xts_data(iv, iv_length, key, key_length, encrypt,
				      data_length, decrypt);
			return rc;
		}


		if (!rc) {
			VV_(printf("Decrypt:\n"));
			dump_xts_data(iv, iv_length, key, key_length, encrypt,
				      data_length, decrypt);
		}

		if (memcmp(decrypt, input_data, data_length)) {
			VV_(printf("Decryption Result does not match the original data!\n"));
			VV_(printf("Original data:\n"));
			dump_array(input_data, data_length);
			VV_(printf("Decryption Result:\n"));
			dump_array(decrypt, data_length);
			rc++;
			return rc;
		}
		key_length = AES_KEY_LEN256 * 2;
	}

	return rc;
}

int main(int argc, char **argv)
{
	int rc = 0;
	int error_count = 0;
	int iteration;
	unsigned int data_length = sizeof(ica_aes_vector_t);

	set_verbosity(argc, argv);

	for(iteration = 1; iteration <= NR_TESTS; iteration++)	{
		rc = kat_aes_xts(iteration);
		if (rc) {
			V_(printf("kat_aes_xts failed with rc = %i\n", rc));
			error_count++;
		}

	}
	for(iteration = 1; iteration <= NR_RANDOM_TESTS; iteration++)	{
		rc = random_aes_xts(iteration, data_length);
		if (rc) {
			V_(printf("random_aes_xts failed with rc = %i\n", rc));
			error_count++;
			goto out;
		}
		data_length += sizeof(ica_aes_vector_t) / 2;
	}

out:
	if (error_count)
		printf("%i AES-XTS tests failed.\n", error_count);
	else
		printf("All AES-XTS tests passed.\n");

	return rc;
}

