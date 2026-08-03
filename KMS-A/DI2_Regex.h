
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Regex.h

#pragma once

#define DI2_Regex_BEGIN   "^"
#define DI2_Regex_DOT     "\\."
#define DI2_Regex_NAME_C1 "(\\w+)"
#define DI2_Regex_SPACE   "\\s*"


#define DI2_Regex_ASSIGN_OPT DI2_Regex_SPACE "=?"

#define DI2_Regex_DEC_C1     DI2_Regex_SPACE "(\\d+)" DI2_Regex_SPACE
#define DI2_Regex_HEX_C1     DI2_Regex_SPACE "(0x[0-9A-Fa-f]+)" DI2_Regex_SPACE

#define DI2_Regex_FIELD_END  DI2_Regex_BEGIN DI2_Regex_SPACE ";" DI2_Regex_SPACE

#define DI2_Regex_OP_C1      DI2_Regex_SPACE "([\\+\\-\\*/\\|&\\^]{0,1}=)"


#define DI2_Regex_GROUP_BEGIN  DI2_Regex_BEGIN DI2_Regex_ASSIGN_OPT DI2_Regex_SPACE "\\{" DI2_Regex_SPACE
#define DI2_Regex_GROUP_END    DI2_Regex_BEGIN DI2_Regex_SPACE "\\}" DI2_Regex_SPACE

#define DI2_Regex_INDEX_DEC_C1 DI2_Regex_BEGIN DI2_Regex_SPACE "\\[" DI2_Regex_DEC_C1 "\\]" DI2_Regex_SPACE
#define DI2_Regex_INDEX_HEX_C1 DI2_Regex_BEGIN DI2_Regex_SPACE "\\[" DI2_Regex_HEX_C1 "\\]" DI2_Regex_SPACE

