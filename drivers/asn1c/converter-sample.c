/*
 * Generic converter template for a selected ASN.1 type.
 * Copyright (c) 2005, 2006, 2007 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * 
 * To compile with your own ASN.1 type, please redefine the PDU as shown:
 * 
 * cc -DPDU=MyCustomType -o myDecoder.o -c converter-sample.c
 */
#include <stdio.h>	/* for stdout */
#include <stdlib.h>	/* for malloc() */
#include "DiscoveryData.h"	/* Include MyTypes definition */

int my_dump(char *desc, uint8_t *data, size_t size, uint32_t index)
{
	int i;
	printf("%s(%zd): ", desc, size);
	for (i = 0; i < size; i++) {
		if (data[i] > 0xf) {
			printf("%x", data[i]);
		} else {
			printf("0%x", data[i]);
		}
	}
	printf("\n");
}

int my_str_dump(char *desc, char *data, uint32_t index)
{
	int i;
	printf("%s(%zd): ", desc, strlen(data));
	printf("%s", data);
	printf("\n");
}

int discoveryData_test_demo(int argc, char * const argv[])
{
	uint8_t send_buf[256] = {0};
	DiscoveryData_t *discoveryData = 0;
	DiscoveryData_t *disData = (DiscoveryData_t *)calloc(1, sizeof(DiscoveryData_t));
	struct DiscoveryData__errata *disData_errata = (struct DiscoveryData__errata *)calloc(1, sizeof(struct DiscoveryData__errata));
	struct DiscoveryData__ext *disData_ext = (struct DiscoveryData__ext *)calloc(1, sizeof(struct DiscoveryData__ext));
	struct RVVConfig *rvv = (struct RVVConfig *)calloc(1, sizeof(struct RVVConfig));
	DiscoveryErrata_t *errata;
	DiscoveryAdditonalData_t *ext;
	
	asn_dec_rval_t rval;
	asn_enc_rval_t eval;
	int i, ret;
	uint8_t *buffer = NULL;
	size_t size = 0;
	printf("%s %d\n", __func__, __LINE__);
	uint8_t payload_data[5] = {0x80, 0x80, 0x80, 0x80, 0x80};
	int payload_len = sizeof(payload_data);

	UTF8String_t url;
	char *oid_test = "1.2.3";
	char *url_test = "123456789";
	char *ext_tag_url_test = "v0.7Vector";
	char *rvv_test = "1234";
	int oid_len = strlen(oid_test);
	int url_len = strlen(url_test);
	int rvv_len = strlen(rvv_test);
	int ext_tag_url_len = strlen(ext_tag_url_test);
	
	if (!disData || !disData_errata || !disData_ext || !rvv) {
		printf("%s %d calloc error\n", __func__, __LINE__);
		ret = -1;
		goto ERR;
	}

	/* update disData msg*/
	disData->version = 1;
	disData->c = NULL;
	disData->errata = disData_errata;
	disData->ext = disData_ext;
	disData->rvv = rvv;

	printf("before encode:\n");
	printf("version:%d\n", disData->version);
	my_str_dump("rvv_vlen", rvv_test, 0);
	my_str_dump("errata_oid", oid_test, 0);
	my_str_dump("errata_url", url_test, 0);
	my_dump("ext_payload", payload_data, sizeof(payload_data), 0);
	my_str_dump("ext_tag_choice_url", ext_tag_url_test, 0);

	/***************************deal errata********************************/
	for (i = 0; i < 1; i++) {
		errata = (DiscoveryErrata_t *)calloc(1, sizeof(DiscoveryErrata_t));
		if(!errata) {
			printf("%s %d calloc errata error\n", __func__, __LINE__);
			ret = -1;
			goto ERR;
		}
		/*fill errata url*/
		url.buf = (uint8_t *)url_test;
		url.size = url_len;
		errata->url = &url;

		/*fill errata oid*/
		errata->oid.buf = (uint8_t *)oid_test;
		errata->oid.size = oid_len;

		/*add errata sequence of*/
		ret = ASN_SEQUENCE_ADD(disData->errata, errata);
		if(ret != 0) {
			printf("%s %d ASN_SEQUENCE_ADD error\n", __func__, __LINE__);
			ret = -1;
			goto ERR;
		}
	}

	/***************************deal ext********************************/
	for (i = 0; i < 1; i++) {
		ext = (DiscoveryAdditonalData_t *)calloc(1, sizeof(DiscoveryAdditonalData_t));
		if(!ext) {
			printf("%s %d calloc ext error\n", __func__, __LINE__);
			ret = -1;
			goto ERR;
		}
		/*fill ext payload*/
		ext->payload.buf = (uint8_t *)payload_data;
		ext->payload.size = payload_len;
		ext->tag.choice.url.buf = (uint8_t *)ext_tag_url_test;
		ext->tag.choice.url.size = ext_tag_url_len;
		ext->tag.present = DiscoveryAdditonalData__tag_PR_url;
	
		/*add ext sequence of*/
		ret = ASN_SEQUENCE_ADD(disData->ext, ext);
		if(ret != 0) {
			printf("%s %d ASN_SEQUENCE_ADD error\n", __func__, __LINE__);
			ret = -1;
			goto ERR;
		}
	}

	/***************************deal rvv********************************/
	rvv->vlen.buf = (uint8_t *)rvv_test;
	rvv->vlen.bits_unused = 0x01;
	rvv->vlen.size = rvv_len;

	/*
	 * Encode to the BER buffer.
	 */
	eval = der_encode_to_buffer(&asn_DEF_DiscoveryData, disData, send_buf, sizeof(send_buf));
	if (eval.encoded < RC_OK) {
		printf("der_encode_to_buffer error code:%x\n", eval.encoded);
		ret = -1;
		goto ERR;
	}
	
	my_dump("encoded_buffer", send_buf, eval.encoded, 0);

	buffer = send_buf;
	size = eval.encoded;

	/*
	 * Decode the BER buffer.
	 */
	rval = ber_decode(0, &asn_DEF_DiscoveryData, &discoveryData, buffer, size);
	if (rval.code != RC_OK) {
		printf("\ncode:%x\n", rval.code);
		ret = -1;
		goto ERR;
	}

	printf("after decoded to discoveryData:\n");
	if (discoveryData) {
		printf("discoveryData.version:%d\n", discoveryData->version);
		my_str_dump("discoveryData.rvv.vlen", discoveryData->rvv->vlen.buf, 0);
		for (i = 0; i < discoveryData->errata->list.count; i++) {
			my_str_dump("discoveryData.errata.oid", discoveryData->errata->list.array[i]->oid.buf, i);
			my_str_dump("discoveryData.errata.url", discoveryData->errata->list.array[i]->url->buf, i);
		}
		for (i = 0; i < discoveryData->ext->list.count; i++) {
			my_dump("discoveryData.ext.payload", discoveryData->ext->list.array[i]->payload.buf, discoveryData->ext->list.array[i]->payload.size, i);
			if (discoveryData->ext->list.array[i]->tag.present == DiscoveryAdditonalData__tag_PR_url) {
				my_str_dump("discoveryData.tag.choice.url", discoveryData->ext->list.array[i]->tag.choice.url.buf, i);
			}
		}
	} else {
		printf("ber_decode error!\n");
		ret = -1;
		goto ERR;
	}
	ret = 0;
ERR:
	if (disData) {
		free(disData);
	}
	if (disData_errata) {
		free(disData_errata);
	}
	if (disData_ext) {
		free(disData_ext);
	}
	if (rvv) {
		free(rvv);
	}
	if (errata) {
		free(errata);
	}
	if (ext) {
		free(ext);
	}
	return ret;
}

