// SPDX-License-Identifier: GPL 2.0 OR BSD-3-Clause
/*
 *  Copyright 2018-2019 NXP
 */

/**
 * @addtogroup  dxgr_PFE_HIF
 * @{
 *
 * @file		pfe_hif_nocpy.c
 * @brief		The HIF_NOCPY module source file.
 * @details
 *				Purpose
 *				-------
 *				This file contains HIF_NOCPY-related functionality. Every HIF_NOCPY instance is owner
 *				of a single underlying HIF Channel component representing particular HIF RX/TX resource
 *				(channel). Since the HIF_NOCPY is not partitioned into multiple channels it is using
 *				this approach due to stronger compatibility with the "standard" HIF.
 *
 *				For more information please see the @see pfe_hif_chnl_t and @see pfe_hif_t.
 *
 *				Initialization
 *				--------------
 *				To create a HIF_NOCPY instance one shall call the pfe_hif_nocpy_create() with valid
 *				parameters.	Once created, instance is ready to be used. Data-path manipulation is done
 *				via pfe_hif_chnl_t instance as mentioned above. To retrieve a channel instance the
 *				pfe_hif_nocpy_get_channel() function shall be called.
 *
 *				Shutdown
 *				--------
 *				When the HIF is no more needed it shall be properly terminated by pfe_hif_nocpy_destroy().
 *				The call will ensure that all instance-related resources will be released and the HIF_NOCPY
 *				hardware will be finalized in way that it can be used again later.
 *
 */

#include "oal.h"
#include "hal.h"

#include "pfe_cbus.h"
#include "pfe_mmap.h"
#include "pfe_hif_nocpy.h"

struct __pfe_hif_nocpy_tag
{
	void *base_va;						/*	CBUS base virtual address */
	pfe_hif_chnl_t *channel;			/*	Associated channel instance */
};

/**
 * @brief		Create new HIF_NOCPY instance
 * @details		Creates and initializes HIF_NOCPY instance
 * @param[in]	base_va HIF_NOCPY base virtual address
 * @param[in]	bmu BMU providing buffers for HIF NOCPY operation
 * @return		The HIF_NOCPY instance or NULL if failed
 */
pfe_hif_nocpy_t *pfe_hif_nocpy_create(void *base_va, pfe_bmu_t *bmu)
{
	pfe_hif_nocpy_t *hif;
	errno_t ret;

#if defined(GLOBAL_CFG_NULL_ARG_CHECK)
	if (unlikely((NULL == base_va) || (NULL == bmu)))
	{
		NXP_LOG_ERROR("NULL argument received\n");
		return NULL;
	}
#endif /* GLOBAL_CFG_NULL_ARG_CHECK */

	hif = oal_mm_malloc(sizeof(pfe_hif_nocpy_t));

	if (NULL == hif)
	{
		return NULL;
	}
	else
	{
		memset(hif, 0, sizeof(pfe_hif_nocpy_t));
		hif->base_va = base_va;
	}

	hif->channel = pfe_hif_chnl_create(hif->base_va, PFE_HIF_CHNL_NOCPY_ID, bmu);
	if (NULL == hif->channel)
	{
		NXP_LOG_ERROR("Can't create HIF_NOCPY channel instance\n");
		oal_mm_free(hif);
		return NULL;
	}

	ret = pfe_hif_nocpy_cfg_init(hif->base_va);
	if (EOK != ret)
	{
		NXP_LOG_ERROR("HIF_NOCPY configuration failed: %d\n", ret);
		oal_mm_free(hif);
		return NULL;
	}

	return hif;
}

/**
 * @brief		Get channel instance according to its ID
 * @param[in]	hif The HIF instance
 * @param[in]	channel_id The channel ID. Currently only PFE_HIF_CHNL_NOCPY_ID is supported.
 * @return		The HIF channel instance or NULL if failed
 */
pfe_hif_chnl_t *pfe_hif_nocpy_get_channel(pfe_hif_nocpy_t *hif, uint32_t channel_id)
{
#if defined(GLOBAL_CFG_NULL_ARG_CHECK)
	if (unlikely(NULL == hif))
	{
		NXP_LOG_ERROR("NULL argument received\n");
		return NULL;
	}
#endif /* GLOBAL_CFG_NULL_ARG_CHECK */

	if (channel_id != PFE_HIF_CHNL_NOCPY_ID)
	{
		return NULL;
	}

	return hif->channel;
}

/**
 * @brief		Destroy HIF_NOCPY instance
 * @param[in]	hif The HIF_NOCPY instance
 */
void pfe_hif_nocpy_destroy(pfe_hif_nocpy_t *hif)
{
	if (NULL != hif)
	{
		pfe_hif_nocpy_cfg_fini(hif->base_va);
		oal_mm_free(hif);
	}
}

/**
 * @brief		Return HIF_NOCPY runtime statistics in text form
 * @details		Function writes formatted text into given buffer.
 * @param[in]	hif 		The HIF_NOCPY instance
 * @param[in]	buf 		Pointer to the buffer to write to
 * @param[in]	buf_len 		Buffer length
 * @param[in]	verb_level 	Verbosity level, number of data written to the buffer
 * @return		Number of bytes written to the buffer
 */
uint32_t pfe_hif_nocpy_get_text_statistics(pfe_hif_nocpy_t *hif, char_t *buf, uint32_t buf_len, uint8_t verb_level)
{
	uint32_t len = 0U;

#if defined(GLOBAL_CFG_NULL_ARG_CHECK)
	if (unlikely(NULL == hif))
	{
		NXP_LOG_ERROR("NULL argument received\n");
		return 0U;
	}
#endif /* GLOBAL_CFG_NULL_ARG_CHECK */

	len += pfe_hif_nocpy_cfg_get_text_stat(hif->base_va, buf, buf_len, verb_level);

	return len;
}


/** @}*/