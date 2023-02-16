/*
** Copyright (C) 2017 Valentin Zotov <iconn8@gmail.com>.
** Author: Valentin Zotov; contact e-mail: iconn8@gmail.com.
**
** Commercial License Usage
** Licensees holding valid commercial author licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and the author. For licensing terms
** and conditions contact the author.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**/


#include "SMO_lib.h"
#include "Offseters.h"
#include "tdibus.h"
#include "tdi.h"
#include "tdiroot.h"

tdifunc TDIRootDefault(uint64_t* data, void* env, smoDataSize_t data_lenb);
tdifunc TDIRootExtended(uint64_t* data, void* env, smoDataSize_t data_lenb);


///TDIRootDefault() may be used as stand alone function for a root device item
/// or may be co-exist with a user hand-shake functions which may only append a user data after this!!!
tdifunc
TDIRootDefault(uint64_t *data, void* env, uintp data_lenb)
{
    (void)env;
    (void)data_lenb;
    *data = (uint64_t)TdiBus_version|(Smo_version<<8)|(TDI_IANA_CHARACTER_SET<<16)|((uint64_t)TDI_CRC<<32);
    return msg_Success;
}

tdifunc
TDIRootExtended(uint64_t *data, void* env, uintp data_lenb)
{
    switch (data_lenb) {
    case SmoBusRoot_get_tdi_info:
        TDIRootDefault(data, env, data_lenb);
        break;
    case SmoBusRoot_get_id:
    { /* TODO: */}
        break;
    case SmoBusRoot_get_sec_data:
    { /* TODO: */}
        break;
    case SmoBusRoot_set_sec_data:
    {
        Offseter_global_set(*(char*)data);
    }
        break;
    default:
        return err_Unsupported;
    }
    return msg_Success;
}

